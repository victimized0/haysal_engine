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

class ConstantBuffer
{
public:
						ConstantBuffer()
							: m_pBuffer(nullptr)
							, m_offset(0)
							, m_size(0)
						{}
						~ConstantBuffer()
						{
							SAFE_RELEASE(m_pBuffer);
						}

	inline void			SetSize(size_t size) { m_size = size; }
	inline void			SetDebugName(const char* name) const
	{
		if (m_pBuffer)
			m_pBuffer->SetDebugName(name);
	}
	inline IGpuBuffer*	GetBuffer() const
	{
		return m_pBuffer->GetBuffer();
	}
	inline IGpuBuffer*	GetBuffer(size_t* outOffset, size_t* outSize) const
	{
		*outOffset	= m_offset;
		*outSize	= m_size;
		return m_pBuffer->GetBuffer();
	}
	bool				Update(const void* pData, size_t size, size_t offset = 0);

protected:
	inline void*		Lock()
	{
		return DeviceFactory::Map(m_pBuffer->GetBuffer(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD);
	}
	inline void			Unlock()
	{
		DeviceFactory::Unmap(m_pBuffer->GetBuffer(), 0);
	}

protected:
	DeviceBuffer*		m_pBuffer;
	size_t				m_offset;
	size_t				m_size;
};

#pragma pack(push, 4)
struct CB_PerDraw
{
	static const int Slot = static_cast<int>(ConstBufferSlot::PerDraw);

	Matrix WorldMatrix;
};

struct CB_PerMaterial
{
	static const int Slot = static_cast<int>(ConstBufferSlot::PerMaterial);

};

struct CB_PerPass
{
	static const int Slot = static_cast<int>(ConstBufferSlot::PerPass);

};

struct CB_PerFrame
{
	static const int Slot = static_cast<int>(ConstBufferSlot::PerFrame);

	Matrix	ViewMatrix;
	Matrix	InvViewMatrix;
	Matrix	ProjMatrix;
	Matrix	InvProjMatrix;
	Matrix	ViewProjMatrix;
	Matrix	InvViewProjMatrix;
	Vec4	CamPos;
	Vec4	CamUpVec;
	Vec4	CamRightVec;
	Vec4	CamFrontVec;
	Vec4	SunDir;
	Vec4	SkyCol;
};

struct CB_PerShadowPass
{
	static const int Slot = static_cast<int>(ConstBufferSlot::PerPass);

	Vec4 LightPos;
	Vec4 ViewPos;
	Vec4 DSBias;
};
#pragma pack(pop)

#endif //GPU_BUFFER_H
