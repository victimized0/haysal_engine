#include "StdAfx.h"
#include "DeviceResources.h"

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
		reinterpret_cast<GpuBuffer*>(GetRawResource())->GetDesc(&desc);
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

	ResourceView RTV = ResourceView::RenderTarget(formatT);
	ResourceView UAV = ResourceView::UnorderedAccess(formatT, 0);
	ResourceView SRV = ResourceView::ShaderResource(formatD, 0);
	ResourceView DSV = ResourceView::DepthStencil(formatDS, 0);

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

void DeviceResource::GetOrCreateResourceView(const ResourceView& desc)
{
}

void DeviceResource::ReserveResourceView(const ResourceView& desc)
{
}

void DeviceResource::ReleaseResourceViews()
{
}

void DeviceResource::SetRawResource(GpuResource* pRes)
{
	m_pRawResource = pRes;
}

DeviceTexture::DeviceTexture()
	: m_textureType(TextureType::Unknown)
	, m_isFilterable(false)
	, m_isSRGB(false)
	, m_allowSRGB(false)
	, m_isMSAA(false)
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

// TODO: Needs to be Graphics API-specific
DeviceTexture* DeviceTexture::Create(const TextureLayout& layout, const TextureData& texData)
{
	DeviceTexture* pDevTex = nullptr;
	HRESULT hr = S_OK;

	uint32 width		= layout.Width;
	uint32 height		= layout.Height;
	uint32 depth		= layout.Depth;
	uint8  mipsCount	= layout.MipsCount;
	uint32 arraySize	= layout.ArraySize;
	DXGIFormat format	= DeviceFormats::ConvertFromTexFormat(layout.Format);

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

	DXGIFormat formatOG = format;
	DXGIFormat formatSRGB = format;

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
