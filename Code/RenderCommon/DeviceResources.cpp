#include "StdAfx.h"
#include "DeviceResources.h"

#if RENDERER_DX11
	#include "DX11DeviceResources_impl.inl"
#elif RENDERER_VULKAN
	#include "VKDeviceResources_impl.inl"
#endif

ResourceLayout DeviceResource::GetLayout() const
{
	ResourceLayout layout = {};

	if (!GetRawResource())
		return layout;

	D3D11_RESOURCE_DIMENSION dim;
	GetRawResource()->GetType(&dim);

	switch (dim)
	{
	case D3D11_RESOURCE_DIMENSION_BUFFER:
	{
		D3D11_BUFFER_DESC desc;
		reinterpret_cast<IGpuBuffer*>(GetRawResource())->GetDesc(&desc);
		layout.m_bytesCount		= desc.ByteWidth;
		layout.m_elementsCount	= desc.ByteWidth / (m_rawFormat == DXGI_FORMAT_UNKNOWN ? std::max(1U, desc.StructureByteStride) : DeviceFormats::GetStride(m_rawFormat));
		layout.m_flags			= ToIntermediateFlags(desc);
	}
	break;
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
	{
		D3D11_TEXTURE2D_DESC desc;
		reinterpret_cast<Texture2D*>(GetRawResource())->GetDesc(&desc);
		layout.m_bytesCount		= desc.Width * desc.Height * desc.ArraySize * DeviceFormats::GetStride(desc.Format);
		layout.m_elementsCount	= desc.Width * desc.Height * desc.ArraySize;
		layout.m_flags			= ToIntermediateFlags(desc);
	}
	break;
	}

	return layout;
}

void DeviceResource::InitResourceViews()
{
	const DXGI_FORMAT formatT	= m_rawFormat;
	const DXGI_FORMAT formatD	= IsDepthStencil() ? DeviceFormats::ConvertToDepthOnly(m_rawFormat) : m_rawFormat;
	const DXGI_FORMAT formatS	= IsDepthStencil() ? DeviceFormats::ConvertToStencilOnly(m_rawFormat) : m_rawFormat;
	const DXGI_FORMAT formatDS	= IsDepthStencil() ? DeviceFormats::ConvertToDepthStencil(m_rawFormat) : m_rawFormat;

	const ResourceLayout layout	= GetLayout();
	const UINT mipsCount		= ~0;
	const UINT slicesCount		= ~0;
	const UINT elementsCount	= layout.m_elementsCount;

	ResourceView RTV = ResourceView::RenderTargetView(formatT);
	ResourceView UAV = ResourceView::UnorderedAccessView(formatT, 0);
	ResourceView SRV = ResourceView::ShaderResourceView(formatD, 0);
	ResourceView DSV = ResourceView::DepthStencilView(formatDS, 0);

	if (IsShaderResource() && formatD != DXGI_FORMAT_UNKNOWN)
		GetOrCreateResourceView(SRV);
	else ReserveResourceView(SRV);

	if (IsRenderTarget()) GetOrCreateResourceView(RTV);
	else ReserveResourceView(RTV);

	if (IsUnorderedAccess()) GetOrCreateResourceView(UAV);
	else ReserveResourceView(UAV);

	if (IsDepthStencil()) GetOrCreateResourceView(DSV);
	else ReserveResourceView(DSV);
}

ResourceViewType DeviceResource::GetOrCreateResourceView(const ResourceView& desc)
{
	for (int i = 0; i < m_resourceViews.size(); ++i)
	{
		auto it = m_resourceViews[i];
		if (it.first == desc)
			return ResourceViewType(i);
	}

	m_resourceViews.emplace_back(std::make_pair(desc, CreateResourceView(desc)));
	return ResourceViewType(m_resourceViews.size());
}

ResourceViewType DeviceResource::ReserveResourceView(const ResourceView& desc)
{
	const uint32 arrSize = m_resourceViews.size();

	// Reservation is unfailable
	assert(arrSize == uint32(ResourceViewType(arrSize)));
	m_resourceViews.emplace_back(std::make_pair(desc, nullptr));
	return ResourceViewType(arrSize);
}

