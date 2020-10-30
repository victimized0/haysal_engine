#include "StdAfx.h"
#include "DeviceObjects.h"

DeviceFactory DeviceFactory::m_sInst;
std::map<std::string, InputLayoutPair> DeviceFactory::s_inputLayouts;

DeviceFactory::DeviceFactory()
	: m_pDevice(nullptr)
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
}

void DeviceFactory::DeleteCustomInputLayouts()
{
	//s_inputLayouts.erase(s_inputLayouts.begin() + VertexFormat::Total, s_inputLayouts.end());
}

void DeviceFactory::ReleaseInputLayouts()
{
	//s_inputLayouts.clear();
}

//const IVertexLayout* DeviceFactory::GetOrCreateInputLayout(const ShaderBlob* pVS, const VertexFormat format)
//{
//	return nullptr;
//}
//
//const IVertexLayout* DeviceFactory::CreateCustomVertexFormat(size_t numDescs, const D3D11_INPUT_ELEMENT_DESC* inputLayout)
//{
//	return nullptr;
//}

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
	D3D11_SUBRESOURCE_DATA* initData = new D3D11_SUBRESOURCE_DATA[mipsCount * arraySize];
	for (size_t j = 0; j < arraySize; ++j)
	{
		size_t w = texData.Width;
		size_t h = texData.Height;
		size_t d = texData.Depth;

		for (size_t i = 0; i < mipsCount; ++i)
		{
			DeviceFormats::GetSurfaceInfo(w, h, texData.Format, &numBytes, &rowBytes, nullptr);

			assert(index < mipsCount* arraySize);
			initData[index].pSysMem				= (const void*)pSrcBits;
			initData[index].SysMemPitch			= static_cast<UINT>(rowBytes);
			initData[index].SysMemSlicePitch	= static_cast<UINT>(numBytes);
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
	desc.Usage					= ConvertUsage(flags);
	desc.MiscFlags				= ConvertMiscFlags(flags);

	// TODO: Incapsulate this to API-independent call
	hr = m_pDevice->CreateTexture2D(&desc, initData, &pTex);
	SAFE_DELETE(initData);

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
