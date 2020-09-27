#include "stdafx.h"
#include "Renderer.h"

Renderer* gRenderer = nullptr;

class RenderModule : public IRenderModule
{
public:
	virtual ~RenderModule()
	{
		SAFE_RELEASE(gEnv->pRenderer);
	}

	const char* GetName() const { return "RenderModule"; }

	bool Initialize(Environment& env, const SystemInitParams& initParams)
	{
		ISystem* pSystem = env.pSystem;
		env.pRenderer = gRenderer;// CreateRenderInterface(pSystem);
		return env.pRenderer != 0;
	}
};

extern "C" DLL_EXPORT IEngineModule * CreateRenderInterface(ISystem * pSystem)
{
	IEngineModule* pModule = new RenderModule;
	//gcpRendD3D->InitRenderer();
	return pModule;
}
