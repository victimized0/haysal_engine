#ifndef RENDERER_H
#define RENDERER_H
#pragma once

#include <RenderModule\IRenderer.h>
#include <RenderModule\IRenderModule.h>

class Renderer : public IRenderer
{
					Renderer() {}
	virtual			~Renderer() {}

	PHWND			Init(int x, int y, int width, int height, SystemInitParams& initParams) override = 0;
	virtual bool	InitRenderer() = 0;

	void			BeginFrame() override = 0;
	void			FillFrame(float clearColor) override = 0;
	void			EndFrame() override = 0;

	virtual void	ShutDown(uint32 nFlags) = 0;
	virtual bool	CreateDevice() = 0;
	virtual void	Reset() = 0;

	virtual void	RenderScene() = 0;
};

extern Renderer* gRenderer;

#endif //RENDERER_H
