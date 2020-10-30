#ifndef RENDERER_H
#define RENDERER_H
#pragma once

#include "platform.h"
#include "SwapChain.h"
#include "TextureManager.h"

#include <WorldModule\IWorldEngine.h>
#include <RenderModule\IRenderer.h>

class Renderer;
extern Renderer* gRenderer;

struct MSAA
{
	MSAA()
		: Type(0)
		, Quality(0)
		, m_pZTexture(nullptr)
	{}

	void Clear()
	{
		Type		= 0;
		Quality		= 0;
		m_pZTexture	= nullptr;
	}

	uint32		Type;
	uint32		Quality;
	Texture*	m_pZTexture;
};

class Renderer : public IRenderer
{
public:
								Renderer();
	virtual						~Renderer();

	WIN_HWND					Init(int width, int height, const SystemInitParams& initParams) override = 0;
	void						PostInit() override;
	virtual WIN_HWND			GetHWND() override = 0;

	virtual bool				CreateDevice() = 0;
	virtual void				CreateSwapChain() = 0;
	virtual void				Reset() = 0;
	virtual void				Release() override;
	virtual void				ShutDown() override;

	virtual int					GetFrameID() override;
	virtual void				BeginFrame() override = 0;
	virtual void				FillFrame(float clearColor) override = 0;
	virtual void				EndFrame() override = 0;

	virtual void				RenderScene() = 0;
	virtual void				FlushRenderList() override;
	virtual void				AddRenderItem(const RenderInfo& info) override;

	inline bool					IsHDR() { return HasFeature(FEATURE_HDR); }

	virtual int					GetHeight() const override { return RenderResources::s_Width; }
	virtual int					GetWidth() const override { return RenderResources::s_Height; }
	virtual IDXGISurface*		GetBackBuffer() const override;

	const MSAA&					GetMSAA() const { return m_MSAAData; }

	virtual void				InitSysResources() override;
	virtual void				FreeSysResources(int flags) override;

protected:
	void						SetFeature(uint64 feature) { m_features |= feature; }
	void						ClearFeature(uint64 feature) { m_features &= ~feature; }
	bool						HasFeature(uint64 feature) const { return m_features & feature; }
	void						OnResolutionChanged(int w, int h);

protected:
	bool							m_isSysResInit;
	bool							m_isInit;
	bool							m_enableVSync;
	bool							m_enableShadows;
	RasterState						m_rasterState;
	RenderViewport					m_viewport;
	SwapChain						m_swapChain;

	Color							m_clearColor;
	MSAA							m_MSAAData;

	uint64							m_features;
	uint32							m_frameID;
	std::unique_ptr<IMaterial>		m_pDefaultMaterial;
	std::unique_ptr<TextureManager>	m_pTextureManager;

#if RENDERER_DX11	// TODO: Implement Vulkan
	ComPtr<DXGIFactory>				m_pFactory;
	ComPtr<DXGIAdapter>				m_pAdapter;
#endif

};

#endif //RENDERER_H
