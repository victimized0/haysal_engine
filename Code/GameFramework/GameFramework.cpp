#include "stdafx.h"
#include "GameFramework.h"

#include <platform.inl>
#include <GameFramework\IGameModule.h>
#include <InputModule\IInputSystem.h>

GameFramework::GameFramework()
{
}

GameFramework::~GameFramework()
{
}

bool GameFramework::Init()
{
    return true;
}

void GameFramework::Release()
{
}

void GameFramework::Update(float dt)
{
	Camera camera = gEnv->pSystem->GetViewCamera();

	auto mouse = gEnv->pInputSystem->GetIMouse()->GetState();
	if (mouse.positionMode == MouseMode::MODE_RELATIVE) {
		Vec3 delta = Vec3(float(mouse.x), float(mouse.y), 0) * dt;

		float dx = -delta.x * 1.5f;
		float dy = -delta.y * 0.2f;
		camera.Rotate(dx, dy);
	}

	auto kb = gEnv->pInputSystem->GetIKeyboard()->GetState();
	if (kb.W)
		camera.Zoom(10.0f * dt);
	if (kb.S)
		camera.Zoom(-10.0f * dt);

	gEnv->pSystem->SetViewCamera(camera);
}

class GameModule final : public IGameModule
{
public:
	virtual			~GameModule() { SAFE_RELEASE(gEnv->pGameFramework); }
	const char*		GetName() const final { return "GameModule"; }
	bool			Initialize(Environment& env, const SystemInitParams& initParams) final
	{
		env.pGameFramework = new GameFramework();
		return env.pGameFramework != nullptr;
	}
};

extern "C" DLL_EXPORT IEngineModule* CreateModule(ISystem * pSystem)
{
	auto pModule = new GameModule();
	// TODO: Subscribe to system events
	return pModule;
}
