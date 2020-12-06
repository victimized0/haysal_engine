#ifndef DX11_DEVICE_RESOURCES_IMPL_INL
#define DX11_DEVICE_RESOURCES_IMPL_INL
#pragma once

#include "DX11Renderer.h"

GpuView* DeviceResource::CreateResourceView(const ResourceView& view)
{
	HRESULT hr				= E_FAIL;
	GpuView* pResult		= nullptr;
	GpuResource* pResource	= GetRawResource();

	if (!pResource) return pResult;

	const uint32 mipsCount			= view.Desc.MipsCount	== ResourceView(0ULL).Desc.MipsCount	|| view.Desc.MipsCount		== ResourceView(~0ULL).Desc.MipsCount	? (uint32)-1 : (uint32)view.Desc.MipsCount;
	const uint32 slicesCount		= view.Desc.SlicesCount	== ResourceView(0ULL).Desc.SlicesCount	|| view.Desc.SlicesCount	== ResourceView(~0ULL).Desc.SlicesCount	? (uint32)-1 : (uint32)view.Desc.SlicesCount;
	//const uint32 nElemsAvailable	= m_resourceElements;
	//const uint32 nMipAvailable		= m_subResources[eSubResource_Mips];
	const uint32 AvailableSliceNum = 0;// m_subResources[eSubResource_Slices];

	if (view.Desc.ViewType == ResourceView::Type::SRV)
	{
		DXGI_FORMAT format = DXGI_FORMAT(view.Desc.Format);
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = format;

		if (DeviceFormats::IsSRGBReadable(format))
		{
			if (view.Desc.IsSrgbRead)
				srvDesc.Format = DeviceFormats::ConvertToSRGB(format);
		}

		if (view.Desc.IsMultisampled && m_textureType == TextureType::Tex2D)
		{
			srvDesc.ViewDimension = AvailableSliceNum > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY : D3D11_SRV_DIMENSION_TEXTURE2DMS;

			if (AvailableSliceNum > 1)
			{
				srvDesc.Texture2DMSArray.FirstArraySlice	= view.Desc.FirstSliceNum;
				srvDesc.Texture2DMSArray.ArraySize			= slicesCount;
			}
		}
		else if (m_isFilterable)
		{
			srvDesc.ViewDimension =
					m_textureType == TextureType::Tex1D			? (AvailableSliceNum > 1 ? D3D11_SRV_DIMENSION_TEXTURE1DARRAY	: D3D11_SRV_DIMENSION_TEXTURE1D)
				: (	m_textureType == TextureType::Tex2D			? (AvailableSliceNum > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DARRAY	: D3D11_SRV_DIMENSION_TEXTURE2D)
				: (	m_textureType == TextureType::TexCube		? (AvailableSliceNum > 6 ? D3D11_SRV_DIMENSION_TEXTURECUBEARRAY	: D3D11_SRV_DIMENSION_TEXTURECUBE)
				: (	m_textureType == TextureType::Tex3D			? (AvailableSliceNum > 1 ? D3D11_SRV_DIMENSION_UNKNOWN			: D3D11_SRV_DIMENSION_TEXTURE3D)
				: (	m_textureType == TextureType::Tex2DArray	? D3D11_SRV_DIMENSION_TEXTURE2DARRAY
				: (	m_textureType == TextureType::TexCubeArray	? D3D11_SRV_DIMENSION_TEXTURECUBEARRAY							: D3D11_SRV_DIMENSION_UNKNOWN)))));

			// D3D11_TEX1D_SRV, D3D11_TEX2D_SRV, D3D11_TEX3D_SRV, D3D11_TEXCUBE_SRV and array versions can be aliased here
			srvDesc.Texture1D.MostDetailedMip	= view.Desc.MostDetailedMipNum;
			srvDesc.Texture1D.MipLevels			= mipsCount;

			if (AvailableSliceNum > 1)
			{
				srvDesc.Texture1DArray.FirstArraySlice	= view.Desc.FirstSliceNum;
				srvDesc.Texture1DArray.ArraySize		= slicesCount;
			}
		}
		else
		{
			srvDesc.ViewDimension = /*view.Desc.IsRaw ? D3D11_SRV_DIMENSION_BUFFEREX :*/ D3D11_SRV_DIMENSION_BUFFER;
		}
		
		GpuSRV* pSRV = nullptr;
		hr = dynamic_cast<DX11Renderer*>(gRenderer)->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, &pSRV);
		pResult = pSRV;
	}
	else // RTV / DSV / UAV
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = DXGI_FORMAT(view.Desc.Format);

		if (view.Desc.IsMultisampled && m_textureType == TextureType::Tex2D && view.Desc.ViewType != ResourceView::Type::UAV)
		{
			rtvDesc.ViewDimension = AvailableSliceNum > 1 ? D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY : D3D11_RTV_DIMENSION_TEXTURE2DMS;

			if (AvailableSliceNum > 1)
			{
				rtvDesc.Texture2DMSArray.FirstArraySlice = view.Desc.FirstSliceNum;
				rtvDesc.Texture2DMSArray.ArraySize = slicesCount;
			}
		}
		else if (m_isFilterable)
		{
			rtvDesc.ViewDimension =
					m_textureType == TextureType::Tex1D			? (AvailableSliceNum > 1 ? D3D11_RTV_DIMENSION_TEXTURE1DARRAY	: D3D11_RTV_DIMENSION_TEXTURE1D)
				: (	m_textureType == TextureType::Tex2D			? (AvailableSliceNum > 1 ? D3D11_RTV_DIMENSION_TEXTURE2DARRAY	: D3D11_RTV_DIMENSION_TEXTURE2D)
				: (	m_textureType == TextureType::TexCube		? D3D11_RTV_DIMENSION_TEXTURE2DARRAY
				: (	m_textureType == TextureType::Tex3D			? (AvailableSliceNum > 1 ? D3D11_RTV_DIMENSION_UNKNOWN			: D3D11_RTV_DIMENSION_TEXTURE3D)
				: (	m_textureType == TextureType::Tex2DArray	? D3D11_RTV_DIMENSION_TEXTURE2DARRAY
				: (	m_textureType == TextureType::TexCubeArray	? D3D11_RTV_DIMENSION_TEXTURE2DARRAY							: D3D11_RTV_DIMENSION_UNKNOWN)))));

			rtvDesc.Texture1D.MipSlice = view.Desc.MostDetailedMipNum;

			if (AvailableSliceNum > 1 || m_textureType == TextureType::Tex3D || m_textureType == TextureType::TexCube || m_textureType == TextureType::TexCubeArray)
			{
				rtvDesc.Texture1DArray.FirstArraySlice = view.Desc.FirstSliceNum;
				rtvDesc.Texture1DArray.ArraySize = slicesCount;
			}
		}
		else
		{
			rtvDesc.ViewDimension = /*view.Desc.IsRaw ? D3D11_RTV_DIMENSION_BUFFER :*/ D3D11_RTV_DIMENSION_BUFFER;
		}

		switch (view.Desc.ViewType)
		{
		case ResourceView::Type::RTV:
		{
			GpuRTV* pRTV = nullptr;
			hr = dynamic_cast<DX11Renderer*>(gRenderer)->GetDevice()->CreateRenderTargetView(pResource, &rtvDesc, &pRTV);
			pResult = pRTV;
		}
		break;
		case ResourceView::Type::DSV:
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
			dsvDesc.Format = DXGI_FORMAT(view.Desc.Format);

			dsvDesc.Flags = view.Desc.Flags;
			memcpy(&dsvDesc.Texture1DArray, &rtvDesc.Texture1DArray, sizeof(dsvDesc.Texture1DArray));

			// Depth/Stencil read only DSV
			if (view.Desc.Flags & ResourceView::Flags::DSV_ReadOnly)
				dsvDesc.Flags |= (D3D11_DSV_READ_ONLY_DEPTH | D3D11_DSV_READ_ONLY_STENCIL);
			// Depth/Stencil read/write DSV
			else
				dsvDesc.Flags &= ~(D3D11_DSV_READ_ONLY_DEPTH | D3D11_DSV_READ_ONLY_STENCIL);

			if (rtvDesc.ViewDimension != D3D11_RTV_DIMENSION_UNKNOWN &&
				rtvDesc.ViewDimension != D3D11_RTV_DIMENSION_TEXTURE3D)
				dsvDesc.ViewDimension = (D3D11_DSV_DIMENSION)(rtvDesc.ViewDimension - 1);

			GpuDSV* pDSV = nullptr;
			hr = dynamic_cast<DX11Renderer*>(gRenderer)->GetDevice()->CreateDepthStencilView(pResource, &dsvDesc, &pDSV);
			pResult = pDSV;
		}
		break;
		case ResourceView::Type::UAV:
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
			memcpy(&uavDesc, &rtvDesc, sizeof(uavDesc));

			if (view.Desc.Flags & ResourceView::Flags::UAV_ReadWrite)
				uavDesc.Format = DXGI_FORMAT_R32_UINT;
			//if (view.Desc.IsRaw)
			//	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;

			if (rtvDesc.ViewDimension == D3D11_RTV_DIMENSION_TEXTURE2DMS ||
				rtvDesc.ViewDimension == D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY)
				uavDesc.ViewDimension = D3D11_UAV_DIMENSION_UNKNOWN;

			GpuUAV* pUAV = nullptr;
			hr = dynamic_cast<DX11Renderer*>(gRenderer)->GetDevice()->CreateUnorderedAccessView(pResource, &uavDesc, &pUAV);
			pResult = pUAV;
		}
		break;
		}
	}

	if (FAILED(hr))
	{
		assert(false);
		return nullptr;
	}

	return pResult;
}

#endif //DX11_DEVICE_RESOURCES_IMPL_INL