#include "stdafx.h"
#include "System.h"

System::System(const SystemInitParams& startupParams)
	: m_hWnd(nullptr)
	, m_width(0)
	, m_height(0)
	, m_isDevMode(false)
	, m_isPaused(false)
{

}

System::~System()
{

}

bool System::Initialise(SystemInitParams& initParams)
{
	return true;
}

void System::RunMainLoop()
{
	//while (true)
	//{

	//}
}

Environment* System::GetEnv()
{
	return &m_env;
}

const char* System::GetRootFolder() const
{
	return nullptr;
}

bool System::IsDevMode() const
{
	return true;
}

bool System::Update()
{
	return true;
}

bool System::DoFrame()
{
	return true;
}

void System::RenderBegin()
{

}

void System::RenderEnd()
{

}

void System::Quit()
{

}

IWorldModule* System::GetIWorld()
{
	return m_env.pWorld;
}

IScripts* System::GetIScripts()
{
	return m_env.pScripts;
}

IPhysics* System::GetIPhysics()
{
	return m_env.pPhysics;
}

IRenderer* System::GetIRenderer()
{
	return m_env.pRenderer;
}

IAIModule* System::GetIAIModule()
{
	return m_env.pAI;
}

IAnimModule* System::GetIAnimModule()
{
	return m_env.pAnimation;
}

void System::SaveConfiguration()
{

}

void System::LoadConfiguration(const char* sFilename)
{

}

void System::ExecuteCommandLine()
{

}

IEngineModule* System::LoadModule(const char* moduleName, const SystemInitParams& initParams)
{
	return nullptr;
}

bool System::UnloadEngineModule(const char* moduleName)
{
	return true;
}

bool System::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	return true;
}

HMODULE System::LoadDLL(const char* dllName)
{
	return nullptr;
}

bool System::UnloadDLL(const char* dllName)
{
	return true;
}

void System::GetLoadedDLLs(std::vector<std::string>& moduleNames) const
{

}

bool System::InitModule(const SystemInitParams& startupParams, const char* dllName)
{
	return true;
}

bool System::InitRenderModule(SystemInitParams& startupParams)
{
	return true;
}

bool System::InitPhysicsModule(const SystemInitParams& startupParams)
{
	return true;
}

bool System::InitAIModule(const SystemInitParams& startupParams)
{
	return true;
}

bool System::InitAnimModule(const SystemInitParams& startupParams)
{
	return true;
}

bool System::InitScriptModule(const SystemInitParams& startupParams)
{
	return true;
}

bool System::InitWorldModule(const SystemInitParams& startupParams)
{
	return true;
}

void System::ShutDown()
{

}
