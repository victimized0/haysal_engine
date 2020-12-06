#ifndef BUFFERS_MANAGER_H
#define BUFFERS_MANAGER_H
#pragma once

class GpuBuffer;

class BuffersManager
{
public:
							BuffersManager();
							~BuffersManager();

	bool					Init();
	void					ShutDown();

	GpuBuffer*				Create(BufferType type, BufferUsage usage, uint32 bindFlags, uint32 usageFlags, size_t size);
	void					Destroy(GpuBuffer* pBuffer);

private:
	std::vector<GpuBuffer*>	m_buffersPool;

};

#endif //BUFFERS_MANAGER_H
