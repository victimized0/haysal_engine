#ifndef RENDER_RESOURCES_H
#define RENDER_RESOURCES_H
#pragma once

#include <RenderModule\IRenderer.h>

enum class TextureFormat : uint16;
class Texture;

class RenderResources
{
public:
							RenderResources()	= default;
							~RenderResources()	= default;

	static void				Init();
	static void				Release();

	static Texture*			CreateDepthTarget(int width, int height, const Color& clear, TextureFormat format);
	static Texture*			CreateRenderTarget(int width, int height, const Color& clear, TextureFormat format);

	static TextureFormat	GetDepthFormat();

public:
	static Texture*			s_pTexShadowMap;
	static Texture*			s_pTexDiffuseAcc;
	static Texture*			s_pTexSpecularAcc;
	static Texture*			s_pTexSceneDepth;
	static Texture*			s_pTexSceneNormal;
	static Texture*			s_pTexHdrTarget;
	static Texture*			s_pTexHdrTargetDiv2;
	static Texture*			s_pTexHdrTargetDiv4	;
	//static Texture*			s_pTexBackBuffer;

	static int				s_Width;
	static int				s_Height;

	static int				s_SizeShadowMap;

private:
	static int				m_rtvsCount;
	static int				m_dsvsCount;

};

#endif //RENDER_RESOURCES_H
