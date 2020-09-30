#include "stdafx.h"
#include "System.h"

#include <RenderModule\IRenderer.h>
#include <RenderModule\IRenderModule.h>

System::System(const SystemInitParams& startupParams)
	: m_hWnd(nullptr)
	, m_width(0)
	, m_height(0)
	, m_isDevMode(false)
	, m_isPaused(false)
	, m_shouldQuit(false)
{
	gEnv = &m_env;
}

System::~System()
{
	gEnv = nullptr;
}

bool System::Initialise(SystemInitParams& initParams)
{
	//m_cmdLine = initParams.systemCmdLine;
	//m_pCmdLine = new CmdLine(initParams.systemCmdLine);
	//m_env.szCmdLine = m_szCmdLine.c_str();
	//m_binariesDir = startupParams.szBinariesDir;
	
#if PLATFORM_DESKTOP && !defined(_RELEASE)
	//m_env.SetIsEditor(startupParams.bEditor);
#endif

	//SetDevMode(devModeEnable);
	//m_pProjectManager = new Cry::CProjectManager();
	//InitFileSystem(startupParams);

	//CreateSystemVars();
	//InitFileSystem_LoadEngineFolders();
	//LoadConfiguration("user.cfg", pCVarsWhiteListConfigSink, eLoadConfigInit, ELoadConfigurationFlags::SuppressConfigNotFoundWarning);
	//m_pTextModeConsole->SetTitle(m_pProjectManager->GetCurrentProjectName());

	//if (!InitPhysics(startupParams))
	//	return false;

	if (!InitRenderModule(initParams))
		return false;
	m_env.pRenderer->PostInit();

	//if (!m_timer.Init())
	//	return (false);
	//m_timer.Reset();

	//if (!InitInput(startupParams))
	//	return false;

	//if (!InitAnimationSystem(startupParams))
	//	return false;

	//if (!Init3DEngine(startupParams))
	//	return false;

	//if (!InitScriptSystem(startupParams))
	//	return false;

	//if (!InitEntitySystem(startupParams))
	//	return false;

	//if (!InitAISystem(startupParams))
	//	return false;

	//InitGameFramework(startupParams);
	//m_env.pInput->PostInit();


	return true;
}

void System::RunMainLoop()
{
	while (true)
	{
		if (!DoFrame())
		{
			break;
		}
	}
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
	// View camera update
	// Game pause state update
	// Return false if game is to be closed

#if PLATFORM_WINDOWS
	{
		if (m_hWnd && ::IsWindow((HWND)m_hWnd))
		{
			PumpWindowMessage(true, m_hWnd);
		}
	}
#endif

	return true;
}

bool System::DoFrame()
{
	//if (m_env.pGameFramework != nullptr)
	//{
	//	m_env.pGameFramework->PreSystemUpdate();
	//}
	
	RenderBegin();

	if (!Update())
	{

	}

	//if (m_env.pGameFramework != nullptr)
	//{
	//	m_env.pGameFramework->PreRender();
	//}

	Render();

	//if (m_env.pGameFramework != nullptr)
	//{
	//	m_env.pGameFramework->PostRender(updateFlags);
	//}

	RenderEnd();

	//if (m_env.pGameFramework != nullptr)
	//{
	//	m_env.pGameFramework->PostRenderSubmit();
	//}

	return true;
}

void System::RenderBegin()
{

}

void System::Render()
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

#ifdef PLATFORM_WINDOWS

	#ifndef GET_X_LPARAM
		#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
	#endif
	#ifndef GET_Y_LPARAM
		#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
	#endif

bool System::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	static bool sbInSizingModalLoop;
	static WPARAM lastWindowMode = -1;

	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	*pResult = 0;
	LPWINDOWPOS wpos;

	switch (uMsg)
	{
	// System event translation
	case WM_CLOSE:
		Quit();
		return false;

	case WM_WINDOWPOSCHANGED:
	{
		wpos = (LPWINDOWPOS)lParam;
		if (!(wpos->flags & (SWP_NOMOVE | SWP_NOSIZE)))
		{
			if (!(wpos->flags & SWP_NOMOVE))
			{
				// If a MOVE is detected (logic inversion intended, the flag NONOMOVE must NOT be set) send a MOVE event (top-left location as params)
				//OnSystemEvent(ESYSTEM_EVENT_MOVE, wpos->x, wpos->y);
				return false;
			}
			// Don't check here for SWP_NOSIZE, return true to DefWndProc, which will generate a WM_SIZE event if required
		}
		return true;
	}
	case WM_SIZE:
	{
		const bool isBeingRestoredDown = (wParam == SIZE_RESTORED && lastWindowMode == SIZE_MAXIMIZED);
		const bool isBeingMaximized = (wParam == SIZE_MAXIMIZED);

		if (!sbInSizingModalLoop || isBeingMaximized || isBeingRestoredDown)
		{
			//OnSystemEvent(ESYSTEM_EVENT_RESIZE, LOWORD(lParam), HIWORD(lParam));
		}

		lastWindowMode = wParam;
		return true;
	}
	case WM_ACTIVATE:
		// Pass HIWORD(wParam) as well to indicate whether this window is minimized or not
		// HIWORD(wParam) != 0 is minimized, HIWORD(wParam) == 0 is not minimized
		//OnSystemEvent(ESYSTEM_EVENT_ACTIVATE, LOWORD(wParam) != WA_INACTIVE, HIWORD(wParam));
		return true;
	case WM_SYSKEYDOWN:
	{
		// If ALT is pressed
		const bool bAltPressed = ((lParam >> 29 & 0x1) != 0);
		if (bAltPressed)
		{
			//const bool isFullScreen = (cvWinType != nullptr && cvWinType->GetIVal() == 3);

			// ALT + Enter to go exclusive fullscreen
			if (wParam == VK_RETURN)
			{
				//OnSystemEvent(ESYSTEM_EVENT_TOGGLE_FULLSCREEN, (isFullScreen ? 0 : 1), 0);
			}
			// Tell Windows the message is handled
		}
		*pResult = 0;
		return true;
	}
	break;
	// Mouse activation
	case WM_MOUSEACTIVATE:
		*pResult = MA_ACTIVATEANDEAT;
		return true;
		// Hardware mouse counters
	case WM_EXITSIZEMOVE:
	{
		if (!sbInSizingModalLoop)
		{
			return false;
		}
		sbInSizingModalLoop = false;

		RECT clientRct = { 0, 0, 0, 0 };
		::GetClientRect(static_cast<HWND>(hWnd), &clientRct);
		//OnSystemEvent(ESYSTEM_EVENT_RESIZE, (clientRct.right - clientRct.left), (clientRct.bottom - clientRct.top));
	}
	// Events that should be forwarded to the hardware mouse
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_LBUTTONDBLCLK:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_RBUTTONUP:
		break;
	case WM_RBUTTONDBLCLK:
		break;
	case WM_MBUTTONDOWN:
		break;
	case WM_MBUTTONUP:
		break;
	case WM_MBUTTONDBLCLK:
		break;
	case WM_MOUSEWHEEL:
		break;

	// Any other event doesn't interest us
	default:
		return false;
	}

	return true;
}
#endif //PLATFORM_WINDOWS

