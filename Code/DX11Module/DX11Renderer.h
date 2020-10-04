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

	static WIN_HWND	CreateWindowCallback();

	WIN_HWND		GetHWND() final;
	bool			CreateMainWindow(int width, int height);
	void			DestroyMainWindow();

	int				GetHeight() const final;
	int				GetWidth() const final;

	WIN_HWND		Init(int width, int height, const SystemInitParams& initParams) final;
	void			PostInit() final;
	bool			InitRenderer() final;
	bool			CreateDevice() final;
	void			Reset() final;
	void			Release() final;
	void			ShutDown() final;

	//void			InitResources(int flags) final;
	//void			FreeResources(int flags) final;

	void			PushProfileMarker(const char* label) final;
	void			PopProfileMarker(const char* label) final;

	void			BeginFrame() final;
	void			FillFrame(float clearColor) final;
	void			EndFrame() final;
	void			RenderScene() final;

private:
	WIN_HWND		m_hWnd;
	std::wstring	m_wndCaption;


};

#endif //DIRECTX11_RENDERER_H
