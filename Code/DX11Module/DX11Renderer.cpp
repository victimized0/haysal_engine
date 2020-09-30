#include "stdafx.h"
#include "DX11Renderer.h"

DX11Renderer* g_d3d11Renderer = nullptr;

DX11Renderer::DX11Renderer()
{

}

DX11Renderer::~DX11Renderer()
{

}

PHWND DX11Renderer::CreateWindowCallback()
{
	return 0;
}

void DX11Renderer::PostInit()
{
}

int DX11Renderer::GetFeatures()
{
	return 0;
}

void DX11Renderer::Release()
{
}

void DX11Renderer::ShutDown()
{
}

void DX11Renderer::InitSystemResources(int nFlags)
{
}

void DX11Renderer::FreeSystemResources(int nFlags)
{
}

int DX11Renderer::GetHeight() const
{
	return 0;
}

int DX11Renderer::GetWidth() const
{
	return 0;
}

float DX11Renderer::GetPixelAspectRatio() const
{
	return 0.0f;
}

void DX11Renderer::PushProfileMarker(const char* label)
{
}

void DX11Renderer::PopProfileMarker(const char* label)
{
}

PHWND DX11Renderer::GetHWND()
{
	return PHWND();
}

bool DX11Renderer::SetWindowIcon(const char* path)
{
	return false;
}

PHWND DX11Renderer::Init(int x, int y, int width, int height, SystemInitParams& initParams)
{
	return PHWND();
}

bool DX11Renderer::InitRenderer()
{
	return false;
}

void DX11Renderer::BeginFrame()
{
}

void DX11Renderer::FillFrame(float clearColor)
{
}

void DX11Renderer::EndFrame()
{
}

void DX11Renderer::ShutDown(uint32 nFlags)
{
}

bool DX11Renderer::CreateDevice()
{
	return false;
}

void DX11Renderer::Reset()
{
}

void DX11Renderer::RenderScene()
{
}

class DX11RenderModule : public IRenderModule
{
public:
	virtual ~DX11RenderModule()
	{
		SAFE_RELEASE(gEnv->pRenderer);
	}

	const char* GetName() const { return "DX11RenderModule"; }

	bool Initialize(Environment& env, const SystemInitParams& initParams)
	{
		ISystem* pSystem = env.pSystem;
		env.pRenderer = g_d3d11Renderer;
		return env.pRenderer != nullptr;
	}
};

extern "C" DLL_EXPORT IEngineModule* CreateModule(ISystem* pSystem)
{
	g_d3d11Renderer = new DX11Renderer();
	auto pModule = std::make_unique<DX11RenderModule>();
	return pModule.get();
}
