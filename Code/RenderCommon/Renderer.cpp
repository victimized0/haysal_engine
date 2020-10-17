#include "stdafx.h"
#include "Renderer.h"

Renderer* gRenderer = nullptr;

Renderer::Renderer()
	: m_frameID(0)
	, m_features(0)
	, m_isInit(false)
	, m_enableVSync(false)
	, m_enableShadows(true)
	, m_rasterState(RasterState::Solid)
	, m_clearColor(0.f, 0.f, 0.f, 1.f)
	, m_pDefaultMaterial(nullptr)
#if RENDERER_DX11	// TODO: Implement VK version
	, m_pFactory(nullptr)
	, m_pAdapter(nullptr)
	, m_pOutput(nullptr)
	, m_pSwapChain(nullptr)
#endif
{
	//m_pTextureManager = new CTextureManager;
	//m_pRenderThread = new RenderThread;
	//m_pRenderThread->Start();
}

Renderer::~Renderer()
{

}

void Renderer::Release()
{
	gRenderer = nullptr;
}

void Renderer::FlushRenderList()
{

}

void Renderer::AddRenderItem(const RenderInfo& info)
{

}

int Renderer::GetFrameID()
{
	return m_frameID;
}
