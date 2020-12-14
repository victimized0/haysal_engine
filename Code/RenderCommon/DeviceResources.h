#ifndef DEVICE_RESOURCES_H
#define DEVICE_RESOURCES_H
#pragma once

struct ResourceDimension
{
	uint32 Width;
	uint32 Height;
	uint32 Depth;
	uint32 Subresources;
};

struct ResourceLayout
{
	uint32	m_bytesCount;
	uint32	m_elementsCount;
	uint32  m_flags;
};

enum ResourceFlags : uint32
{
	BIND_DEPTH_STENCIL		= BIT32(0),
	BIND_RENDER_TARGET		= BIT32(1),
	BIND_UNORDERED_ACCESS	= BIT32(2),
	BIND_VERTEX_BUFFER		= BIT32(3),
	BIND_INDEX_BUFFER		= BIT32(4),
	BIND_CONST_BUFFER		= BIT32(5),
	BIND_SHADER_RESOURCE	= BIT32(6),

	USAGE_UAV_READWRITE		= BIT32(7),
	USAGE_UAV_COUNTER		= BIT32(8),
	USAGE_AUTOGENMIPS		= BIT32(9),
	USAGE_STRUCTURED		= BIT32(10),
	USAGE_INDIRECTARGS		= BIT32(11),
	USAGE_RAW				= BIT32(12),

	// CPU_READ | CPU_WRITE -> D3D HEAP
	//    no    |    no     -> DEFAULT
	//    no    |    yes    -> DYNAMIC
	//    yes   | yes or no -> STAGING
	USAGE_CPU_READ			= BIT32(13),
	USAGE_CPU_WRITE			= BIT32(14)
};

struct TextureLayout
{
	Color			ClearColor;
	uint32			Flags;
	uint16			Width;
	uint16			Height;
	uint16			Depth;
	uint16			ArraySize;
	int8			MipsCount;

	TextureType		Type;	// uint8
	TextureFormat	Format;	// uint8
	bool			IsSRGB;

	TextureLayout()
		: Width(0)
		, Flags(0)
		, Height(0)
		, Depth(1)
		, ArraySize(1)
		, MipsCount(1)
		, Type(TextureType::Unknown)
		, Format(TextureFormat::Unknown)
		, IsSRGB(false)
	{}
};

struct BufferLayout
{
	DXGI_FORMAT	Format;
	uint32		ElementCount;
	uint16      ElementSize;
	uint32      Flags;
};

class DeviceResource
{
public:
	DeviceResource()
		: m_pRawResource(nullptr)
		, m_rawFormat(DXGI_FORMAT_UNKNOWN)
		, m_flags(0)
		, m_textureType(TextureType::Unknown)
		, m_isFilterable(false)
		, m_isSRGB(false)
		, m_allowSRGB(false)
		, m_isMSAA(false)
	{}

	virtual				~DeviceResource() {}

	ResourceLayout		GetLayout() const;

