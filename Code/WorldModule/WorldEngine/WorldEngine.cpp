#include "stdafx.h"
#include "WorldEngine.h"

#include <WorldModule\IWorldModule.h>

WorldEngine* g_world = nullptr;

WorldEngine::WorldEngine()
{
}

WorldEngine::~WorldEngine()
{
}

void WorldEngine::Release()
{
}

void WorldEngine::ShutDown()
{
}

class WorldModule final : public IWorldModule
{
public:
	virtual ~WorldModule() { SAFE_RELEASE(gEnv->pWorld); }
	const char* GetName() const final { return "WorldModule"; }
	bool Initialize(Environment& env, const SystemInitParams& initParams) final
	{
		ISystem* pSystem = env.pSystem;
		env.pWorld = g_world;
		return env.pWorld != nullptr;
	}
};

extern "C" DLL_EXPORT IEngineModule* CreateModule(ISystem* pSystem)
{
	g_world = new WorldEngine();
	auto pModule = new WorldModule();
	return pModule;
}
