#include "stdafx.h"
#include "Renderer.h"

Renderer* gRenderer = nullptr;

extern "C" DLL_EXPORT IRenderer* CreateRenderInterface(ISystem *pSystem)
{
	//std::unique_ptr<Renderer> pRenderer;
	//gcpRendD3D->InitRenderer();
	return gRenderer;
}

class RenderModule : public IRenderModule
{
public:
	~RenderModule()
	{
		SAFE_RELEASE(gEnv->pRenderer);
	}

	const char* GetName() const { return "RenderModule"; }

	bool Initialize(Environment& env, const SystemInitParams& initParams)
	{
		ISystem* pSystem = env.pSystem;
		env.pRenderer = CreateRenderInterface(pSystem);
		return env.pRenderer != 0;
	}
};
