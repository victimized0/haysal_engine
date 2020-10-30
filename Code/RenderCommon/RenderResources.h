#ifndef RENDER_RESOURCES_H
#define RENDER_RESOURCES_H
#pragma once

#include <RenderModule\IRenderer.h>

class Texture;

class RenderResources
{
public:
	static int			s_Width;
	static int			s_Height;

private:
	static Texture*		CreateDepthTarget(int width, int height, const Color& clear, TextureFormat format);
	static Texture*		CreateRenderTarget(int width, int height, const Color& clear, TextureFormat format);

};

#endif //RENDER_RESOURCES_H
