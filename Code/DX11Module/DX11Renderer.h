#ifndef DIRECTX11_RENDERER_H
#define DIRECTX11_RENDERER_H
#pragma once

#include <RenderModule\IRenderModule.h>
#include <platform.inl>
#include <Renderer.h>

class DX11Renderer;
extern DX11Renderer* g_d3d11Renderer;

class DX11Renderer final : public Renderer
{
public:
					DX11Renderer();
	virtual			~DX11Renderer();

	static PHWND	CreateWindowCallback();

	PHWND			GetHWND() final;
	bool			CreateMainWindow(int width, int height);
	//bool			SetWindowIcon(const char* path) final;

	int				GetHeight() const final;
	int				GetWidth() const final;
	float			GetPixelAspectRatio() const final;

	PHWND			Init(int width, int height, SystemInitParams& initParams) final;
	void			PostInit() final;
	bool			InitRenderer() final;
	//int				GetFeatures() final;
	bool			CreateDevice() final;
	void			Reset() final;
	void			Release() final;
	void			ShutDown() final;
	void			ShutDown(uint32 nFlags) final;

	//void			InitSystemResources(int nFlags) final;
	//void			FreeSystemResources(int nFlags) final;

	void			PushProfileMarker(const char* label) final;
	void			PopProfileMarker(const char* label) final;

	void			BeginFrame() final;
	void			FillFrame(float clearColor) final;
	void			EndFrame() final;
	void			RenderScene() final;

private:
	PHWND			m_hWnd;
	std::wstring	m_wndCaption;
};

#endif //DIRECTX11_RENDERER_H