HMODULE System::LoadDLL(const char* dllName)
{
	HMODULE handle = nullptr;

	handle = LoadLibraryA(dllName);
	if (!handle)
	{

	}
	if (handle == nullptr)
	{
		Quit();
	}

	//m_moduleDLLHandles.emplace(dllName, handle);
	//std::string moduleName = PathUtil::GetFileName(dllName);
	
	typedef void* (*PtrFunc_ModuleInitISystem)(ISystem* pSystem, const char* dllName);
	PtrFunc_ModuleInitISystem pfnModuleInitISystem = (PtrFunc_ModuleInitISystem)GetProcAddress(handle, "ModuleInitISystem");
	if (pfnModuleInitISystem)
	{
		pfnModuleInitISystem(this, dllName);
	}

	return handle;
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
	HMODULE dll = LoadDLL(dllName);

	if (dll == 0)
		return false;

	IEngineModule* pModule = nullptr;

	typedef void* (*CreateModuleFunc)(ISystem* pSystem, const char* dllName);
	auto pCreateModuleFunc = (CreateModuleFunc)GetProcAddress(dll, "CreateModule");
	if (pCreateModuleFunc)
	{
		pModule = static_cast<IEngineModule*>(pCreateModuleFunc(this, dllName));
	}

	if (pModule == nullptr)
		return false;

	if (!pModule->Initialize(m_env, startupParams))
		return false;

	return true;
}

bool System::InitRenderModule(SystemInitParams& startupParams)
{
	//if (param == "DX11")
	// libName = DX11
	// else if (param == "Vulkan")
	// libName = Vulkan
	
	if (!InitModule(startupParams, "DX11Renderer"))
		return false;

	if (!m_env.pRenderer)
		return false;

	if (m_env.pRenderer)
	{
		int width = m_width;
		int height = m_height;

		m_hWnd = m_env.pRenderer->Init( 0, 0, width, height, startupParams );
		return m_hWnd != 0;
	}

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

int System::PumpWindowMessage(bool bAll, PHWND opaqueHWnd)
{
#if PLATFORM_WINDOWS
	int count = 0;
	const HWND hWnd = (HWND)opaqueHWnd;

	do
	{
		MSG msg = {};
		BOOL bHasMessage = PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE);
		if (bHasMessage == FALSE) break;
		++count;

		// Special case for WM_QUIT
		if (msg.message == WM_QUIT)
		{
			return -1;
		}

		// Dispatch the message
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	} while (bAll);

	return count;
#else
	return 0;
#endif
}

PHWND System::GetHWND()
{
	return m_hWnd;
}

#if PLATFORM_WINDOWS
static LRESULT WINAPI WndProc(PHWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	System* pSystem = 0;
	if (gEnv)
	{
		pSystem = static_cast<System*>(gEnv->pSystem);
	}

	if (pSystem && !pSystem->ShouldQuit())
	{
		LRESULT result;
		bool bAny = false;
		//for (std::vector<IWindowMessageHandler*>::const_iterator it = pSystem->m_windowMessageHandlers.begin(); it != pSystem->m_windowMessageHandlers.end(); ++it)
		//{
		//	IWindowMessageHandler* pHandler = *it;
		//	LRESULT maybeResult = 0xDEADDEAD;
		//	if (pHandler->HandleMessage(hWnd, uMsg, wParam, lParam, &maybeResult))
		//	{
		//		assert(maybeResult != 0xDEADDEAD && "Message handler indicated a resulting value, but no value was written");
		//		if (bAny)
		//		{
		//			assert(result == maybeResult && "Two window message handlers tried to return different result values");
		//		}
		//		else
		//		{
		//			bAny = true;
		//			result = maybeResult;
		//		}
		//	}
		//}
		if (bAny)
		{
			// One of the registered handlers returned something
			return result;
		}
	}

	// Handle with the default procedure
	return DefWindowProc((HWND)hWnd, uMsg, wParam, lParam);
}
#endif //PLATFORM_WINDOWS
