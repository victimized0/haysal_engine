#include "stdafx.h"
#include "Renderer.h"

Renderer* gRenderer = nullptr;

Renderer::Renderer()
	: m_frameID(0)
	, m_features(0)
	, m_isInit(false)
	, m_isSysResInit(false)
	, m_enableVSync(false)
	, m_enableShadows(true)
	, m_rasterState(RasterState::Solid)
	, m_clearColor(0.f, 0.f, 0.f, 1.f)
	, m_pDefaultMaterial(nullptr)
#if RENDERER_DX11	// TODO: Implement VK version
	, m_pFactory(nullptr)
	, m_pAdapter(nullptr)
#endif
{
	m_pTextureManager	= std::make_unique<TextureManager>();
	m_pShaderManager	= std::make_unique<ShaderManager>();
	m_pBufferManager	= std::make_unique<BuffersManager>();
	//m_pRenderThread = new RenderThread;
	//m_pRenderThread->Start();
}

Renderer::~Renderer()
{
	
}

void Renderer::PostInit()
{
	// TODO: Called after other systems that renderer depends on were initialised
	//m_cEF.mfPostInit(); Init shader system

	//if (gEnv->pCryFont)
	//	m_pDefaultFont = gEnv->pCryFont->GetFont("default");

	// load all default textures
	if (m_pTextureManager)
		m_pTextureManager->PreloadDefaultTextures();

	InitSysResources();

	m_pBufferManager->Init();
}

void Renderer::Release()
{
	gRenderer = nullptr;
}

void Renderer::ShutDown()
{
	if (m_pTextureManager)
		m_pTextureManager->ReleaseDefaultTextures();
	m_pTextureManager.release();

	BaseResource::ShutDown();
}

void Renderer::FlushRenderList()
{

}

void Renderer::OnResolutionChanged(int w, int h)
{
	RenderResources::s_Width = w;
	RenderResources::s_Height = h;
	m_swapChain.OnResolutionChanged(w, h);
}

int Renderer::GetFrameID()
{
	return m_frameID;
}

void Renderer::BeginFrame()
{
}

void Renderer::EndFrame()
{
}

void Renderer::RenderScene(IRenderView* renderView)
{
	PushProfileMarker("SCENE");

	renderView->ExecuteRenderPass(RenderListId::ShadowGen);
	renderView->ExecuteRenderPass(RenderListId::ZPrePass);
	renderView->ExecuteRenderPass(RenderListId::LightPass);
	renderView->ExecuteRenderPass(RenderListId::Opaque);
	renderView->ExecuteRenderPass(RenderListId::Transparent);
	renderView->ExecuteRenderPass(RenderListId::PostEffects);

	PopProfileMarker("SCENE");
}

IDXGISurface* Renderer::GetBackBuffer() const
{
	IDXGISurface* pBackBuffer = nullptr;
#if RENDERER_DX11
	m_swapChain.Get()->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
#elif RENDERER_VK
	assert(false); // TODO: Not implemented
#endif
	return pBackBuffer;
}

void Renderer::InitSysResources()
{
	if (m_isSysResInit == false)
	{
		m_pShaderManager->PreloadBinaryShaders();
		m_pShaderManager->LoadSystemShaders();

		//CreateRenderResources();
		//PrecacheDefaultShaders();

		m_isSysResInit = true;
	}
}

void Renderer::FreeSysResources(int flags)
{
}

std::unique_ptr<IRenderView>&& Renderer::AllocateRenderView() const
{
	return std::move(std::make_unique<RenderView>());
}
