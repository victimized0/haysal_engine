#ifndef DIRECTX11_RENDERER_H
#define DIRECTX11_RENDERER_H
#pragma once

#include <RenderModule\IRenderModule.h>
#include <platform.inl>
#include <Renderer.h>

class DX11Renderer;
extern DX11Renderer* g_dx11Renderer;

class DX11Renderer final : public Renderer
{
public:
							DX11Renderer();
	virtual					~DX11Renderer();

	virtual WIN_HWND		GetHWND() final { return m_hWnd; }
	bool					CreateMainWindow(int width, int height);
	void					DestroyMainWindow();

	virtual WIN_HWND		Init(int width, int height, const SystemInitParams& initParams) final;
	virtual bool			CreateDevice() final;
	virtual void			CreateSwapChain() final;

	virtual void			Reset() final;
	virtual void			Release() final;
	virtual void			ShutDown() final;
	virtual void			RenderScene(IRenderView* renderView) final;

	//void					InitResources(int flags) final;
	//void					FreeResources(int flags) final;

	virtual void			PushProfileMarker(const char* label) final;
	virtual void			PopProfileMarker(const char* label) final;

public:
	const GpuDevice*		GetDevice()			const	{ return m_pDevice.Get(); }
	GpuDevice*				GetDevice()					{ return m_pDevice.Get(); }
	const GpuContext*		GetDeviceContext()	const	{ return m_pContext.Get(); }
	GpuContext*				GetDeviceContext()			{ return m_pContext.Get(); }

private:
	HWND					m_hWnd;
	std::wstring			m_wndCaption;

	ComPtr<GpuDevice>		m_pDevice;
	ComPtr<GpuContext>		m_pContext;

	uint32					m_creationFlags;
	D3D_FEATURE_LEVEL		m_featureLevel;
	DXGI_ADAPTER_DESC1		m_adapterDesc;

	ComPtr<ID3DUserDefinedAnnotation>	m_pProfileAnnotations;
};

#endif //DIRECTX11_RENDERER_H