	inline void			SetDebugName(const char* name) const
	{
		m_pRawResource->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(strlen(name) + 1), name);
	}
	inline GpuResource* GetRawResource() const
	{
		return m_pRawResource.Get();
	}
	inline DXGIFormat	GetRawFormat() const
	{
		return m_rawFormat;
	}
	inline uint32		GetFlags() const { return m_flags; }
	inline bool			IsWritable() const
	{
		return m_flags & (ResourceFlags::BIND_DEPTH_STENCIL | ResourceFlags::BIND_RENDER_TARGET | ResourceFlags::USAGE_UAV_READWRITE);
	}
	inline bool			IsResolvable() const
	{
		return m_flags & ResourceFlags::BIND_DEPTH_STENCIL;
	}
	inline bool			IsRenderTarget() const
	{
		return m_flags & ResourceFlags::BIND_RENDER_TARGET;
	}
	inline bool			IsShaderResource() const
	{
		return m_flags & ResourceFlags::BIND_SHADER_RESOURCE;
	}
	inline bool			IsUnorderedAccess() const
	{
		return m_flags & ResourceFlags::BIND_UNORDERED_ACCESS;
	}
	inline bool			IsDepthStencil() const
	{
		return m_flags & ResourceFlags::BIND_DEPTH_STENCIL;
	}

	void											InitResourceViews();
	GpuView*										CreateResourceView(const ResourceView& desc);
	ResourceViewType								GetOrCreateResourceView(const ResourceView& desc);
	ResourceViewType								ReserveResourceView(const ResourceView& desc);
	void											ReserveResourceViews(int count);
	void											ReleaseResourceViews();

	const std::pair<ResourceView, GpuResource*>		LookupResourceView(const ResourceViewType desc) const;
	std::pair<ResourceView, GpuResource*>			LookupResourceView(const ResourceViewType desc);

	void											SetRawResource(GpuResource* pRes);

	inline GpuSRV*									GetOrCreateSRV(const ResourceView desc)	{ return reinterpret_cast<GpuSRV*>(LookupResourceView(GetOrCreateResourceView(desc)).second); }
	inline GpuRTV*									GetOrCreateRTV(const ResourceView desc)	{ return reinterpret_cast<GpuRTV*>(LookupResourceView(GetOrCreateResourceView(desc)).second); }
	inline GpuUAV*									GetOrCreateUAV(const ResourceView desc)	{ return reinterpret_cast<GpuUAV*>(LookupResourceView(GetOrCreateResourceView(desc)).second); }
	inline GpuDSV*									GetOrCreateDSV(const ResourceView desc)	{ return reinterpret_cast<GpuDSV*>(LookupResourceView(GetOrCreateResourceView(desc)).second); }

	inline GpuSRV*									LookupSRV(const ResourceViewType desc) const { return reinterpret_cast<GpuSRV*>(LookupResourceView(desc).second); }
	inline GpuRTV*									LookupRTV(const ResourceViewType desc) const { return reinterpret_cast<GpuRTV*>(LookupResourceView(desc).second); }
	inline GpuUAV*									LookupUAV(const ResourceViewType desc) const { return reinterpret_cast<GpuUAV*>(LookupResourceView(desc).second); }
	inline GpuDSV*									LookupDSV(const ResourceViewType desc) const { return reinterpret_cast<GpuDSV*>(LookupResourceView(desc).second); }

protected:
	std::vector<std::pair<ResourceView, GpuView*>>	m_resourceViews;
	TextureType										m_textureType;

	uint32											m_flags;
	ComPtr<GpuResource>								m_pRawResource;
	DXGIFormat										m_rawFormat;
	int												m_resourceElements;

	bool											m_isFilterable	: 1;
	bool											m_isSRGB		: 1;
	bool											m_allowSRGB		: 1;
	bool											m_isMSAA		: 1;

};

class DeviceTexture : public DeviceResource
{
public:
							DeviceTexture();
	virtual					~DeviceTexture();

	TextureLayout			GetLayout() const;
	void					Release();

public:
	static DeviceTexture*	Create(const TextureLayout& layout, const TextureData& texData);

private:

};

class DeviceBuffer : public DeviceResource
{
public:
							DeviceBuffer() = default;
	virtual					~DeviceBuffer() = default;

	BufferLayout            GetLayout() const;
	int32					Release();

	inline GpuBaseBuffer*	GetBaseBuffer() const
	{
		return reinterpret_cast<GpuBaseBuffer*>(GetRawResource());
	}
	inline IGpuBuffer*		GetBuffer() const
	{
		return reinterpret_cast<IGpuBuffer*>(GetBaseBuffer());
	}

public:
	static DeviceBuffer*	Create(const BufferLayout& pLayout, const void* pData);
	static DeviceBuffer*	Associate(const BufferLayout& pLayout, GpuResource* pBuf);
};

#endif //DEVICE_RESOURCES_H
