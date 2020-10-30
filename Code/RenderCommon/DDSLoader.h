#ifndef DDS_LOADER_H
#define DDS_LOADER_H
#pragma once

const uint32 DDS_MAGIC = 0x20534444; // "DDS "

#define DDS_FOURCC      0x00000004 // DDPF_FOURCC
#define DDS_RGB         0x00000040 // DDPF_RGB
#define DDS_LUMINANCE   0x00020000 // DDPF_LUMINANCE
#define DDS_ALPHA       0x00000002 // DDPF_ALPHA
#define DDS_BUMPDUDV    0x00080000 // DDPF_BUMPDUDV

#define DDS_HEIGHT      0x00000002 // DDSD_HEIGHT
#define DDS_WIDTH       0x00000004 // DDSD_WIDTH
#define DDS_DEPTH       0x00800000 // DDSD_DEPTH
#define DDS_CUBEMAP     0x00000200 // DDSCAPS2_CUBEMAP

#define DDS_CUBEMAP_POSITIVEX 0x00000600 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
#define DDS_CUBEMAP_NEGATIVEX 0x00000a00 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
#define DDS_CUBEMAP_POSITIVEY 0x00001200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
#define DDS_CUBEMAP_NEGATIVEY 0x00002200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
#define DDS_CUBEMAP_POSITIVEZ 0x00004200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
#define DDS_CUBEMAP_NEGATIVEZ 0x00008200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ

#define DDS_CUBEMAP_ALLFACES ( DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX |  \
                               DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY |  \
                               DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ )

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                                          \
                ((uint32)(uint8)(ch0) | ((uint32)(uint8)(ch1) << 8) |           \
                ((uint32)(uint8)(ch2) << 16) | ((uint32)(uint8)(ch3) << 24 ))
#endif /* defined(MAKEFOURCC) */

enum DDS_ALPHA_MODE
{
    DDS_ALPHA_MODE_UNKNOWN          = 0,
    DDS_ALPHA_MODE_STRAIGHT         = 1,
    DDS_ALPHA_MODE_PREMULTIPLIED    = 2,
    DDS_ALPHA_MODE_OPAQUE           = 3,
    DDS_ALPHA_MODE_CUSTOM           = 4,
};

struct DDS_PIXELFORMAT
{
    uint32  Size;
    uint32  Flags;
    uint32  FourCC;
    uint32  RGBBitCount;
    uint32  RBitMask;
    uint32  GBitMask;
    uint32  BBitMask;
    uint32  ABitMask;
};

struct DDS_HEADER
{
    uint32          Size;
    uint32          Flags;
    uint32          Height;
    uint32          Width;
    uint32          PitchOrLinearSize;
    uint32          Depth; // only if DDS_HEADER_FLAGS_VOLUME is set in flags
    uint32          MipsCount;
    uint32          Reserved1[11];
    DDS_PIXELFORMAT DDSpf;
    uint32          Caps;
    uint32          Caps2;
    uint32          Caps3;
    uint32          Caps4;
    uint32          Reserved2;
};

struct DDS_HEADER_DXT10
{
    DXGIFormat  Format;
    uint32      Dimension;
    uint32      MiscFlags; // see D3D11_RESOURCE_MISC_FLAG
    uint32      ArraySize;
    uint32      MiscFlags2;
};

class DDSLoader
{
public:
				DDSLoader();
				~DDSLoader();

	static HRESULT Load(const wchar_t* filename, struct TextureData* outData);

private:
    static DXGIFormat GetDXGIFormat(const DDS_PIXELFORMAT& ddpf);

};

#endif //DDS_LOADER_H
