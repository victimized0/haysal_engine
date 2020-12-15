#include "StdAfx.h"
#include "RenderResources.h"

int RenderResources::s_Width            = 0;
int RenderResources::s_Height           = 0;
int RenderResources::s_SizeShadowMap    = 2048;
int RenderResources::m_rtvsCount        = 0;
int RenderResources::m_dsvsCount        = 0;

Texture* RenderResources::s_pTexBackBuffer      = nullptr;
Texture* RenderResources::s_pTexShadowMap       = nullptr;
Texture* RenderResources::s_pTexDiffuseAcc      = nullptr;
Texture* RenderResources::s_pTexSpecularAcc     = nullptr;
Texture* RenderResources::s_pTexHdrTarget       = nullptr;
Texture* RenderResources::s_pTexHdrTargetDiv2   = nullptr;
Texture* RenderResources::s_pTexHdrTargetDiv4   = nullptr;
Texture* RenderResources::s_pTexSceneNormal     = nullptr;
Texture* RenderResources::s_pTexSceneDepth      = nullptr;

void RenderResources::Init()
{
    assert(s_Width > 0);
    assert(s_Height > 0);

    s_pTexShadowMap         = Texture::GetOrCreateTexture( "$ShadowMap",        s_SizeShadowMap,    s_SizeShadowMap,        1, 0, TextureType::Tex2D, TextureFormat::Unknown );
    s_pTexDiffuseAcc        = Texture::GetOrCreateTexture( "$SceneDiffuseAcc",  s_Width,            s_Height,               1, 0, TextureType::Tex2D, TextureFormat::Unknown );
    s_pTexSpecularAcc       = Texture::GetOrCreateTexture( "$SceneSpecularAcc", s_Width,            s_Height,               1, 0, TextureType::Tex2D, TextureFormat::Unknown );
                                                                                                                                                                                     
    s_pTexHdrTarget         = Texture::GetOrCreateTexture( "$SceneHdr",         s_Width,            s_Height,               1, 0, TextureType::Tex2D, TextureFormat::Unknown );
    s_pTexHdrTargetDiv2     = Texture::GetOrCreateTexture( "$SceneHdr/2",       (s_Width + 1) / 2,  (s_Height + 1) / 2,     1, 0, TextureType::Tex2D, TextureFormat::Unknown );
    s_pTexHdrTargetDiv4     = Texture::GetOrCreateTexture( "$SceneHdr/4",       (s_Width + 1) / 4,  (s_Height + 1) / 4,     1, 0, TextureType::Tex2D, TextureFormat::Unknown );
                                                                                                                                                                                     
    s_pTexSceneNormal       = Texture::GetOrCreateTexture( "$SceneNormal",      s_Width,            s_Height,               1, 0, TextureType::Tex2D, TextureFormat::Unknown );
    s_pTexSceneDepth        = Texture::GetOrCreateTexture( "$SceneDepth",       s_Width,            s_Height,               1, 0, TextureType::Tex2D, TextureFormat::Unknown );
}

void RenderResources::Release()
{
    //SAFE_RELEASE(s_pTexBackBuffer);
    SAFE_RELEASE(s_pTexShadowMap);
    SAFE_RELEASE(s_pTexDiffuseAcc);
    SAFE_RELEASE(s_pTexSpecularAcc);
    SAFE_RELEASE(s_pTexHdrTarget);
    SAFE_RELEASE(s_pTexHdrTargetDiv2);
    SAFE_RELEASE(s_pTexHdrTargetDiv4);
    SAFE_RELEASE(s_pTexSceneNormal);
    SAFE_RELEASE(s_pTexSceneDepth);
}

Texture* RenderResources::CreateDepthTarget(int width, int height, const Color& clear, TextureFormat format)
{
    char name[64];
    sprintf_s(name, "$DepthTarget%d", m_dsvsCount++);
    return Texture::GetOrCreateDepthStencil(name, width, height, 1, TextureFlags::NoMips, TextureType::Tex2D, format == TextureFormat::Unknown ? GetDepthFormat() : format);
}

Texture* RenderResources::CreateRenderTarget(int width, int height, const Color& clear, TextureFormat format)
{
    char name[64];
    sprintf_s(name, "$RenderTarget%d", m_rtvsCount++);
    return Texture::GetOrCreateRenderTarget(name, width, height, 1, TextureFlags::NoMips, TextureType::Tex2D, format);
}

TextureFormat RenderResources::GetDepthFormat()
{
    return gRenderer->GetDepthBpp() == 32 ? TextureFormat::D32FS8 : TextureFormat::D24S8;
}
