#ifndef GPU_BUFFER_H
#define GPU_BUFFER_H
#pragma once

enum class BufferUsage : uint8
{
	Immutable = 0,
	Static,
	Dynamic,

	Count
};

enum class BufferType : uint8
{
	Vertex = 0,
	Index,
	Constant,

	Count
};

class GpuBuffer : NoCopy
{
public:
					GpuBuffer(DeviceBuffer* pDevBuf = nullptr);
	virtual			~GpuBuffer();

	void			Create(uint32 elementsCount, uint32 elementSize, DXGI_FORMAT elementFormat, uint32 flags, const void* pData);
	void			Release();

	void			UpdateContent(const void* pData, uint32 size);
	void*			Lock();
	void			Unlock(uint32 size);

	bool			IsNullBuffer()		const { return m_elementCount == 0; }
	bool			IsAvailable()		const { return m_pDeviceBuffer != nullptr;  }
	DeviceBuffer*	GetDevBuffer()		const { return m_pDeviceBuffer; }
	uint32			GetFlags()			const { return m_flags; }
	uint32			GetElementCount()	const { return m_elementCount; }

	inline void		SetDebugName(const char* name) const { m_pDeviceBuffer->SetDebugName(name); }
					
	inline BufferLayout GetLayout() const
	{
		const BufferLayout layout =
		{
			m_format,
			m_elementCount,
			static_cast<uint16>(m_elementSize),
			m_flags,
		};

		return layout;
	}

private:
	DeviceBuffer*	AllocateDeviceBuffer(const void* pInitialData) const;
	void            ReleaseDeviceBuffer(DeviceBuffer*& pDeviceBuffer) const;

private:
	DeviceBuffer*	m_pDeviceBuffer;
	uint32			m_elementCount;
	uint32			m_elementSize;
	uint32			m_flags;
	D3D11_MAP		m_mapMode;
	DXGI_FORMAT		m_format;
	bool            m_isLocked;
};

#endif //GPU_BUFFER_H
