#ifndef RENDER_COMMON_H
#define RENDER_COMMON_H
#pragma once

#include "InputLayout.h"
#include "SamplerState.h"
#include "DeviceFormats.h"

struct ShaderBlob
{
	void* m_pData;
	int   m_size;
};

struct ResourceView
{
	enum Type
	{
		ShaderResourceView	= 0,
		RenderTargetView	= 1,
		DepthStencilView	= 2,
		UnorderedAccessView	= 3,

		Total
	};

	static ResourceView RenderTarget(DXGIFormat format);
	static ResourceView ShaderResource(DXGIFormat format, int flags = 0);
	static ResourceView DepthStencil(DXGIFormat format, int flags = 0);
	static ResourceView UnorderedAccess(DXGIFormat format, int flags = 0);

	inline bool			IsShaderResourceView()	{ return Type == Type::ShaderResourceView; }
	inline bool			IsRenderTargetView()	{ return Type == Type::RenderTargetView; }
	inline bool			IsDepthStencilView()	{ return Type == Type::DepthStencilView; }
	inline bool			IsUnorderedAccessView()	{ return Type == Type::UnorderedAccessView; }

	uint64				Type;
	uint64				Flags;
	uint64				Format;
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
	virtual void			Release();

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
