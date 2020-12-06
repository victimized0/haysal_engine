#include "StdAfx.h"
#include "BuffersManager.h"

BuffersManager::BuffersManager()
{
}

BuffersManager::~BuffersManager()
{
	ShutDown();
}

bool BuffersManager::Init()
{
	return true;
}

void BuffersManager::ShutDown()
{
	for (GpuBuffer* buf : m_buffersPool)
		SAFE_RELEASE(buf);
	m_buffersPool.clear();
}

GpuBuffer* BuffersManager::Create(BufferType type, BufferUsage usage, uint32 bindFlags, uint32 usageFlags, size_t size)
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

void BuffersManager::Destroy(GpuBuffer* pBuffer)
{
	for (size_t i = 0; i < m_buffersPool.size(); ++i)
	{
		if (pBuffer == m_buffersPool[i])
		{
			m_buffersPool.erase(m_buffersPool.begin() + i);
		}
	}
}
