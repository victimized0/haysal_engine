#ifndef SYSTEM_H
#define SYSTEM_H
#pragma once

#include <ISystem.h>
#include <SystemInitParams.h>

class System final : public ISystem
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
	void					RenderEnd() final;

	void					Quit() final;

	//ILocManager*			GetLocalizationManager() final;
	//IResourceManager*		GetIResourceManager() final;
	//ICmdLine*				GetICmdLine() final;
	//ILog*					GetILog() final;
	//IEntitySystem*		GetIEntitySystem() final;
	IWorldModule*			GetIWorld() final;
	IScripts*				GetIScripts() final;
	IPhysics*				GetIPhysics() final;
	IRenderer*				GetIRenderer() final;
	IAIModule*				GetIAIModule() final;
	IAnimModule*			GetIAnimModule() final;

	//HWND					GetHWND() final;

	//void					SetViewCamera(Camera& camera) final;
	//const Camera&			GetViewCamera() const final;

	void					SaveConfiguration() final;
	void					LoadConfiguration(const char* sFilename) final;

	void					ExecuteCommandLine() final;

	IEngineModule*			LoadModule(const char* moduleName, const SystemInitParams& initParams) final;
	bool					UnloadEngineModule(const char* moduleName) final;

	//ISystemEventListener

	bool					Initialise(SystemInitParams& initParams);
	void					RunMainLoop();

#if PLATFORM_WINDOWS
	friend LRESULT WINAPI	WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool					HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
#endif

	//void*					GetRootWindowMessageHandler() final;
	//void					RegisterWindowMessageHandler(IWindowMessageHandler* pHandler) final;
	//void					UnregisterWindowMessageHandler(IWindowMessageHandler* pHandler) final;
	//int						PumpWindowMessage(bool bAll, CRY_HWND hWnd) final;

	HMODULE					LoadDLL(const char* dllName);
	bool					UnloadDLL(const char* dllName);
	void					GetLoadedDLLs(std::vector<std::string>& moduleNames) const;

private:
	bool					InitModule(const SystemInitParams& startupParams, const char* dllName);
	bool					InitRenderModule(SystemInitParams& startupParams);
	bool					InitPhysicsModule(const SystemInitParams& startupParams);
	bool					InitAIModule(const SystemInitParams& startupParams);
	bool					InitAnimModule(const SystemInitParams& startupParams);
	bool					InitScriptModule(const SystemInitParams& startupParams);
	bool					InitWorldModule(const SystemInitParams& startupParams);
	//bool					InitFileSystem(const SystemInitParams& startupParams);
	//void					InitLog(const SystemInitParams& startupParams);
	//bool					InitEntitySystem(const SystemInitParams& startupParams);

	void					ShutDown();

private:
	HWND					m_hWnd;
	Environment				m_env;
	//Timer					m_timer;
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

	struct SDllHandles
	{
		HMODULE hRenderer;
		//HMODULE hEntitySystem;
		HMODULE hAI;
		HMODULE hPhysics;
		HMODULE hScript;
		HMODULE hWorld;
		HMODULE hAnimation;
	};
	SDllHandles				m_dll;

};

#endif //SYSTEM_H
