#include "StdAfx.h"
#include "DeviceObjects.h"

IVertexLayout* DeviceFactory::CreateInputLayout(const InputLayout& layout, const ShaderBlob* pVertexShader)
{
	if (!pVertexShader || !pVertexShader->m_pData)
		return nullptr;

	const int   size	= pVertexShader->m_size;
	const void* pData	= pVertexShader->m_pData;

	IVertexLayout* pLayout = nullptr;
	if ( FAILED(Get().m_pDevice->CreateInputLayout(&layout.m_ElementsDesc[0], layout.m_ElementsDesc.size(), pData, size, &pLayout)) )
	{
		assert(false);
		return pLayout;
	}

	return pLayout;
}

HRESULT DeviceFactory::CreateBuffer(size_t size, size_t elemSize, uint32 usageFlags, uint32 bindFlags, IGpuBuffer** ppBuff, const void* pData /*= nullptr*/)
{
	HRESULT hr = S_OK;

	if (bindFlags & (BIND_RENDER_TARGET | BIND_DEPTH_STENCIL))
	{
		assert(false); // Isn't really a buffer
		return S_FALSE;
	}

	D3D11_BUFFER_DESC desc		= {};
	desc.StructureByteStride	= elemSize;
	desc.ByteWidth				= size * elemSize;
	desc.BindFlags				= ConvertBindFlags(bindFlags);
	desc.CPUAccessFlags			= ConvertCPUAccessFlags(usageFlags);
	desc.Usage					= ConvertUsageFlags(usageFlags);
	desc.MiscFlags				= ConvertMiscFlags(usageFlags);

	if ((usageFlags & USAGE_CPU_WRITE))
		desc.ByteWidth = BufferManager::AlignElementCount(size, elemSize) * elemSize;

	D3D11_SUBRESOURCE_DATA* pResource = nullptr;
	D3D11_SUBRESOURCE_DATA resource;
	if (pData)
	{
		pResource = &resource;

		resource.pSysMem			= pData;
		resource.SysMemPitch		= desc.ByteWidth;
		resource.SysMemSlicePitch	= desc.ByteWidth;
	}
	
	hr = Get().GetDevice()->CreateBuffer(&desc, pResource, ppBuff);
	return hr;
}

uint8* DeviceFactory::Map(IGpuBuffer* pBuffer, uint32 subresource, size_t offset, size_t size, D3D11_MAP mode)
{
	D3D11_MAPPED_SUBRESOURCE resource = { 0 };
	Get().GetContext()->Map(pBuffer, subresource, mode, 0, &resource);
	return reinterpret_cast<uint8*>(resource.pData);
}

void DeviceFactory::Unmap(IGpuBuffer* pBuffer, uint32 subresource)
{
	Get().GetContext()->Unmap(pBuffer, subresource);
}

void DeviceFactory::UploadContents(IGpuBuffer* pBuffer, uint32 subresource, size_t offset, size_t size, D3D11_MAP mode, const void* pInDataCPU, void* pOutDataGPU /*= nullptr*/, UINT numDataBlocks /*= 1*/)
{
	const uint8* pInData = reinterpret_cast<const uint8*>(pInDataCPU);
	uint8* pOutData = Map(pBuffer, subresource, offset, size, mode) + offset;
	memcpy(pOutData, pInData, size);
	Unmap(pBuffer, subresource);
}

void DeviceFactory::DownloadContents(IGpuBuffer* pBuffer, uint32 subresource, size_t offset, size_t size, D3D11_MAP mode, void* pOutDataCPU, const void* pInDataGPU /*= nullptr*/, UINT numDataBlocks /*= 1*/)
{
	uint8* pOutData = reinterpret_cast<uint8*>(pOutDataCPU);
	const uint8* pInData = Map(pBuffer, subresource, offset, size, mode) + offset;
	memcpy(pOutData, pInData, size);
	Unmap(pBuffer, subresource);
}
