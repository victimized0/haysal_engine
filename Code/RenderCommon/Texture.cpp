#include "StdAfx.h"
#include "Texture.h"

Texture::Texture(uint32 flags, Color clearCol /*= Colors::Black*/, DeviceTexture* pDevTex /*= nullptr*/)
    : m_flags(flags)
    , m_clearCol(clearCol)
    , m_wasUnloaded(false)
{
    if (pDevTex)
        SetDeviceTexture(pDevTex);
}

Texture::~Texture()
{
    ReleaseDeviceTexture();
}

const uint32 Texture::GetDeviceDataSize() const
{
    return 0;
}

const uint32 Texture::GetDataSize() const
{
    return 0;
}

const bool Texture::IsTextureLoaded() const
{
    return false;
}

uint8* Texture::GetData32(int side /*= 0*/, int level /*= 0*/, uint8* pDst /*= nullptr*/, TextureFormat dstFmt /*= TextureFormat::R8G8B8A8*/)
{
    return nullptr;
}

bool Texture::SetFilter(int filterId)
{
    return false;
}

void Texture::SetClamp(bool enable)
{
}

bool Texture::Clear()
{
    return false;
}

bool Texture::Clear(const Color& color)
{
    return false;
}

void Texture::UpdateData(TextureData*&& data, int flags)
{
}

Texture* Texture::GetById(int id)
{
    return nullptr;
}

Texture* Texture::GetByName(const char* name, uint32 flags)
{
    return nullptr;
}

Texture* Texture::ForName(const char* name, uint32 flags, TextureFormat format)
{
    std::tuple<bool, Texture*> texTuple = FindOrCreateTexture(name, flags);
    bool isNewTex = std::get<0>(texTuple);
    Texture* pTex = std::get<1>(texTuple);

    if (!isNewTex)
    {
        pTex->ReleaseDeviceTexture();
        pTex->Reload();
        return pTex;
    }

    pTex->m_srcFileName = name;

    bool isNormal = pTex->m_name.find("_normal") > 0;
    if (isNormal)
        flags |= TextureFlags::HasAlpha;

    pTex->m_texLayout.Format    = format;
    pTex->m_flags               = flags;
    pTex->m_wasUnloaded         = true;
    pTex->LoadFromImage(name, format);

    return pTex;
}

void Texture::LoadFromImage(const char* filename, TextureFormat format)
{
    m_wasUnloaded = false;

    const std::string& filepath = gEnv->pSystem->GetAbsPath(filename);
    const std::wstring& wfilename = ToWString(filepath.c_str());
    //uint32 imageFlags = ((m_flags & TextureFlags::IsAlpha) ? FIM_ALPHA : 0)
    m_flags |= TextureFlags::FromImage;

    TextureData texData;
    DDSLoader::Load(wfilename.c_str(), &texData);
    texData.FilePath = filepath.c_str();

    CreateShaderResource(std::move(texData));
}

void Texture::Reload()
{
}

void Texture::Refresh()
{
}

void Texture::CreateRenderTarget(TextureFormat format, const Color& clearCol)
{
    if (m_texLayout.Format == TextureFormat::Unknown)
        m_texLayout.Format = format;
    if (m_texLayout.Format == TextureFormat::Unknown)
        return;

    m_texLayout.Flags       |= TextureFlags::RenderTarget;
    m_texLayout.ClearColor  = clearCol;
    m_texLayout.MipsCount   = 1;

    TextureData pData = {};
    CreateDeviceTexture(std::move(pData));
}

void Texture::CreateDepthStencil(TextureFormat format, const Color& clearCol)
{
    if (m_texLayout.Format == TextureFormat::Unknown)
        m_texLayout.Format = format;
    if (m_texLayout.Format == TextureFormat::Unknown)
        return;

    m_texLayout.Flags       |= TextureFlags::DepthStencil;
    m_texLayout.ClearColor  = clearCol;
    m_texLayout.MipsCount   = 1;

    TextureData pData = {};
    CreateDeviceTexture(std::move(pData));
}

