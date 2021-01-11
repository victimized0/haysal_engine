#ifndef INTERFACE_TEXTURE_H
#define INTERFACE_TEXTURE_H
#pragma once

class Texture;

enum class TextureType : uint8
{
	Unknown			= 0,
	Tex1D			= 1,
	Tex2D			= 2,
	Tex2DArray		= 3,
	Tex3D			= 4,
	TexCube			= 5,
	TexCubeArray	= 6,

	Total
};

enum class TextureFormat : uint16
{
	Unknown = 0,

	R8,
	R8S,
	R16,
	R16S,
	R16F,
	R32F,
	R8G8,
	R8G8S,
	R16G16,
	R16G16S,
	R16G16F,
	R32G32F,
	R8G8B8A8,
	R8G8B8A8S,
	R16G16B16A16,
	R16G16B16A16S,
	R16G16B16A16F,
	R32G32B32A32F,

	BC1,
	BC2,
	BC3,
	BC4U,
	BC4S,
	BC5U,
	BC5S,
	BC6UH,
	BC6SH,
	BC7,
	CTX1,
	A8,

	// Depth/Stencil
	S8,
	D16,
	D16S8,
	D24,
	D24S8,
	D32F,
	D32FS8,

	B8G8R8,
	B8G8R8X8,
	B8G8R8A8,

	Total
};

enum TextureFlags : uint16
{
	ShaderResource	= BIT16(0),
	NoMips			= BIT16(1),
	NormalMap		= BIT16(2),
	DepthStencil	= BIT16(3),
	AllowSRGB		= BIT16(4),
	HasAlpha		= BIT16(5),
	IsAlpha			= BIT16(6),
	UseMSAA			= BIT16(7),
	UseMips			= BIT16(8),
	RenderTarget	= BIT16(9),
	UnorderedAccess	= BIT16(10),
	UAVOverlap		= BIT16(11),
	UAVTextureRW	= BIT16(12),
	FromImage		= BIT16(13)
};

struct TextureData : NoMove
{
	const uint8*	pData[6];
	uint32			Flags;
	uint16			Width;
	uint16			Height;
	uint16			Depth;
	uint8			SlicesCount;
	uint8			MipsCount;
	TextureFormat	Format;
	const char*		FilePath;

	TextureData()
		: Width(0)
		, Height(0)
		, Depth(0)
		, Format(TextureFormat::Unknown)
		, Flags(0)
		, MipsCount(0)
		, SlicesCount(0)
		, FilePath(nullptr)
		, m_rellocMask(0)
	{
		pData[0] = pData[1] = pData[2] = pData[3] = pData[4] = pData[5] = nullptr;
	}
	~TextureData() {
		for (int i = 0; i < 6; i++)
			if (WasReallocated(i))
				SAFE_DELETE_ARRAY(pData[i]);
	}

	bool IsOwned()
	{
		for (int i = 0; i < 6; i++)
			if (pData[i] && !WasReallocated(i))
				return false;
		return true;
	}
	void AssignData(unsigned int i, const uint8* pNewData)
	{
		assert(i < 6);
		if (WasReallocated(i))
			SAFE_DELETE_ARRAY(pData[i]);
		pData[i] = pNewData;
		SetReallocated(i);
	}
	inline bool WasReallocated(unsigned int i) const
	{
		return (m_rellocMask & (1 << i)) != 0;
	}
	inline void SetReallocated(unsigned int i)
	{
		m_rellocMask |= (1 << i);
	}

private:
	uint8 m_rellocMask;

};

struct ITexture
{
	virtual						~ITexture() = default;

	virtual const char*			GetName() const = 0;
	virtual const int			GetWidth() const = 0;
	virtual const int			GetHeight() const = 0;
	virtual const int			GetDepth() const = 0;
	virtual const int			GetTextureID() const = 0;
	virtual const int8			GetNumMips() const = 0;
	virtual const uint32		GetFlags() const = 0;
	virtual const uint32		GetDeviceDataSize() const = 0;
	virtual const uint32		GetDataSize() const = 0;
	virtual const TextureType	GetTextureType() const = 0;
	virtual const bool			IsTextureLoaded() const = 0;
	virtual uint8*				GetData32(int side = 0, int level = 0, uint8* pDst = nullptr, TextureFormat dstFmt = TextureFormat::R8G8B8A8) = 0;
	virtual bool				SetFilter(int filterId) = 0;
	virtual void				SetClamp(bool enable) = 0;
	virtual bool				Clear() = 0;
	virtual bool				Clear(const Color& color) = 0;
	virtual const TextureFormat	GetFormat() const = 0;
	virtual void				UpdateData(TextureData*&& data, int flags) = 0;

	virtual struct TextureLayout		GetTextureLayout() const = 0;

};

#endif //INTERFACE_TEXTURE_H
