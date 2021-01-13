#ifndef TEXTURE_H
#define TEXTURE_H
#pragma once

#include <RenderModule\ITexture.h>
#include "DDSLoader.h"

class Texture : public ITexture, public BaseResource
{
public:
										Texture(uint32 flags, Color clearCol = Colors::Black, DeviceTexture* pDevTex = nullptr);
	virtual								~Texture();

	// BaseResource
	virtual int							AddRef() final { return BaseResource::AddRef(); }
	virtual int							Release() final { return BaseResource::Release(); }
	// ~BaseResource

	// Static methods to create a Texture instance
	static Texture*						GetOrCreateTexture(const char* name, uint16 width, uint16 height, uint16 depth, uint32 flags, TextureType type, TextureFormat format);
	static Texture*						GetOrCreateRenderTarget(const char* name, uint16 width, uint16 height, uint16 depth, uint32 flags, TextureType type, TextureFormat format);
	static Texture*						GetOrCreateDepthStencil(const char* name, uint16 width, uint16 height, uint16 depth, uint32 flags, TextureType type, TextureFormat format);

	// ITexture
	virtual inline const char*			GetName() const final			{ return m_name.c_str(); }
	virtual inline const int			GetTextureID() const final		{ return GetId(); }
	virtual inline const uint32			GetFlags() const final			{ return m_flags; }
	virtual inline const int			GetWidth() const final			{ return m_texLayout.Width; }
	virtual inline const int			GetHeight() const final			{ return m_texLayout.Height; }
	virtual inline const int			GetDepth() const final			{ return m_texLayout.Depth; }
	virtual inline const int8			GetNumMips() const final		{ return m_texLayout.MipsCount; }
	virtual inline const TextureType	GetTextureType() const final	{ return m_texLayout.Type; }
	virtual const TextureFormat			GetFormat() const final			{ return m_texLayout.Format; }
	virtual inline TextureLayout		GetTextureLayout() const final  { return m_texLayout; }
	virtual inline const uint32			GetDeviceDataSize() const final;
	virtual inline const uint32			GetDataSize() const final;
	virtual const bool					IsTextureLoaded() const final;
	virtual uint8*						GetData32(int side = 0, int level = 0, uint8* pDst = nullptr, TextureFormat dstFmt = TextureFormat::R8G8B8A8) final;
	virtual bool						SetFilter(int filterId) final;
	virtual void						SetClamp(bool enable) final;
	virtual bool						Clear() final;
	virtual bool						Clear(const Color& color) final;
	virtual void						UpdateData(TextureData*&& data, int flags) final;
	// ~ITexture

	const bool							IsDynamic() const { return ((m_flags & (TextureFlags::RenderTarget | TextureFlags::DepthStencil | TextureFlags::UnorderedAccess)) != 0); }

	static Texture*						GetById(int id);
	static Texture*						GetByName(const char* name, uint32 flags = 0);
	static Texture*						ForName(const char* name, uint32 flags, TextureFormat format);

	void								LoadFromImage(const char* filepath, TextureFormat format = TextureFormat::Unknown);
	void								Reload();
	void								Refresh();

	DeviceTexture*						GetDeviceTexture() const { return m_pDeviceTexture; }
	void								SetDeviceTexture(DeviceTexture* pDevTex);

private:
	void								CreateRenderTarget(TextureFormat format, const Color& clearCol);
	void								CreateDepthStencil(TextureFormat format, const Color& clearCol);
	void								CreateShaderResource(const TextureData& texData);

	static std::tuple<bool, Texture*>	FindOrCreateTexture(const char* name, uint32 flags);
	void								CreateDeviceTexture(const TextureData& texData);
	void								ReleaseDeviceTexture();

private:
	DeviceTexture*						m_pDeviceTexture;
	TextureLayout						m_texLayout;
	Color								m_clearCol;
	uint32								m_flags;
	std::string							m_name;
	std::string							m_srcFileName;
	bool								m_wasUnloaded;

};

#endif //TEXTURE_H
