#include "StdAfx.h"
#include "InputModule.h"

#include <InputModule\IInputModule.h>

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

bool InputSystem::Init()
{
	m_mouse.SetWindow((HWND)gEnv->pSystem->GetHWND());
	m_mouse.SetMode(MouseMode::MODE_RELATIVE);

	return true;
}

void InputSystem::Release()
{
}

void InputSystem::ProcessMessage(InputType type, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (type)
	{
	case InputType::Mouse:
		Mouse::ProcessMessage(message, wParam, lParam);
		break;
	case InputType::Keyboard:
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;
	case InputType::Both:
		Keyboard::ProcessMessage(message, wParam, lParam);
		Mouse::ProcessMessage(message, wParam, lParam);
		break;
	}
}

class InputModule final : public IInputModule
{
public:
	virtual			~InputModule() { SAFE_RELEASE(gEnv->pInputSystem); }
	const char*		GetName() const final { return "InputModule"; }
	bool			Initialize(Environment& env, const SystemInitParams& initParams) final
	{
		env.pInputSystem = new InputSystem();
		return env.pInputSystem != nullptr;
	}
};

extern "C" DLL_EXPORT IEngineModule* CreateModule(ISystem * pSystem)
{
	auto pModule = new InputModule();
	// TODO: Subscribe to system events
	return pModule;
}
