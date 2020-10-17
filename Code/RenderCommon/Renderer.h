#ifndef RENDERER_H
#define RENDERER_H
#pragma once

#include "platform.h"
#include <WorldModule\IWorldEngine.h>
#include <RenderModule\IRenderer.h>

class Renderer;
extern Renderer* gRenderer;

class Renderer : public IRenderer
{
public:
								Renderer();
	virtual						~Renderer();

	WIN_HWND					Init(int width, int height, const SystemInitParams& initParams) override = 0;
	virtual WIN_HWND			GetHWND() override = 0;

	virtual bool				CreateDevice() = 0;
	virtual void				CreateSwapChain() = 0;
	virtual void				Reset() = 0;
	virtual void				Release() override;

	virtual int					GetFrameID() override;
	virtual void				BeginFrame() override = 0;
	virtual void				FillFrame(float clearColor) override = 0;
	virtual void				EndFrame() override = 0;

	virtual void				RenderScene() = 0;
	virtual void				FlushRenderList() override;
	virtual void				AddRenderItem(const RenderInfo& info) override;

	inline bool					IsHDR() { return HasFeature(FEATURE_HDR); }

protected:
	void						SetFeature(uint64 feature) { m_features |= feature; }
	void						ClearFeature(uint64 feature) { m_features &= ~feature; }
	bool						HasFeature(uint64 feature) const { return m_features & feature; }
	virtual void				CreateOutput() = 0;

protected:
	bool						m_isInit;
	bool						m_enableVSync;
	bool						m_enableShadows;
	RasterState					m_rasterState;

	Color						m_clearColor;

	uint64						m_features;
	uint32						m_frameID;
	//TextureManager*			m_pTextureManager;
	std::unique_ptr<IMaterial>	m_pDefaultMaterial;

#if RENDERER_DX11	// TODO: Implement Vulkan
	ComPtr<DXGIFactory>			m_pFactory;
	ComPtr<DXGIAdapter>			m_pAdapter;
	ComPtr<DXGIOutput>			m_pOutput;
	ComPtr<DXGISwapChain>		m_pSwapChain;
#endif

};

#endif //RENDERER_H
