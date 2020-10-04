#ifndef RENDERER_H
#define RENDERER_H
#pragma once

#include "platform.h"
#include <RenderModule\IRenderer.h>
#include <RenderModule\IRenderModule.h>

class Renderer;
extern Renderer* gRenderer;

class Renderer : public IRenderer
{
public:
					Renderer();
	virtual			~Renderer() {}

	WIN_HWND		Init(int width, int height, const SystemInitParams& initParams) override = 0;
	virtual bool	InitRenderer() = 0;

	virtual int		GetFrameID() override;
	virtual void	BeginFrame() override = 0;
	virtual void	FillFrame(float clearColor) override = 0;
	virtual void	EndFrame() override = 0;

	virtual bool	CreateDevice() = 0;
	virtual void	Reset() = 0;

	virtual void	RenderScene() = 0;

	virtual void	FlushRenderList() override;
	virtual void	AddRenderItem(const RenderInfo& info) override;

protected:
	uint32						m_frameID;
	//TextureManager*			m_pTextureManager;
	std::unique_ptr<IMaterial>	m_pDefaultMaterial;

};

#endif //RENDERER_H