void DeviceResource::ReserveResourceViews(int count)
{
	m_resourceViews.reserve(count);
}

void DeviceResource::ReleaseResourceViews()
{
	m_resourceViews.clear();
}

const std::pair<ResourceView, GpuResource*> DeviceResource::LookupResourceView(ResourceViewType type) const
{
	return std::pair<ResourceView, GpuResource*>({}, nullptr);
}

std::pair<ResourceView, GpuResource*> DeviceResource::LookupResourceView(ResourceViewType type)
{
	return std::pair<ResourceView, GpuResource*>({}, nullptr);
	//for (const auto& it : m_resourceViews)
	//{
	//	if (it.first.Desc.ViewType == type)
	//	{
	//		return std::pair(it.first, nullptr);
	//	}
	//}
}

void DeviceResource::SetRawResource(GpuResource* pRes)
{
	m_pRawResource = pRes;
}

DeviceTexture::DeviceTexture()
{
}

DeviceTexture::~DeviceTexture()
{
}

TextureLayout DeviceTexture::GetLayout() const
{
	return TextureLayout();
}

void DeviceTexture::Release()
{
	delete this;
}

DeviceTexture* DeviceTexture::Create(const TextureLayout& layout, const TextureData& texData)
{
	DeviceTexture* pDevTex	= nullptr;
	HRESULT hr				= S_OK;

	uint32 width			= layout.Width;
	uint32 height			= layout.Height;
	uint32 depth			= layout.Depth;
	uint8  mipsCount		= layout.MipsCount;
	uint32 arraySize		= layout.ArraySize;
	DXGIFormat format		= DeviceFormats::ConvertFromTexFormat(layout.Format);

	uint32 flags = 0;
	if (layout.Flags & TextureFlags::ShaderResource)
		flags |= ResourceFlags::BIND_SHADER_RESOURCE;
	if (layout.Flags & TextureFlags::DepthStencil)
		flags |= ResourceFlags::BIND_DEPTH_STENCIL;
	if (layout.Flags & TextureFlags::RenderTarget)
		flags |= ResourceFlags::BIND_RENDER_TARGET;
	if (layout.Flags & TextureFlags::UnorderedAccess)
		flags |= ResourceFlags::BIND_UNORDERED_ACCESS;
	if (layout.Flags & TextureFlags::UAVTextureRW)
		flags |= ResourceFlags::USAGE_UAV_READWRITE;
	if (layout.Flags & TextureFlags::UseMips)
		flags |= ResourceFlags::USAGE_AUTOGENMIPS;

	//if (layout.Flags & (TextureFlags::RenderTarget | TextureFlags::DepthStencil | TextureFlags::UnorderedAccess))
	// TODO: Alloc render target data here

	DXGIFormat formatOG		= format;
	DXGIFormat formatSRGB	= format;

	bool isSRGB = layout.IsSRGB && (layout.Flags & (TextureFlags::UseMSAA | TextureFlags::RenderTarget | TextureFlags::DepthStencil)) == 0;
	if (isSRGB || (layout.Flags & TextureFlags::AllowSRGB))
		formatSRGB = DeviceFormats::ConvertToSRGB(format);

	if (isSRGB)
		format = formatSRGB;

	if (layout.Flags & TextureFlags::AllowSRGB)
		format = DeviceFormats::ConvertToTypeless(format);

	if (layout.Type == TextureType::Tex2D || layout.Type == TextureType::Tex2DArray)
	{
		if (layout.Flags & TextureFlags::UseMSAA)
		{
			//assert(pRenderTargetData);
			// TODO: Alloc and create MSAA texture here.
			//hr = DeviceFactory::Get().CreateTexture2D(width, height, mipsCount, arraySize, flags, layout.ClearColor, format, &pRenderTargetData->m_pDeviceTextureMSAA, &pDevTex);
			//assert(SUCCEEDED(hr));
		}

		hr = DeviceFactory::Get().CreateTexture2D(width, height, mipsCount, arraySize, flags, layout.ClearColor, format, false, texData, &pDevTex);
		if (FAILED(hr))
		{
			assert(0);
			return nullptr;
		}

		if (layout.Flags & TextureFlags::AllowSRGB)
			format = formatOG;
	}
	else
	{
		assert(0);	// TODO: Not supported as of yet.
		return nullptr;
	}

	if (pDevTex)
	{
		// TODO: Set MSAA member pointer here.
		pDevTex->m_rawFormat							= format;
		pDevTex->m_flags								= flags;
		//pDevTex->m_resourceElements						= Texture::TextureDataSize(width, height, depth, mipsCount, arraySize, TextureFormat::A8);
		//pDevTex->m_subResources[eSubResource_Mips]		= nMips;
		//pDevTex->m_subResources[eSubResource_Slices]	= nArraySize;
		pDevTex->m_textureType							= layout.Type;
		pDevTex->m_isFilterable							= true;
		pDevTex->m_isSRGB								= isSRGB;
		pDevTex->m_allowSRGB							= layout.Flags & TextureFlags::AllowSRGB;
		pDevTex->m_isMSAA								= false;

		pDevTex->InitResourceViews();
	}

	// TODO: Fill MSAA texture data here.
	return pDevTex;
}