void Texture::CreateShaderResource(const TextureData& texData)
{
    m_texLayout.Width       = texData.Width;
    m_texLayout.Height      = texData.Height;
    m_texLayout.Depth       = texData.Depth;
    m_texLayout.Format      = texData.Format;
    m_texLayout.MipsCount   = texData.MipsCount;
    m_texLayout.ArraySize   = texData.SlicesCount;
    m_texLayout.IsSRGB      = DeviceFormats::IsSRGBFormat(texData.Format);
    m_texLayout.Type        =
        (texData.Depth > 1)         ? TextureType::Tex3D :
        (texData.SlicesCount == 6)  ? TextureType::TexCube :
        !(texData.SlicesCount % 6)  ? TextureType::TexCubeArray :
        (texData.SlicesCount == 1)  ? TextureType::Tex2D :
                                      TextureType::Tex2DArray;

    assert(m_texLayout.Width && m_texLayout.Height && m_texLayout.MipsCount);
    CreateDeviceTexture(texData);
}

std::tuple<bool, Texture*> Texture::FindOrCreateTexture(const char* name, uint32 flags)
{
    Texture* pTex;
    std::tuple<bool, Texture*> res;

    BaseResource* pRes = BaseResource::Get(name, false);
    if (pRes == nullptr)
    {
        pTex = new Texture(flags);
        pTex->Register(name);
        res = std::make_tuple(true, pTex);
    }
    else
    {
        pTex = dynamic_cast<Texture*>(pRes);
        pTex->AddRef();
        res = std::make_tuple(false, pTex);
    }

    return res;
}

Texture* Texture::GetOrCreateTexture(const char* name, uint16 width, uint16 height, uint16 depth, uint32 flags, TextureType type, TextureFormat format)
{
    std::tuple<bool, Texture*> texTuple = FindOrCreateTexture(name, flags);
    bool isNewTex = std::get<0>(texTuple);
    Texture* pTex = std::get<1>(texTuple);

    if (isNewTex)
    {
        pTex->m_name                = name;
        pTex->m_texLayout.Width     = width;
        pTex->m_texLayout.Height    = height;
        pTex->m_texLayout.Depth     = depth;
        pTex->m_texLayout.Type      = type;
        pTex->m_texLayout.Format    = format;
    }
    else
    {
        if (pTex->GetWidth() == 0)  pTex->m_texLayout.Width  = width;
        if (pTex->GetHeight() == 0) pTex->m_texLayout.Height = height;

        pTex->m_flags |= flags & (TextureFlags::RenderTarget | TextureFlags::DepthStencil);
    }

    pTex->m_flags |= flags;
    return pTex;
}

Texture* Texture::GetOrCreateRenderTarget(const char* name, uint16 width, uint16 height, uint16 depth, uint32 flags, TextureType type, TextureFormat format)
{
    Texture* pTex = GetOrCreateTexture(name, width, height, depth, flags | TextureFlags::RenderTarget, type, format);
    pTex->m_flags |= flags;
    pTex->CreateRenderTarget(format, Colors::White);
    return pTex;
}

Texture* Texture::GetOrCreateDepthStencil(const char* name, uint16 width, uint16 height, uint16 depth, uint32 flags, TextureType type, TextureFormat format)
{
    Texture* pTex = GetOrCreateTexture(name, width, height, depth, flags | TextureFlags::DepthStencil, type, format);
    pTex->m_flags |= flags;
    pTex->CreateDepthStencil(format, Colors::White);
    return pTex;
}

void Texture::SetDeviceTexture(DeviceTexture* pDevTex)
{
    assert(pDevTex);
    if (m_pDeviceTexture == pDevTex || pDevTex == nullptr)
        return;

    if (m_pDeviceTexture != nullptr)
        m_pDeviceTexture->Release();
    
    m_pDeviceTexture = pDevTex;
}

void Texture::ReleaseDeviceTexture()
{
    if (m_pDeviceTexture == nullptr)
        return;

    m_pDeviceTexture->Release();
}

void Texture::CreateDeviceTexture(const TextureData& texData)
{
    if (m_texLayout.Type == TextureType::Tex3D)
        m_texLayout.IsSRGB = false;

    DeviceTexture* pDeviceTex;
    if (!(pDeviceTex = DeviceTexture::Create(m_texLayout, texData)))
        return;

    SetDeviceTexture(pDeviceTex);
}
