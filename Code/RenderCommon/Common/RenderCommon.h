#ifndef RENDER_COMMON_H
#define RENDER_COMMON_H
#pragma once

#include "InputLayout.h"
#include "SamplerState.h"
#include "DeviceFormats.h"

enum class ResourceViewType : uint8
{
	Default			= 0,
	Alternative		= 1,
	RasterTarget	= 2,

	Linear			= Default,
	sRGB			= Alternative,

	DepthOnly		= Default,
	StencilOnly		= Alternative,
	RenderTarget	= RasterTarget,
	DepthStencil	= RasterTarget,
};

enum class PrimitiveTopology : uint8
{
	Undefined		= D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED,
	PointList		= D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
	LineList		= D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
	LineStrip		= D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
	TriangleList	= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	TriangleStrip	= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
};

enum class ConstBufferSlot : uint8
{
	PerDraw		= 0,
	PerMaterial	= 1,
	PerPass		= 2,
	PerFrame	= 3,

	AACount
};

enum class ShaderClass : uint8
{
	Vertex		= 0,
	Pixel		= 1,
	Geometry	= 2,
	Compute		= 3,

	AACount
};

enum class ShaderStage : uint8
{
	Vertex		= BIT8(static_cast<uint8>(ShaderClass::Vertex)),
	Pixel		= BIT8(static_cast<uint8>(ShaderClass::Pixel)),
	Geometry	= BIT8(static_cast<uint8>(ShaderClass::Geometry)),
	Compute		= BIT8(static_cast<uint8>(ShaderClass::Compute)),

	AACount		= static_cast<uint8>(ShaderClass::AACount)
};

struct ShaderBlob
{
	void* m_pData;
	int   m_size;
};

struct ResourceView
{
	enum Type
	{
		SRV	= 0,
		RTV	= 1,
		DSV	= 2,
		UAV	= 3,

		Total
	};

	enum Flags
	{
		SRV_DepthOnly	= 0,
		SRV_StencilOnly	= 1,

		DSV_ReadWrite	= 0,
		DSV_ReadOnly	= 1,

		UAV_WriteOnly	= 0,
		UAV_ReadWrite	= 1,
	};

	union RVDesc
	{
		// Texture View configuration
		struct
		{
			uint64 ViewType				: 3;
			uint64 Format				: 7;
			uint64 Flags				: 2;
			uint64 IsSrgbRead			: 1;
			uint64 IsMultisampled		: 1;
			uint64 FirstSliceNum		: 11;
			uint64 SlicesCount			: 11;
			uint64 MostDetailedMipNum	: 4;
			uint64 MipsCount			: 4;
			uint64 Unused				: 20;
		};

		//// Buffer View configuration
		//struct
		//{
		//	uint64 CommonHeader			: 12;
		//	uint64 IsRaw				: 1;
		//	uint64 OffsetBitsCount		: 5;
		//	uint64 OffsetAndSize		: 46;
		//};

		uint64 Key;
	};

	ResourceView(uint64 key = 0) { Desc.Key = key; }

	static ResourceView		ShaderResourceView		(DXGIFormat format, int firstElementNum = 0, int elementsCount = -1, int mostDetailedMipNum = 0, int mipsCount = -1, bool isSrgbRead = false, bool isMultisampled = false, uint32 flags = 0);
	static ResourceView		RenderTargetView		(DXGIFormat format, int firstElementNum = 0, int elementsCount = -1, int mipLevel = 0, bool isMultisampled = false);
	static ResourceView		DepthStencilView		(DXGIFormat format, int firstElementNum = 0, int elementsCount = -1, int mipLevel = 0, bool isMultisampled = false, uint32 flags = 0);
	static ResourceView		UnorderedAccessView		(DXGIFormat format, int firstElementNum = 0, int elementsCount = -1, int mipLevel = 0, uint32 flags = 0);

	//static ResourceView		RenderTargetRawView		(DXGIFormat format, int firstElementNum = 0, int elementsCount = -1);
	//static ResourceView		ShaderResourceRawView	(DXGIFormat format, int firstElementNum = 0, int elementsCount = -1, uint32 flags = 0);
	//static ResourceView		DepthStencilRawView		(DXGIFormat format, int firstElementNum = 0, int elementsCount = -1, uint32 flags = 0);
	//static ResourceView		UnorderedAccessRawView	(DXGIFormat format, int firstElementNum = 0, int elementsCount = -1, uint32 flags = 0);

	static bool				IsMultisampled			(uint64 key)			{ return ResourceView(key).Desc.IsMultisampled; }
	inline bool				IsShaderResourceView	(uint64 key)	const	{ return ResourceView(key).Desc.ViewType == Type::SRV; }
	inline bool				IsRenderTargetView		(uint64 key)	const	{ return ResourceView(key).Desc.ViewType == Type::RTV; }
	inline bool				IsDepthStencilView		(uint64 key)	const	{ return ResourceView(key).Desc.ViewType == Type::DSV; }
	inline bool				IsUnorderedAccessView	(uint64 key)	const	{ return ResourceView(key).Desc.ViewType == Type::UAV; }

	bool					operator ==(const ResourceView& other)	const	{ return Desc.Key == other.Desc.Key; }
	bool					operator !=(const ResourceView& other)	const	{ return Desc.Key != other.Desc.Key; }

	RVDesc					Desc;
};

class BaseResource : NoCopy
{
public:
							BaseResource()
								: m_refCount(1)
								, m_id(0)
							{}

	virtual					~BaseResource() {}

	inline int				GetId() const { return m_id; }
	inline int				GetRefCount() const { return m_refCount; }
	inline const char*		GetName() const { return m_name.c_str(); }
	virtual int				AddRef() { return ++m_refCount; }
	virtual int				Release();

	virtual bool			IsValid();
	virtual void			Register(const std::string& name);
	virtual void			Unregister();

public:
	static BaseResource*	Get(int id, bool addRef);
	static BaseResource*	Get(const std::string& name, bool addRef);
	static void				ShutDown();

private:
	int						m_refCount;
	int						m_id;
	std::string				m_name;

private:
	static std::map<std::string, BaseResource*> s_resourcesMap;
	static std::vector<BaseResource*>			s_resources;
	static std::vector<int>						s_freeIndices;

};

#endif //RENDER_COMMON_H
