#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H
#pragma once

#include <RenderModule\IRenderModule.h>
#include <platform.inl>
#include <Renderer.h>

class VKRenderer;
extern VKRenderer* g_vkRenderer;

class VKRenderer final : public Renderer
{
public:
	VKRenderer();
	virtual			~VKRenderer();

	void			PostInit() final;
	void			Release() final;
	void			ShutDown() final;
	int				GetHeight() const final;
	int				GetWidth() const final;
	void			PushProfileMarker(const char* label) final;
	void			PopProfileMarker(const char* label) final;
	WIN_HWND		GetHWND() final;
	WIN_HWND		Init(int width, int height, const SystemInitParams& initParams) final;
	void			BeginFrame() final;
	void			FillFrame(float clearColor) final;
	void			EndFrame() final;
	bool			CreateDevice() final;
	void			CreateSwapChain() final;
	void			Reset() final;
	void			RenderScene() final;

protected:
	void			CreateOutput() final;

};


#endif //VULKAN_RENDERER_H
