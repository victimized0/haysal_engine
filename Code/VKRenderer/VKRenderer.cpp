#include "stdafx.h"
#include "VKRenderer.h"

VKRenderer* g_vkRenderer = nullptr;

VKRenderer::VKRenderer()
{

}

VKRenderer::~VKRenderer()
{

}

void VKRenderer::PostInit()
{
}

void VKRenderer::Release()
{
}

void VKRenderer::ShutDown()
{
}

int VKRenderer::GetHeight() const
{
	return 0;
}

int VKRenderer::GetWidth() const
{
	return 0;
}

void VKRenderer::PushProfileMarker(const char* label)
{
}

void VKRenderer::PopProfileMarker(const char* label)
{
}

WIN_HWND VKRenderer::GetHWND()
{
	return WIN_HWND();
}

WIN_HWND VKRenderer::Init(int width, int height, const SystemInitParams& initParams)
{
	return WIN_HWND();
}

bool VKRenderer::InitRenderer()
{
	return false;
}

void VKRenderer::BeginFrame()
{
}

void VKRenderer::FillFrame(float clearColor)
{
}

void VKRenderer::EndFrame()
{
}

bool VKRenderer::CreateDevice()
{
	return false;
}

void VKRenderer::Reset()
{
}

void VKRenderer::RenderScene()
{
}

class VKRenderModule : public IRenderModule
{
public:
	virtual ~VKRenderModule()
	{
		SAFE_RELEASE(gEnv->pRenderer);
	}

	const char* GetName() const { return "VKRenderModule"; }

	bool Initialize(Environment& env, const SystemInitParams& initParams)
	{
		ISystem* pSystem = env.pSystem;
		env.pRenderer = g_vkRenderer;
		return env.pRenderer != nullptr;
	}
};

extern "C" DLL_EXPORT IEngineModule* CreateModule(ISystem* pSystem)
{
	g_vkRenderer = new VKRenderer();
	auto pModule = new VKRenderModule();
	return pModule;
}
