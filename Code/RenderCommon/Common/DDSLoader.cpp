#include "StdAfx.h"
#include "DDSLoader.h"

namespace
{
    struct handle_closer
    {
        void operator()(HANDLE h)
        {
            if (h) CloseHandle(h);
        }
    };

    typedef public std::unique_ptr<void, handle_closer> ScopedHandle;
    inline HANDLE safe_handle(HANDLE h)
    {
        return (h == INVALID_HANDLE_VALUE) ? 0 : h;
    }
}

DDSLoader::DDSLoader()
{
}

DDSLoader::~DDSLoader()
{
}

HRESULT DDSLoader::Load(const wchar_t* filename, TextureData* outData)
{
	if (!outData) return E_FAIL;

    // open the file
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
    ScopedHandle hFile(safe_handle(CreateFile2(filename,
        GENERIC_READ,
        FILE_SHARE_READ,
        OPEN_EXISTING,
        nullptr)));
#else
    ScopedHandle hFile(safe_handle(CreateFileW(filename,
        GENERIC_READ,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr)));
#endif

    if (!hFile)
        return HRESULT_FROM_WIN32(GetLastError());

    // Get the file size
    FILE_STANDARD_INFO fileInfo;
    if (!GetFileInformationByHandleEx(hFile.get(), FileStandardInfo, &fileInfo, sizeof(fileInfo)))
        return HRESULT_FROM_WIN32(GetLastError());

    // File is too big for 32-bit allocation, so reject read
    if (fileInfo.EndOfFile.HighPart > 0)
        return E_FAIL;

    // Need at least enough data to fill the header and magic number to be a valid DDS
    if (fileInfo.EndOfFile.LowPart < (sizeof(DDS_HEADER) + sizeof(DDS_MAGIC)))
        return E_FAIL;

    // create enough space for the file data
    uint8* ddsData = new uint8[fileInfo.EndOfFile.LowPart];
    if (!ddsData)
        return E_OUTOFMEMORY;

    // read the data in
    DWORD BytesRead = 0;
    if ( !ReadFile(hFile.get(), ddsData, fileInfo.EndOfFile.LowPart, &BytesRead, nullptr) )
        return HRESULT_FROM_WIN32(GetLastError());

    if (BytesRead < fileInfo.EndOfFile.LowPart)
        return E_FAIL;

    // DDS files always start with the same magic number ("DDS ")
    uint32 dwMagicNumber = *reinterpret_cast<const uint32*>(ddsData);
    if (dwMagicNumber != DDS_MAGIC)
        return E_FAIL;

    auto header = reinterpret_cast<const DDS_HEADER*>(ddsData + sizeof(DDS_MAGIC));

    // Verify header to validate DDS file
    if (header->Size != sizeof(DDS_HEADER) || header->DDSpf.Size != sizeof(DDS_PIXELFORMAT))
        return E_FAIL;

    // Check for DX10 extension
    bool isDXT10Header = false;
    if ((header->DDSpf.Flags & DDS_FOURCC) && (MAKEFOURCC('D', 'X', '1', '0') == header->DDSpf.FourCC))
    {
        // Must be long enough for both headers and magic value
        if (fileInfo.EndOfFile.LowPart < (sizeof(DDS_HEADER) + sizeof(DDS_MAGIC) + sizeof(DDS_HEADER_DXT10)))
            return E_FAIL;

        isDXT10Header = true;
    }

    // setup the pointers in the process request
    ptrdiff_t offset    = sizeof(uint32) + sizeof(DDS_HEADER) + (isDXT10Header ? sizeof(DDS_HEADER_DXT10) : 0);
    outData->pData[0]   = ddsData + offset;
    //outData->Size       = fileInfo.EndOfFile.LowPart - offset;

    uint32      flags           = 0;
    uint32      width           = header->Width;
    uint32      height          = header->Height;
    uint32      depth           = header->Depth;
    uint32      dimension       = D3D11_RESOURCE_DIMENSION_UNKNOWN;
    uint32      arraySize       = 1;
    DXGIFormat  format          = DXGI_FORMAT_UNKNOWN;
    bool        isCubeMap       = false;

    size_t mipsCount = header->MipsCount;
    if (mipsCount == 0)
        mipsCount = 1;

    if ((header->DDSpf.Flags & DDS_FOURCC) && (MAKEFOURCC('D', 'X', '1', '0') == header->DDSpf.FourCC))
    {
        auto d3d10ext = reinterpret_cast<const DDS_HEADER_DXT10*>((const char*)header + sizeof(DDS_HEADER));

        arraySize = d3d10ext->ArraySize;
        if (arraySize == 0)
            return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);

        switch (d3d10ext->Format)
        {
        case DXGI_FORMAT_AI44:
        case DXGI_FORMAT_IA44:
        case DXGI_FORMAT_P8:
        case DXGI_FORMAT_A8P8:
            return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
        }

        format = d3d10ext->Format;
        switch (d3d10ext->Dimension)
        {
        case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
            // D3DX writes 1D textures with a fixed Height of 1
            if ((header->Flags & DDS_HEIGHT) && height != 1)
            {
                return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
            }
            height = depth = 1;
            break;

        case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
            if (d3d10ext->MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE)
            {
                arraySize *= 6;
                isCubeMap = true;
            }
            depth = 1;
            break;

        case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
            if (!(header->Flags & DDS_DEPTH))
                return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
            if (arraySize > 1)
                return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
            break;

        default:
            return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
        }

        dimension = d3d10ext->Dimension;
    }
    else
    {
        format = GetDXGIFormat(header->DDSpf);
        if (format == DXGI_FORMAT_UNKNOWN)
            return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
        if (header->Flags & DDS_DEPTH)
            dimension = D3D11_RESOURCE_DIMENSION_TEXTURE3D;
        else
        {
            if (header->Caps2 & DDS_CUBEMAP)
            {
                // We require all six faces to be defined
                if ((header->Caps2 & DDS_CUBEMAP_ALLFACES) != DDS_CUBEMAP_ALLFACES)
                    return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);

                arraySize = 6;
                isCubeMap = true;
            }

            depth = 1;
            dimension = D3D11_RESOURCE_DIMENSION_TEXTURE2D;
            // Note there's no way for a legacy Direct3D 9 DDS to express a '1D' texture
        }

        //assert(BitsPerPixel(format) != 0);
    }

    // Bound sizes (for security purposes we don't trust DDS file metadata larger than the D3D 11.x hardware requirements)
    if (mipsCount > D3D11_REQ_MIP_LEVELS)
        return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);

    switch (dimension)
    {
    case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
        if ((arraySize > D3D11_REQ_TEXTURE1D_ARRAY_AXIS_DIMENSION) || (width > D3D11_REQ_TEXTURE1D_U_DIMENSION))
            return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
        break;

    case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
        if (isCubeMap)
        {
            // This is the right bound because we set arraySize to (NumCubes*6) above
            if ((arraySize > D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION) ||
                (width > D3D11_REQ_TEXTURECUBE_DIMENSION) ||
                (height > D3D11_REQ_TEXTURECUBE_DIMENSION))
            {
                return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
            }
        }
        else if ((arraySize > D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION) ||
            (width > D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION) ||
            (height > D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION))
        {
            return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
        }
        break;

    case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
        if ((arraySize > 1) ||
            (width > D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION) ||
            (height > D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION) ||
            (depth > D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION))
        {
            return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
        }
        break;

    default:
        return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
    }

    outData->Flags      = flags;
    outData->Width      = width;
    outData->Height     = height;
    outData->Depth      = depth;
    outData->SlicesCount = arraySize;
    outData->MipsCount  = mipsCount;
    outData->Format     = DeviceFormats::ConvertToTexFormat(format);

    return S_OK;
}

