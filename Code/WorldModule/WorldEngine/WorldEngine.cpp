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

void WorldEngine::OnFrameStart()
{
}

//IWorldObject* WorldEngine::CreateObject(const WorldObjectDescr& descr)
//{
//	auto newObj = std::make_unique<WorldObject>();
//	m_worldObjects.push_back( std::move(newObj) );
//	return m_worldObjects[m_worldObjects.size()-1].get();
//}
//
//bool WorldEngine::DeleteObject(IWorldObject* pObj)
//{
//	auto it = std::find_if(m_worldObjects.begin(), m_worldObjects.end(), [&](std::unique_ptr<IWorldObject>& obj) { return obj.get() == pObj; });
//	if (it != m_worldObjects.end())
//	{
//		m_worldObjects.erase(it);
//		return true;
//	}
//
//	return false;
//}

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
