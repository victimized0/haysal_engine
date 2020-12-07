#include "StdAfx.h"
#include "BuffersManager.h"

BufferManager::BufferManager()
{
}

BufferManager::~BufferManager()
{
	ShutDown();
}

bool BufferManager::Init()
{
	return true;
}

void BufferManager::ShutDown()
{
	for (GpuBuffer* buf : m_buffersPool)
		SAFE_RELEASE(buf);
	m_buffersPool.clear();
}

GpuBuffer* BufferManager::Create(BufferType type, BufferUsage usage, uint32 bindFlags, uint32 usageFlags, size_t size)
{
	const BufferLayout layout =
	{
		DXGI_FORMAT_UNKNOWN,
		size,
		1,
		usageFlags | bindFlags
	};

	GpuBuffer* pBuffer = new GpuBuffer();
	pBuffer->Create(layout.ElementSize, layout.ElementCount, layout.Format, layout.Flags, nullptr);
	m_buffersPool.push_back(pBuffer);
	return pBuffer;
}

void BufferManager::Destroy(GpuBuffer* pBuffer)
{
	for (size_t i = 0; i < m_buffersPool.size(); ++i)
	{
		if (pBuffer == m_buffersPool[i])
		{
			m_buffersPool.erase(m_buffersPool.begin() + i);
		}
	}
}

void BufferManager::Read(GpuBuffer* pBuffer, void* pOutData, size_t size, size_t offset)
{
	assert(pBuffer);
	assert(pOutData);

	if (!pBuffer->IsAvailable())
		return;

	DeviceFactory::DownloadContents(pBuffer->GetDevBuffer()->GetBuffer(), 0, offset, size, D3D11_MAP::D3D11_MAP_READ, pOutData);
}

void BufferManager::Write(GpuBuffer* pBuffer, void* pInData, size_t size, size_t offset)
{
	assert(pBuffer);
	assert(pInData);

	if (!pBuffer->IsAvailable())
		return;

	DeviceFactory::UploadContents(pBuffer->GetDevBuffer()->GetBuffer(), 0, offset, size, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, pInData);
}
