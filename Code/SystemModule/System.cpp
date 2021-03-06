#include "stdafx.h"
#include "System.h"

#include <WorldModule\IWorldEngine.h>
#include <RenderModule\IRenderer.h>
#include <InputModule\IInputSystem.h>
#include <AIModule\IAISystem.h>
#include <EntitySystem\IEntitySystem.h>
#include <GameFramework\IGameFramework.h>

#include "..\EntitySystem\EntitySystem.h"

#if PLATFORM_WINDOWS
static LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	System* pSystem = 0;
	if (gEnv)
	{
		pSystem = static_cast<System*>(gEnv->pSystem);
	}

	if (pSystem && !pSystem->ShouldQuit())
	{
		LRESULT result = S_OK;
		bool bAny = false;
		for (std::vector<IWindowMessageHandler*>::const_iterator it = pSystem->m_winMsgHandlers.begin(); it != pSystem->m_winMsgHandlers.end(); ++it)
		{
			IWindowMessageHandler* pHandler = *it;
			LRESULT maybeResult = 0xDEADDEAD;
			if (pHandler->HandleMessage(hWnd, uMsg, wParam, lParam, &maybeResult))
			{
				//assert(maybeResult != 0xDEADDEAD && "Message handler indicated a resulting value, but no value was written");
				if (bAny)
				{
					//assert(result == maybeResult && "Two window message handlers tried to return different result values");
				}
				else
				{
					bAny = true;
					result = maybeResult;
				}
			}
		}
		if (bAny)
		{
			// One of the registered handlers returned something
			return result;
		}
	}

	// Handle with the default procedure
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
#endif //PLATFORM_WINDOWS

System::System(const SystemInitParams& startupParams)
	: m_hWnd(nullptr)
	, m_width(800)
	, m_height(600)
	, m_isDevMode(false)
	, m_isPaused(false)
	, m_shouldQuit(false)
{
	memset(&m_dll, 0, sizeof(m_dll));

	gEnv = &m_env;
	m_env.pSystem = this;
	m_env.pTimer = &m_timer;

	m_rootDir = GetRootFolder();

	RegisterWindowMessageHandler(this);
}

