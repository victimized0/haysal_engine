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
	float			GetPixelAspectRatio() const final;
	void			PushProfileMarker(const char* label) final;
	void			PopProfileMarker(const char* label) final;
	PHWND			GetHWND() final;
	PHWND			Init(int width, int height, SystemInitParams& initParams) final;
	bool			InitRenderer() final;
	void			BeginFrame() final;
	void			FillFrame(float clearColor) final;
	void			EndFrame() final;
	void			ShutDown(uint32 nFlags) final;
	bool			CreateDevice() final;
	void			Reset() final;
	void			RenderScene() final;
};


#endif //VULKAN_RENDERER_H