BufferLayout DeviceBuffer::GetLayout() const
{
	return BufferLayout();
}

int32 DeviceBuffer::Release()
{
	int32 refCount = 0;// DeviceResource::Clean();
	assert(refCount >= 0);
	if (refCount == 0)
		delete this;
	return refCount;
}

DeviceBuffer* DeviceBuffer::Create(const BufferLayout& pLayout, const void* pData)
{
	DeviceBuffer*	pDevBuffer	= nullptr;
	IGpuBuffer*		pBuffer		= nullptr;

	size_t elementSize	= pLayout.ElementSize;
	size_t elementCount	= pLayout.ElementCount;
	uint32 flags		= pLayout.Flags; // No FT_ flags used here, no mapping of flags required
	size_t size			= elementSize * elementCount;

	assert(elementSize > 0);
	assert(size > 0);

	static const uint32 bindFlagMask =
		ResourceFlags::BIND_DEPTH_STENCIL    |
		ResourceFlags::BIND_RENDER_TARGET    |
		ResourceFlags::BIND_UNORDERED_ACCESS |
		ResourceFlags::BIND_VERTEX_BUFFER    |
		ResourceFlags::BIND_INDEX_BUFFER     |
		ResourceFlags::BIND_CONST_BUFFER     |
		ResourceFlags::BIND_SHADER_RESOURCE;

	const HRESULT hr = DeviceFactory::CreateBuffer(elementCount, elementSize, flags & ~bindFlagMask, flags & bindFlagMask, &pBuffer, pData);
	if (FAILED(hr))
	{
		assert(false);
		return nullptr;
	}

	pDevBuffer						= new DeviceBuffer();
	pDevBuffer->m_pRawResource		= pBuffer;
	pDevBuffer->m_rawFormat			= pLayout.Format;
	pDevBuffer->m_flags				= pLayout.Flags;
	pDevBuffer->m_resourceElements	= elementCount * elementSize;
	//pDevBuffer->m_subResources[eSubResource_Mips] = 0;
	//pDevBuffer->m_subResources[eSubResource_Slices] = 0; 
	pDevBuffer->m_textureType		= TextureType::Unknown;
	pDevBuffer->m_isFilterable		= false;
	pDevBuffer->m_isSRGB			= false;
	pDevBuffer->m_allowSRGB			= false;
	pDevBuffer->m_isMSAA			= false;
	pDevBuffer->InitResourceViews();

	return pDevBuffer;
}

DeviceBuffer* DeviceBuffer::Associate(const BufferLayout& pLayout, GpuResource* pBuf)
{
	return nullptr;
}
