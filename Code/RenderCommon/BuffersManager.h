#ifndef BUFFERS_MANAGER_H
#define BUFFERS_MANAGER_H
#pragma once

class GpuBuffer;
class ConstantBuffer;

class BufferManager
{
public:
							BufferManager();
							~BufferManager();

	bool					Init();
	void					ShutDown();

	ConstantBuffer*         CreateConstBuffer(size_t size);
	GpuBuffer*				Create(BufferType type, BufferUsage usage, uint32 bindFlags, uint32 usageFlags, size_t size);
	void					Destroy(GpuBuffer* pBuffer);

	void					Read(GpuBuffer* pBuffer, void* pOutData, size_t size, size_t offset);
	void					Write(GpuBuffer* pBuffer, void* pInData, size_t size, size_t offset);

	template<typename var_size_t>
	static inline var_size_t AlignBufferSize(var_size_t size)
	{
		const var_size_t alignment = 16;
		return (size + (alignment - 1)) & ~(alignment - 1);
	}

	template<typename var_size_t>
	static inline var_size_t AlignElementCount(var_size_t numElements, var_size_t sizeOfElement)
	{
		const var_size_t missing = AlignBufferSize(sizeOfElement * numElements) - (sizeOfElement * numElements);
		if (missing)
			numElements += (missing + (sizeOfElement - 1)) / sizeOfElement;
		return numElements;
	}

private:
	std::vector<GpuBuffer*>	m_buffersPool;

};

#endif //BUFFERS_MANAGER_H
