#include "StdAfx.h"
#include "RenderResources.h"

int RenderResources::s_Width = 0;
int RenderResources::s_Height = 0;

Texture* RenderResources::CreateDepthTarget(int width, int height, const Color& clear, TextureFormat format)
{
    return nullptr;
}

Texture* RenderResources::CreateRenderTarget(int width, int height, const Color& clear, TextureFormat format)
{
    return nullptr;
}
