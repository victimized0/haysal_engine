#include "StdAfx.h"
#include "DeviceObjects.h"

DeviceFactory DeviceFactory::m_sInst;
std::vector<InputLayout> DeviceFactory::s_inputLayoutsCache;
std::map<VertexFormat, InputLayoutPair> DeviceFactory::s_inputLayoutCompositions;

DeviceFactory::DeviceFactory()
	: m_pDevice(nullptr)
#if RENDERER_DX11
	, m_pDevCon(nullptr)
#endif
{

}

DeviceFactory::~DeviceFactory()
{
	ReleaseResources();
}

void DeviceFactory::ReleaseResources()
{
	ReleaseInputLayouts();
}

void DeviceFactory::AllocateDefaultInputLayouts()
{
	s_inputLayoutsCache.reserve(static_cast<size_t>(VertexFormat::Total));
	for (size_t i = 0; i < static_cast<size_t>(VertexFormat::Total); ++i)
		CreateVertexFormat(InputLayout::InputLayoutDescs[i].ElementsCount, InputLayout::InputLayoutDescs[i].ElementsDesc);
}

void DeviceFactory::ReleaseInputLayouts()
{
	s_inputLayoutsCache.clear();
}

void DeviceFactory::DeleteCustomInputLayouts()
{
	s_inputLayoutsCache.erase(s_inputLayoutsCache.begin() + static_cast<uint32>(VertexFormat::Total), s_inputLayoutsCache.end());
}

InputLayout DeviceFactory::CreateInputLayout(const ShaderBlob* pVertexShader, const VertexFormat format)
{
	const InputLayout* layout = &s_inputLayoutsCache[static_cast<size_t>(format)];
	if (layout == nullptr)
		return InputLayout({});

	std::vector<D3D11_INPUT_ELEMENT_DESC> decs;
	decs.reserve(layout->m_ElementsDesc.size());
	for (int i = 0; i < layout->m_ElementsDesc.size(); ++i)
		decs.push_back(layout->m_ElementsDesc[i]);

	return InputLayout(std::move(decs));
}

const InputLayoutPair* DeviceFactory::GetOrCreateInputLayout(const ShaderBlob* pVertexShader, const VertexFormat format)
{
	assert(pVertexShader);
	if (!pVertexShader)
		return nullptr;

	void* pShaderReflect;
	{
		HRESULT hr = DxReflection(pVertexShader->m_pData, pVertexShader->m_size, IID_DxShaderReflection, &pShaderReflect);
	}
	DxShaderReflection* pShaderReflection = static_cast<DxShaderReflection*>(pShaderReflect);

	auto it = s_inputLayoutCompositions.find(format);
	if (it == s_inputLayoutCompositions.end() || it->first != format)
	{
		// Create the input layout for the current permutation
		InputLayout		inputLayout			= CreateInputLayout(pVertexShader, format);
		IVertexLayout*	pDeviceInputLayout	= CreateInputLayout(inputLayout, pVertexShader);

		auto pair = std::make_pair(inputLayout, pDeviceInputLayout);
		auto item = s_inputLayoutCompositions.insert(it, std::make_pair(format, pair));
		return &item->second;
	}

	SAFE_RELEASE(pShaderReflection);
	return &it->second;
}

VertexFormat DeviceFactory::CreateVertexFormat(size_t descCount, const D3D11_INPUT_ELEMENT_DESC* descArr)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> decs;
	for (int n = 0; n < descCount; ++n)
	{
		auto it = std::lower_bound(decs.begin(), decs.end(), descArr[n], [](const D3D11_INPUT_ELEMENT_DESC& lhs, const D3D11_INPUT_ELEMENT_DESC& rhs)
			{
				return strcmp(lhs.SemanticName, rhs.SemanticName) <= 0;
			});
		decs.insert(it, descArr[n]);
	}

	// Find existing vertex format or store a new one
	auto it = std::find(s_inputLayoutsCache.begin(), s_inputLayoutsCache.end(), decs);
	auto idx = it - s_inputLayoutsCache.begin();
	if (it == s_inputLayoutsCache.end())
		s_inputLayoutsCache.emplace_back(std::move(decs));

	return static_cast<VertexFormat>(idx);
}