System::~System()
{
	ShutDown();
	UnregisterWindowMessageHandler(this);

	SafeFreeLib(m_dll.hAI);
	SafeFreeLib(m_dll.hScript);
	SafeFreeLib(m_dll.hPhysics);
	//SafeFreeLib(m_dll.hEntitySystem);
	SafeFreeLib(m_dll.hRenderer);
	SafeFreeLib(m_dll.hWorld);
	SafeFreeLib(m_dll.hAnimation);
	SafeFreeLib(m_dll.hInput);
	SafeFreeLib(m_dll.hGame);

	//SAFE_DELETE(m_pResourceManager);
	//SAFE_DELETE(m_pSystemEventDispatcher);

	m_env.pSystem = nullptr;
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

	//if (!InitPhysics(initParams))
	//	return false;

	if (!InitRenderModule(initParams))
		return false;
	m_env.pRenderer->PostInit();

	if (!m_timer.Init())
		return (false);
	m_timer.Reset();

	if (!InitInputSystem(initParams))
		return false;

	//if (!InitAnimationSystem(initParams))
	//	return false;

	//if (!InitScriptSystem(initParams))
	//	return false;

	if (!InitAIModule(initParams))
		return false;

	if (!InitEntitySystem(initParams))
		return false;

	if (!InitWorldEngine(initParams))
		return false;

	if (!InitGameFramework(initParams))
		return false;

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

bool System::Update()
{
	Camera camera = GetViewCamera();
	const int newWidth	= m_env.pRenderer->GetWidth();
	const int newHeight = m_env.pRenderer->GetHeight();

	if ((newWidth != camera.GetWidth()) || (newHeight != camera.GetHeight()))
	{
		camera.Zoom(-200);
		camera.SetFrustum( newWidth, newHeight, camera.GetFov(), camera.GetNearPlane(), camera.GetFarPlane() );
		SetViewCamera(camera);
	}

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

	m_timer.Tick();

	if (m_env.pWorld /* && bNotLoading*/)
		m_env.pWorld->OnFrameStart();

	//if (m_env.pScripts /* && bNotLoading*/)
	//	m_env.pScripts->Update();

	//m_pResourceManager->Update();

	if (m_env.pWorld)
		m_env.pWorld->Update();

	if (m_env.pAISystem)
		m_env.pAISystem->Update();

	return true;
}

bool System::DoFrame()
{
	
	RenderBegin();

	if (!Update())
	{

	}

	if (m_env.pGameFramework)
		m_env.pGameFramework->Update(m_timer.GetFrameTime());

	Render();

	//if (m_env.pGameFramework)
	//	m_env.pGameFramework->PostRender();

	RenderEnd();

	return true;
}

void System::RenderBegin()
{
	if (m_env.pRenderer)
	{
		m_env.pRenderer->BeginFrame();
	}
}

void System::RenderEnd()
{
	if (!m_env.pRenderer)
		return;

#if !defined (_RELEASE)
	// Flush render data and swap buffers.
	//m_env.pRenderer->RenderDebug(bRenderStats);
#endif

	m_env.pRenderer->EndFrame();
}

void System::Render()
{
	const int flags = (RenderFlags::AllowHDR | RenderFlags::AllowPostProcess | RenderFlags::ZPass);
	
	if (m_env.pRenderer)
	{
		if (m_env.pWorld)
		{
			m_env.pWorld->RenderScene(flags, m_viewCamera);
		}

		if (m_env.pAISystem)
		{
		//	m_env.pAI->DebugDraw();
		}
	}
}

void System::Quit()
{
	if (m_env.pRenderer)
		m_env.pRenderer->ShutDown();

	ShutDown();

#if PLATFORM_ORBIS
	_Exit(0);
#elif PLATFORM_WINDOWS
	TerminateProcess(GetCurrentProcess(), 0);
#else
	_exit(0);
#endif

#if !PLATFORM_LINUX && !PLATFORM_DURANGO && !PLATFORM_ORBIS
	PostQuitMessage(0);
#endif
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

bool System::UnloadModule(const char* moduleName)
{
	return UnloadDLL(moduleName);
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
	{
		Quit();
		return false;
	}
	case WM_KEYDOWN:
	{
		if (m_env.pInputSystem)
			m_env.pInputSystem->ProcessMessage(InputType::Keyboard, uMsg, wParam, lParam);
		if (wParam == VK_ESCAPE)
		{
			Quit();
			return false;
		}
		return true;
	}
	case WM_WINDOWPOSCHANGED:
	{
		wpos = (LPWINDOWPOS)lParam;
		if (wpos && !(wpos->flags & (SWP_NOMOVE | SWP_NOSIZE)))
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
		if (m_env.pInputSystem)
			m_env.pInputSystem->ProcessMessage(InputType::Both, uMsg, wParam, lParam);
		return true;
	case WM_SYSKEYDOWN:
	{
		if (m_env.pInputSystem)
			m_env.pInputSystem->ProcessMessage(InputType::Keyboard, uMsg, wParam, lParam);
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
		return true;
	}
	// Events that should be forwarded to the hardware mouse
	case WM_MOUSEMOVE:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_INPUT:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		if (m_env.pInputSystem)
			m_env.pInputSystem->ProcessMessage(InputType::Mouse, uMsg, wParam, lParam);
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (m_env.pInputSystem)
			m_env.pInputSystem->ProcessMessage(InputType::Keyboard, uMsg, wParam, lParam);
		break;

	// Any other event doesn't interest us
	default:
		return false;
	}

	return true;
}
#endif //PLATFORM_WINDOWS

WIN_HMODULE System::LoadDLL(const char* dllName)
{
	WIN_HMODULE handle = nullptr;

	handle = LoadLib(dllName);
	if (!handle)
	{

	}
	if (handle == nullptr)
	{
		Quit();
	}

	m_dllHandles.emplace(dllName, handle);
	
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
	auto it = m_dllHandles.find(dllName);
	if (it != m_dllHandles.end())
	{
#if PLATFORM_WINDOWS || PLATFORM_DURANGO || PLATFORM_LINUX
		WIN_HMODULE hModule = it->second;
#endif
		FreeLib(hModule);
		m_dllHandles.erase(it);

		return true;
	}

	return false;
}

const pugi::xml_node& System::CreateXmlNode(const char* nodeName)
{
	return pugi::xml_node();
}

pugi::xml_document System::LoadXmlFromBuffer(const char* buffer, size_t size)
{
	using namespace pugi;

	xml_document doc;
	xml_parse_result res = doc.load_buffer(buffer, size);
	assert(res.status == status_ok);

	return doc;
}

pugi::xml_document System::LoadXmlFromFile(const char* filename)
{
	using namespace pugi;
	std::string path = m_rootDir + filename;

	xml_document doc;
	xml_parse_result res = doc.load_file(path.c_str());
	assert(res.status == status_ok);

	return doc;
}

bool System::InitModule(const SystemInitParams& startupParams, const char* dllName)
{
	WIN_HMODULE dll = LoadDLL(dllName);

	if (dll == 0)
		return false;

	IEngineModule* pModule = nullptr;

	typedef IEngineModule* (*CreateModuleFunc)(ISystem* pSystem, const char* dllName);
	auto pCreateModuleFunc = (CreateModuleFunc)GetProcAddress(dll, "CreateModule");
	if (pCreateModuleFunc)
		pModule = pCreateModuleFunc(this, dllName);

	if (pModule == nullptr)
		return false;

	if (!pModule->Initialize(m_env, startupParams))
		return false;

	return true;
}

bool System::InitRenderModule(SystemInitParams& startupParams)
{
	const char* libName = DLL_DX11_MODULE;
	//if (param == "DX11")
	// libName = DLL_DX11_MODULE
	// else if (param == "VK")
	// libName = DLL_VK_MODULE
	
	if (!InitModule(startupParams, libName))
		return false;

	if (!m_env.pRenderer)
		return false;

	if (m_env.pRenderer)
	{
		int width = m_width;
		int height = m_height;

		m_hWnd = m_env.pRenderer->Init( width, height, startupParams );
		return m_hWnd != nullptr;
	}

	// TODO: Init other systems and modules

	if (m_env.pRenderer)
	{
		// Call post init after systems that renderer depends on were initialised
		m_env.pRenderer->PostInit();
	}

	return true;
}

bool System::InitPhysicsModule(const SystemInitParams& startupParams)
{
	return true;
}

bool System::InitAIModule(const SystemInitParams& startupParams)
{
	if (!InitModule(startupParams, DLL_AI_MODULE))
		return false;

	if (!m_env.pAISystem)
		return false;

	m_env.pAISystem->Init();
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

bool System::InitWorldEngine(const SystemInitParams& startupParams)
{
	if (!InitModule(startupParams, DLL_WORLD_MODULE))
		return false;

	if (!m_env.pWorld)
		return false;

	return m_env.pWorld->Init();
}

bool System::InitInputSystem(const SystemInitParams& startupParams)
{
	if (!InitModule(startupParams, DLL_INPUT_MODULE))
		return false;

	if (!m_env.pInputSystem)
		return false;

	return m_env.pInputSystem->Init();
}

bool System::InitGameFramework(const SystemInitParams& startupParams)
{
	if (!InitModule(startupParams, DLL_GAME_MODULE))
		return false;

	if (!m_env.pGameFramework)
		return false;

	return m_env.pGameFramework->Init();
}

bool System::InitEntitySystem(const SystemInitParams& startupParams)
{
	// TODO: Move this to a .dll
	m_env.pEntitySystem = new EntitySystem();
	return m_env.pEntitySystem != nullptr;
}

bool System::CloseRenderModule()
{
	std::string libName = "DX11Renderer";
	//if (param == "DX11")
	// libName = "DX11Renderer"
	// else if (param == "VK")
	// libName = "VKRenderer"

	return UnloadModule(libName.c_str());
}

void System::ShutDown()
{
	//if (m_pSystemEventDispatcher)
	//{
	//	m_pSystemEventDispatcher->RemoveListener(this);
	//}

	/*if (m_pUserCallback)
	{
		m_pUserCallback->OnShutdown();
		m_pUserCallback = nullptr;
	}*/

	SAFE_RELEASE(m_env.pRenderer);

	if (m_env.pEntitySystem)
		m_env.pEntitySystem->ShutDown();

	if (m_env.pWorld)
		m_env.pWorld->ShutDown();

	//m_pResourceManager->Shutdown();

	UnloadModule(DLL_AI_MODULE);
	UnloadModule(DLL_ANIM_MODULE);
	UnloadModule(DLL_WORLD_MODULE);
	UnloadModule(DLL_ENTITY_MODULE);
	UnloadModule(DLL_PHYSICS_MODULE);
	UnloadModule(DLL_LUA_MODULE);
	UnloadModule(DLL_INPUT_MODULE);
	UnloadModule(DLL_GAME_MODULE);

	CloseRenderModule(); // TODO: might move this somewhere else

	SAFE_DELETE(m_env.pRenderer);
	SAFE_DELETE(m_env.pEntitySystem); // TODO: Move to a .dll
	//SAFE_DELETE(m_pCmdLine);
	//SAFE_RELEASE(m_env.pConsole);

}

void* System::GetWndProcHandler()
{
#if PLATFORM_WINDOWS
	return &WndProc;
#else
	return nullptr;
#endif
}

void System::RegisterWindowMessageHandler(IWindowMessageHandler* pHandler)
{
	m_winMsgHandlers.push_back(pHandler);
}

void System::UnregisterWindowMessageHandler(IWindowMessageHandler* pHandler)
{
	auto it = std::find(m_winMsgHandlers.begin(), m_winMsgHandlers.end(), pHandler);
	if (it != m_winMsgHandlers.end())
	{
		m_winMsgHandlers.erase(it);
	}
}

int System::PumpWindowMessage(bool bAll, WIN_HWND phWnd)
{
#if PLATFORM_WINDOWS
	int count = 0;
	const HWND hWnd = (HWND)phWnd;

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

WIN_HWND System::GetHWND()
{
	return m_hWnd;
}

void System::SetViewCamera(Camera& camera)
{
	m_viewCamera = camera;
	m_viewCamera.CalculateMatrices();
}

const Camera& System::GetViewCamera() const
{
	return m_viewCamera;
}
