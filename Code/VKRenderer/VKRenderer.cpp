#include "stdafx.h"
#include "VKRenderer.h"

VKRenderer* g_vkRenderer = nullptr;

VKRenderer::VKRenderer()
{

}

VKRenderer::~VKRenderer()
{

}

void VKRenderer::Release()
{
}

void VKRenderer::ShutDown()
{
}

void VKRenderer::PushProfileMarker(const char* label)
{
}

void VKRenderer::PopProfileMarker(const char* label)
{
}

WIN_HWND VKRenderer::GetHWND()
{
	return nullptr;
}

WIN_HWND VKRenderer::Init(int width, int height, const SystemInitParams& initParams)
{
	return nullptr;
}

bool VKRenderer::CreateDevice()
{
	return false;
}

void VKRenderer::CreateSwapChain()
{
}

void VKRenderer::Reset()
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
