#include "stdafx.h"
#include "Renderer.h"

#include <RenderModule\IRenderModule.h>

Renderer* gRenderer = nullptr;

extern "C" DLL_EXPORT IRenderer * CreateRenderInterface(ISystem * pSystem)
{
	//std::unique_ptr<Renderer> pRenderer;
	//gcpRendD3D->InitRenderer();
	return gRenderer;
}

class RendererModule : public IRenderModule
{
public:
	virtual ~RendererModule()
	{
		SAFE_RELEASE(gEnv->pRenderer);
	}

	const char* GetName() const final { return "RendererModule"; }

	bool Initialize(Environment& env, const SystemInitParams& initParams) final
	{
		ISystem* pSystem = env.pSystem;
		env.pRenderer = CreateRenderInterface(pSystem);
		return env.pRenderer != 0;
	}
};
