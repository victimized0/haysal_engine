#ifndef SYSTEM_H
#define SYSTEM_H
#pragma once

#include <System\ISystem.h>
#include <System\SystemInitParams.h>
#include <Library.h>
#include <Timer.h>

#define DLL_DX11_MODULE		"DX11Renderer"
#define DLL_VK_MODULE		"VKRenderer"
#define DLL_WORLD_MODULE	"WorldModule"

class System final : public ISystem, public IWindowMessageHandler
{
public:
							System(const SystemInitParams& startupParams);
	virtual					~System();

	//void					OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) final;

	Environment*			GetEnv() final;
	const char*				GetRootFolder() const final;
	bool					IsDevMode() const final;

	bool					Update() final;
	bool					DoFrame() final;
	void					RenderBegin() final;
	void					Render();
	void					RenderEnd() final;

	void					Quit() final;

	//IResourceManager*		GetIResourceManager() final;
	//ICmdLine*				GetICmdLine() final;
	//ILog*					GetILog() final;
	//IEntitySystem*		GetIEntitySystem() final;
	IWorldEngine*			GetIWorld() final;
	IScripts*				GetIScripts() final;
	IPhysics*				GetIPhysics() final;
	IRenderer*				GetIRenderer() final;
	IAIModule*				GetIAIModule() final;
	IAnimModule*			GetIAnimModule() final;

	WIN_HWND					GetHWND() final;

	//void					SetViewCamera(Camera& camera) final;
	//const Camera&			GetViewCamera() const final;

	void					SaveConfiguration() final;
	void					LoadConfiguration(const char* sFilename) final;

	void					ExecuteCommandLine() final;

	IEngineModule*			LoadModule(const char* moduleName, const SystemInitParams& initParams) final;
	bool					UnloadModule(const char* moduleName) final;

	//ISystemEventListener

	bool					Initialise(SystemInitParams& initParams);
	void					RunMainLoop();

#if PLATFORM_WINDOWS
	friend LRESULT WINAPI	WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool					HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) final;
#endif

	void*					GetWndProcHandler() final;
	void					RegisterWindowMessageHandler(IWindowMessageHandler* pHandler) final;
	void					UnregisterWindowMessageHandler(IWindowMessageHandler* pHandler) final;
	int						PumpWindowMessage(bool bAll, WIN_HWND hWnd) final;

	WIN_HMODULE				LoadDLL(const char* dllName);
	bool					UnloadDLL(const char* dllName);

	bool					ShouldQuit() { return m_shouldQuit; }

private:
	bool					InitModule(const SystemInitParams& startupParams, const char* dllName);
	bool					InitRenderModule(SystemInitParams& startupParams);
	bool					InitPhysicsModule(const SystemInitParams& startupParams);
	bool					InitAIModule(const SystemInitParams& startupParams);
	bool					InitAnimModule(const SystemInitParams& startupParams);
	bool					InitScriptModule(const SystemInitParams& startupParams);
	bool					InitWorldEngine(const SystemInitParams& startupParams);
	//bool					InitFileSystem(const SystemInitParams& startupParams);
	//void					InitLog(const SystemInitParams& startupParams);
	//bool					InitEntitySystem(const SystemInitParams& startupParams);

	bool					CloseRenderModule();

	void					ShutDown();

private:
	WIN_HWND				m_hWnd;
	Environment				m_env;
	Timer					m_timer;
	//Camera				m_viewCamera;
	//ResourceManager*		m_pResMan;
	//IFont*				m_pFont;
	//CmdLine*				m_pCmdLine;
	std::string				m_cmdLine;
	std::string				m_rootDir;
	std::string				m_binDir;
	int						m_height;
	int						m_width;
	bool					m_isDevMode;
	bool					m_isPaused;
	bool					m_shouldQuit;

	struct DllHandles
	{
		WIN_HMODULE hRenderer;
		//WIN_HMODULE hEntitySystem;
		WIN_HMODULE hAI;
		WIN_HMODULE hPhysics;
		WIN_HMODULE hScript;
		WIN_HMODULE hWorld;
		WIN_HMODULE hAnimation;
	};
	DllHandles m_dll;

	std::unordered_map <std::string, WIN_HMODULE> m_dllHandles;

	std::vector<IWindowMessageHandler*> m_winMsgHandlers;
};

#endif //SYSTEM_H