HRESULT DeviceFactory::CreateTexture1D()
{
	return E_NOTIMPL;
}

HRESULT DeviceFactory::CreateTexture2D(uint16 width, uint16 height, uint16 mipsCount, uint16 arraySize, uint32 flags, Color clearCol, DXGIFormat format, bool isMSAA, const TextureData& texData, DeviceTexture** ppTex)
{	
	HRESULT hr = S_OK;
	Texture2D* pTex = nullptr;

	size_t numBytes = 0;
	size_t rowBytes = 0;
	size_t index = 0;

	const uint8* pSrcBits = texData.pData[0];
	D3D11_SUBRESOURCE_DATA* initData = nullptr;
	if (pSrcBits != nullptr)
	{
		initData = new D3D11_SUBRESOURCE_DATA[mipsCount * arraySize];
		for (size_t j = 0; j < arraySize; ++j)
		{
			size_t w = texData.Width;
			size_t h = texData.Height;
			size_t d = texData.Depth;

			for (size_t i = 0; i < mipsCount; ++i)
			{
				DeviceFormats::GetSurfaceInfo(w, h, texData.Format, &numBytes, &rowBytes, nullptr);

				assert(index < mipsCount* arraySize);
				initData[index].pSysMem = (const void*)pSrcBits;
				initData[index].SysMemPitch = static_cast<UINT>(rowBytes);
				initData[index].SysMemSlicePitch = static_cast<UINT>(numBytes);
				++index;

				pSrcBits += numBytes * d;

				w = w >> 1;
				h = h >> 1;
				d = d >> 1;

				if (w == 0) w = 1;
				if (h == 0) h = 1;
				if (d == 0) d = 1;
			}
		}
	}

	// The format(0x2d, D24_UNORM_S8_UINT) cannot be bound as a ShaderResource or cast to a format that could be bound as a ShaderResource.
	// Therefore this format cannot support D3D11_BIND_SHADER_RESOURCE.Specifiying the format R24G8_TYPELESS instead would solve this issue.
	if ((flags & ResourceFlags::BIND_DEPTH_STENCIL) && (flags & ResourceFlags::BIND_SHADER_RESOURCE) || (flags & ResourceFlags::USAGE_UAV_READWRITE))
		format = DeviceFormats::ConvertToTypeless(format);

	D3D11_TEXTURE2D_DESC desc	= {};
	desc.Width					= width;
	desc.Height					= height;
	desc.MipLevels				= mipsCount;
	desc.SampleDesc.Count		= 1; // TODO: Add MSAA support later
	desc.SampleDesc.Quality		= 0; // TODO: Add MSAA support later
	desc.Format					= format;
	desc.ArraySize				= arraySize;
	desc.BindFlags				= ConvertBindFlags(flags);
	desc.CPUAccessFlags			= ConvertCPUAccessFlags(flags);
	desc.Usage					= ConvertUsageFlags(flags);
	desc.MiscFlags				= ConvertMiscFlags(flags);

	// TODO: Incapsulate this to API-independent call
	hr = m_pDevice->CreateTexture2D(&desc, initData, &pTex);
	SAFE_DELETE_ARRAY(initData);

	if (SUCCEEDED(hr) && pTex)
	{
		DeviceTexture* pDeviceTexture = new DeviceTexture();
		pDeviceTexture->SetRawResource(pTex);
		*ppTex = pDeviceTexture;
	}

	return hr;
}

HRESULT DeviceFactory::CreateTexture3D()
{
	return E_NOTIMPL;
}

HRESULT DeviceFactory::CreateTextureCube()
{
	return E_NOTIMPL;
}

void DeviceFactory::ReleaseResource(GpuResource* pResource)
{
	pResource->Release();
}
