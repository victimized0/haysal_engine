#include "StdAfx.h"
#include "GpuBuffer.h"

GpuBuffer::GpuBuffer(DeviceBuffer* pDevBuf = nullptr)
	: m_pDeviceBuffer(pDevBuf)
	, m_elementCount(0)
	, m_elementSize(0)
	, m_flags(0)
	, m_mapMode(D3D11_MAP(0))
	, m_isLocked(false)
{
}

GpuBuffer::~GpuBuffer()
{
	// Unbind??
	Release();
}

void GpuBuffer::Create(uint32 elementsCount, uint32 elementSize, DXGI_FORMAT elementFormat, uint32 flags, const void* pData)
{
	Release();

	m_mapMode = 
		((flags & ResourceFlags::BIND_SHADER_RESOURCE ) ? D3D11_MAP_WRITE_DISCARD :
		((flags & ResourceFlags::BIND_VERTEX_BUFFER   ) ? D3D11_MAP_WRITE_NO_OVERWRITE :
		((flags & ResourceFlags::BIND_INDEX_BUFFER    ) ? D3D11_MAP_WRITE_NO_OVERWRITE :
		((flags & ResourceFlags::BIND_UNORDERED_ACCESS) ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP(0)))));

	m_elementSize	= elementFormat == DXGI_FORMAT_UNKNOWN ? elementSize : DeviceFormats::GetStride(elementFormat);
	m_elementCount	= elementsCount;
	m_flags			= flags;
	m_format		= elementFormat;
	m_pDeviceBuffer = AllocateDeviceBuffer(pData);
}

void GpuBuffer::Release()
{
	if (m_pDeviceBuffer)
		ReleaseDeviceBuffer(m_pDeviceBuffer);

	m_elementCount	= 0;
	m_flags			= 0;
	m_isLocked		= false;
	m_mapMode		= D3D11_MAP(0);
}

void GpuBuffer::UpdateContent(const void* pData, uint32 size)
{
	assert(!m_isLocked);
	if (size)
	{
		if (m_flags & ResourceFlags::USAGE_CPU_WRITE)
		{
			// Transfer sub-set of GPU resource to CPU, also allows graphics debugger and multi-gpu broadcaster to do the right thing
			DeviceFactory::UploadContents(m_pDeviceBuffer->GetBuffer(), 0, 0, size, D3D11_MAP(m_mapMode), pData);
		}
		else
		{
			assert(false);
			// Not supported
		}
	}
}

void* GpuBuffer::Lock()
{
	assert(m_flags & ResourceFlags::USAGE_CPU_WRITE);
	assert(!m_isLocked);

	m_isLocked = true;
	return DeviceFactory::Map(m_pDeviceBuffer->GetBuffer(), 0, D3D11_MAP(m_mapMode));
}

void GpuBuffer::Unlock(uint32 size)
{
	assert(m_flags & ResourceFlags::USAGE_CPU_WRITE);
	assert(m_isLocked);

	m_isLocked = false;
	return DeviceFactory::Unmap(m_pDeviceBuffer->GetBuffer(), 0);
}

DeviceBuffer* GpuBuffer::AllocateDeviceBuffer(const void* pInitialData) const
{
    return DeviceBuffer::Create(GetLayout(), pInitialData);
}

void GpuBuffer::ReleaseDeviceBuffer(DeviceBuffer*& pDeviceBuffer) const
{
	SAFE_RELEASE(pDeviceBuffer);
}

bool ConstantBuffer::Update(const void* pData, size_t size, size_t offset)
{
	if (m_pBuffer == nullptr)
	{
		const BufferLayout layout =
		{
			DXGI_FORMAT_UNKNOWN,
			m_size,
			1,
			(ResourceFlags::USAGE_CPU_WRITE | ResourceFlags::BIND_CONST_BUFFER)
		};

		if (!(m_pBuffer = DeviceBuffer::Create(layout, nullptr)))
		{
			assert(false);
			return false;
		}
	}
	else
	{
		DeviceFactory::UploadContents(m_pBuffer->GetBuffer(), 0, offset, size, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, pData);
	}

	return true;
}