DXGIFormat DDSLoader::GetDXGIFormat(const DDS_PIXELFORMAT& ddpf)
{
#define ISBITMASK( r,g,b,a ) ( ddpf.RBitMask == r && ddpf.GBitMask == g && ddpf.BBitMask == b && ddpf.ABitMask == a ) //-V1003

    if (ddpf.Flags & DDS_RGB)
    {
        // Note that sRGB formats are written using the "DX10" extended header
        switch (ddpf.RGBBitCount)
        {
        case 32:
            if (ISBITMASK(0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000))
                return DXGI_FORMAT_R8G8B8A8_UNORM;
            if (ISBITMASK(0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000))
                return DXGI_FORMAT_B8G8R8A8_UNORM;
            if (ISBITMASK(0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000))
                return DXGI_FORMAT_B8G8R8X8_UNORM;

            // No DXGI format maps to ISBITMASK(0x000000ff,0x0000ff00,0x00ff0000,0x00000000) aka D3DFMT_X8B8G8R8

            // Note that many common DDS reader/writers (including D3DX) swap the
            // the RED/BLUE masks for 10:10:10:2 formats. We assume
            // below that the 'backwards' header mask is being used since it is most
            // likely written by D3DX. The more robust solution is to use the 'DX10'
            // header extension and specify the DXGI_FORMAT_R10G10B10A2_UNORM format directly

            // For 'correct' writers, this should be 0x000003ff,0x000ffc00,0x3ff00000 for RGB data
            if (ISBITMASK(0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000))
                return DXGI_FORMAT_R10G10B10A2_UNORM;

            // No DXGI format maps to ISBITMASK(0x000003ff,0x000ffc00,0x3ff00000,0xc0000000) aka D3DFMT_A2R10G10B10

            if (ISBITMASK(0x0000ffff, 0xffff0000, 0x00000000, 0x00000000))
                return DXGI_FORMAT_R16G16_UNORM;
            if (ISBITMASK(0xffffffff, 0x00000000, 0x00000000, 0x00000000))
                // Only 32-bit color channel format in D3D9 was R32F
                return DXGI_FORMAT_R32_FLOAT; // D3DX writes this out as a FourCC of 114
            break;

        case 24:
            // No 24bpp DXGI formats aka D3DFMT_R8G8B8
            break;

        case 16:
            if (ISBITMASK(0x7c00, 0x03e0, 0x001f, 0x8000)) //-V525
                return DXGI_FORMAT_B5G5R5A1_UNORM;
            if (ISBITMASK(0xf800, 0x07e0, 0x001f, 0x0000))
                return DXGI_FORMAT_B5G6R5_UNORM;

            // No DXGI format maps to ISBITMASK(0x7c00,0x03e0,0x001f,0x0000) aka D3DFMT_X1R5G5B5
            if (ISBITMASK(0x0f00, 0x00f0, 0x000f, 0xf000))
                return DXGI_FORMAT_B4G4R4A4_UNORM;

            // No DXGI format maps to ISBITMASK(0x0f00,0x00f0,0x000f,0x0000) aka D3DFMT_X4R4G4B4

            // No 3:3:2, 3:3:2:8, or paletted DXGI formats aka D3DFMT_A8R3G3B2, D3DFMT_R3G3B2, D3DFMT_P8, D3DFMT_A8P8, etc.
            break;
        }
    }
    else if (ddpf.Flags & DDS_LUMINANCE)
    {
        if (ddpf.RGBBitCount == 8)
        {
            if (ISBITMASK(0x000000ff, 0x00000000, 0x00000000, 0x00000000))
                return DXGI_FORMAT_R8_UNORM; // D3DX10/11 writes this out as DX10 extension

            // No DXGI format maps to ISBITMASK(0x0f,0x00,0x00,0xf0) aka D3DFMT_A4L4
            if (ISBITMASK(0x000000ff, 0x00000000, 0x00000000, 0x0000ff00))
                return DXGI_FORMAT_R8G8_UNORM; // Some DDS writers assume the bitcount should be 8 instead of 16
        }

        if (16 == ddpf.RGBBitCount)
        {
            if (ISBITMASK(0x0000ffff, 0x00000000, 0x00000000, 0x00000000))
                return DXGI_FORMAT_R16_UNORM; // D3DX10/11 writes this out as DX10 extension
            if (ISBITMASK(0x000000ff, 0x00000000, 0x00000000, 0x0000ff00))
                return DXGI_FORMAT_R8G8_UNORM; // D3DX10/11 writes this out as DX10 extension
        }
    }
    else if (ddpf.Flags & DDS_ALPHA)
    {
        if (ddpf.RGBBitCount == 8)
        {
            return DXGI_FORMAT_A8_UNORM;
        }
    }
    else if (ddpf.Flags & DDS_BUMPDUDV)
    {
        if (ddpf.RGBBitCount == 16)
        {
            if (ISBITMASK(0x00ff, 0xff00, 0x0000, 0x0000))
                return DXGI_FORMAT_R8G8_SNORM; // D3DX10/11 writes this out as DX10 extension
        }

        if (ddpf.RGBBitCount == 32)
        {
            if (ISBITMASK(0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000))
                return DXGI_FORMAT_R8G8B8A8_SNORM; // D3DX10/11 writes this out as DX10 extension
            if (ISBITMASK(0x0000ffff, 0xffff0000, 0x00000000, 0x00000000))
                return DXGI_FORMAT_R16G16_SNORM; // D3DX10/11 writes this out as DX10 extension
            // No DXGI format maps to ISBITMASK(0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000) aka D3DFMT_A2W10V10U10
        }
    }
    else if (ddpf.Flags & DDS_FOURCC)
    {
        if (MAKEFOURCC('D', 'X', 'T', '1') == ddpf.FourCC)
            return DXGI_FORMAT_BC1_UNORM;
        if (MAKEFOURCC('D', 'X', 'T', '3') == ddpf.FourCC)
            return DXGI_FORMAT_BC2_UNORM;
        if (MAKEFOURCC('D', 'X', 'T', '5') == ddpf.FourCC)
            return DXGI_FORMAT_BC3_UNORM;

        // While pre-multiplied alpha isn't directly supported by the DXGI formats,
        // they are basically the same as these BC formats so they can be mapped
        if (MAKEFOURCC('D', 'X', 'T', '2') == ddpf.FourCC)
            return DXGI_FORMAT_BC2_UNORM;
        if (MAKEFOURCC('D', 'X', 'T', '4') == ddpf.FourCC)
            return DXGI_FORMAT_BC3_UNORM;
        if (MAKEFOURCC('A', 'T', 'I', '1') == ddpf.FourCC)
            return DXGI_FORMAT_BC4_UNORM;
        if (MAKEFOURCC('B', 'C', '4', 'U') == ddpf.FourCC)
            return DXGI_FORMAT_BC4_UNORM;
        if (MAKEFOURCC('B', 'C', '4', 'S') == ddpf.FourCC)
            return DXGI_FORMAT_BC4_SNORM;
        if (MAKEFOURCC('A', 'T', 'I', '2') == ddpf.FourCC)
            return DXGI_FORMAT_BC5_UNORM;
        if (MAKEFOURCC('B', 'C', '5', 'U') == ddpf.FourCC)
            return DXGI_FORMAT_BC5_UNORM;
        if (MAKEFOURCC('B', 'C', '5', 'S') == ddpf.FourCC)
            return DXGI_FORMAT_BC5_SNORM;
        // BC6H and BC7 are written using the "DX10" extended header
        if (MAKEFOURCC('R', 'G', 'B', 'G') == ddpf.FourCC)
            return DXGI_FORMAT_R8G8_B8G8_UNORM;
        if (MAKEFOURCC('G', 'R', 'G', 'B') == ddpf.FourCC)
            return DXGI_FORMAT_G8R8_G8B8_UNORM;
        if (MAKEFOURCC('Y', 'U', 'Y', '2') == ddpf.FourCC)
            return DXGI_FORMAT_YUY2;
        // Check for D3DFORMAT enums being set here
        switch (ddpf.FourCC)
        {
        case 36: // D3DFMT_A16B16G16R16
            return DXGI_FORMAT_R16G16B16A16_UNORM;
        case 110: // D3DFMT_Q16W16V16U16
            return DXGI_FORMAT_R16G16B16A16_SNORM;
        case 111: // D3DFMT_R16F
            return DXGI_FORMAT_R16_FLOAT;
        case 112: // D3DFMT_G16R16F
            return DXGI_FORMAT_R16G16_FLOAT;
        case 113: // D3DFMT_A16B16G16R16F
            return DXGI_FORMAT_R16G16B16A16_FLOAT;
        case 114: // D3DFMT_R32F
            return DXGI_FORMAT_R32_FLOAT;
        case 115: // D3DFMT_G32R32F
            return DXGI_FORMAT_R32G32_FLOAT;
        case 116: // D3DFMT_A32B32G32R32F
            return DXGI_FORMAT_R32G32B32A32_FLOAT;
        }
    }
    return DXGI_FORMAT_UNKNOWN;
}
