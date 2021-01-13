#ifndef RENDERER_H
#define RENDERER_H
#pragma once

#include "platform.h"
#include "SwapChain.h"
#include "TextureManager.h"
#include "BuffersManager.h"
#include "ShaderManager.h"
#include "RenderResources.h"

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
	virtual							~Renderer();

	WIN_HWND						Init(int width, int height, const SystemInitParams& initParams) override;
	void							PostInit() override;
	virtual void					SetHWND(WIN_HWND hWnd) override = 0;
	virtual WIN_HWND				GetHWND() override = 0;

	virtual bool					CreateDevice() = 0;
	virtual void					CreateSwapChain() = 0;

	virtual ShaderManager*			GetShaderMan()	const	{ return m_pShaderManager.get(); }
	virtual BufferManager*			GetBufMan()		const	{ return m_pBufferManager.get(); }
	virtual TextureManager*			GetTexMan()		const	{ return m_pTextureManager.get(); }

	virtual void					Reset() = 0;
	virtual void					Release() override;
	virtual void					ShutDown() override;

	virtual int						GetFrameID() override;
	virtual void					BeginFrame() override;
	virtual void					EndFrame() override;
	virtual void					RenderScene(IRenderView* renderView) override;

	virtual void					FlushRenderList() override;

	inline bool						IsHDR() { return HasFeature(FEATURE_HDR); }
	inline bool						IsEditorMode() const { return m_isEditor; }

	virtual int						GetHeight() const override { return RenderResources::s_Height; }
	virtual int						GetWidth() const override { return RenderResources::s_Width; }
	virtual IDXGISurface*			GetBackBuffer(GpuResource** pRes) const override;

	const MSAA&						GetMSAA() const { return m_MSAAData; }
	int								GetDepthBpp() const { return 32; }

	virtual void					InitSysResources() override;
	virtual void					FreeSysResources(int flags) override;

	virtual IRenderView*			AllocateRenderView() const final;
	virtual IRenderMesh*			CreateRenderMesh(const char* srcName, RenderMeshType bufferType = RenderMeshType::Static) override;

protected:
	void							SetFeature(uint64 feature) { m_features |= feature; }
	void							ClearFeature(uint64 feature) { m_features &= ~feature; }
	bool							HasFeature(uint64 feature) const { return m_features & feature; }
	void							OnResolutionChanged(int w, int h);

protected:
	bool							m_isEditor;
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
	std::unique_ptr<BufferManager>	m_pBufferManager;
	std::unique_ptr<ShaderManager>	m_pShaderManager;

#if RENDERER_DX11	// TODO: Implement Vulkan
	ComPtr<DXGIFactory>				m_pFactory;
	ComPtr<DXGIAdapter>				m_pAdapter;
#endif

};

#endif //RENDERER_H
