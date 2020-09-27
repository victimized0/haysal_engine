#ifndef DIRECTX11_RENDERER_H
#define DIRECTX11_RENDERER_H
#pragma once

#include <RenderModule\IRenderModule.h>
#include <platform.inl>
#include <Renderer.h>

class DX11Renderer final : public Renderer
{
public:
	virtual			~DX11Renderer();

	static PHWND	CreateWindowCallback();
};

#endif //DIRECTX11_RENDERER_H
