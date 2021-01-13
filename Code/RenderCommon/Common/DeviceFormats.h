#ifndef DEVICE_FORMATS_H
#define DEVICE_FORMATS_H
#pragma once

#include <RenderModule\ITexture.h>

namespace DeviceFormats
{
	bool IsTypeless						(DXGIFormat format);
	bool IsDepthStencil					(DXGIFormat format);
	bool IsSRGBReadable					(DXGIFormat format);
	bool IsSRGBFormat					(DXGIFormat format);
	bool IsSRGBFormat					(TextureFormat format);

	DXGIFormat ConvertFromTexFormat		(TextureFormat format);
	uint32 GetWriteMask					(TextureFormat format);
	uint32 GetWriteMask					(DXGIFormat format);
	TextureFormat ConvertToTexFormat	(DXGIFormat format);

	DXGIFormat ConvertToDepthStencil	(DXGIFormat format);
	DXGIFormat ConvertToStencilOnly		(DXGIFormat format);
	DXGIFormat ConvertToDepthOnly		(DXGIFormat format);
	DXGIFormat ConvertToSRGB			(DXGIFormat format);
	DXGIFormat ConvertToSigned			(DXGIFormat format);
	DXGIFormat ConvertToUnsigned		(DXGIFormat format);
	DXGIFormat ConvertToFloat			(DXGIFormat format);
	DXGIFormat ConvertToTypeless		(DXGIFormat format);

	inline TextureFormat ConvertToDepthStencil(TextureFormat format) { return ConvertToTexFormat(ConvertToDepthStencil  (ConvertFromTexFormat(format))); }
	inline TextureFormat ConvertToStencilOnly (TextureFormat format) { return ConvertToTexFormat(ConvertToStencilOnly   (ConvertFromTexFormat(format))); }
	inline TextureFormat ConvertToDepthOnly   (TextureFormat format) { return ConvertToTexFormat(ConvertToDepthOnly     (ConvertFromTexFormat(format))); }
	inline TextureFormat ConvertToSRGB        (TextureFormat format) { return ConvertToTexFormat(ConvertToSRGB          (ConvertFromTexFormat(format))); }
	inline TextureFormat ConvertToSigned      (TextureFormat format) { return ConvertToTexFormat(ConvertToSigned        (ConvertFromTexFormat(format))); }
	inline TextureFormat ConvertToUnsigned    (TextureFormat format) { return ConvertToTexFormat(ConvertToUnsigned      (ConvertFromTexFormat(format))); }
	inline TextureFormat ConvertToFloat       (TextureFormat format) { return ConvertToTexFormat(ConvertToFloat         (ConvertFromTexFormat(format))); }
	inline TextureFormat ConvertToTypeless    (TextureFormat format) { return ConvertToTexFormat(ConvertToTypeless      (ConvertFromTexFormat(format))); }

	UINT GetStride(DXGIFormat format);
	void GetSurfaceInfo(uint16 w, uint16 h, TextureFormat format, size_t* outBytesCount, size_t* outRowBytes, size_t* outRowsNum);
	size_t BitsPerPixel(TextureFormat format);
}

#endif //DEVICE_FORMATS_H
