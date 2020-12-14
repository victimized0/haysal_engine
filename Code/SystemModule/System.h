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
#define DLL_AI_MODULE		"AIModule"
#define DLL_ANIM_MODULE		"AnimationModule"
#define DLL_LUA_MODULE		"LuaModule"
#define DLL_PHYSICS_MODULE	"PhysicsModule"
#define DLL_ENTITY_MODULE	"EntityModule"
#define DLL_INPUT_MODULE	"InputModule"

class System final : public ISystem, public IWindowMessageHandler
{
public:
									System(const SystemInitParams& startupParams);
	virtual							~System();

	//void							OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) final;

	virtual Environment*			GetEnv() final { return &m_env; }
	virtual const char*				GetRootDir() const final { return m_rootDir.c_str(); }
	virtual std::string				GetEngineDir() const final { return Path::AppendPath(m_rootDir, "Engine"); }
	virtual std::string				GetDataDir() const final { return Path::AppendPath(m_rootDir, "Data"); }
	virtual std::string				GetAbsPath(const char* path) const final { return Path::AppendPath(m_rootDir, path); }
	virtual bool					IsDevMode() const final { return true; }

	virtual bool					Update() final;
	virtual bool					DoFrame() final;
	virtual void					RenderBegin() final;
	virtual void					Render();
	virtual void					RenderEnd() final;

	virtual void					Quit() final;

	//virtual IResourceManager*		GetIResourceManager() final;
	//virtual ICmdLine*				GetICmdLine() final;
	//virtual ILog*					GetILog() final;
	//virtual IEntitySystem*		GetIEntitySystem() final;
	virtual IWorldEngine*			GetIWorld() final;
	virtual IScripts*				GetIScripts() final;
	virtual IPhysics*				GetIPhysics() final;
	virtual IRenderer*				GetIRenderer() final;
	virtual IAIModule*				GetIAIModule() final;
	virtual IAnimModule*			GetIAnimModule() final;

	virtual WIN_HWND				GetHWND() final;

	virtual void					SetViewCamera(Camera& camera) final;
	virtual const Camera&			GetViewCamera() const final;

	virtual void					SaveConfiguration() final;
	virtual void					LoadConfiguration(const char* sFilename) final;

	virtual void					ExecuteCommandLine() final;

	virtual IEngineModule*			LoadModule(const char* moduleName, const SystemInitParams& initParams) final;
	virtual bool					UnloadModule(const char* moduleName) final;

	//ISystemEventListener

	virtual bool					Initialise(SystemInitParams& initParams);
	virtual void					RunMainLoop();

#if PLATFORM_WINDOWS
	friend LRESULT WINAPI			WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual bool					HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) final;
#endif

	virtual void*					GetWndProcHandler() final;
	virtual void					RegisterWindowMessageHandler(IWindowMessageHandler* pHandler) final;
	virtual void					UnregisterWindowMessageHandler(IWindowMessageHandler* pHandler) final;
	virtual int						PumpWindowMessage(bool bAll, WIN_HWND hWnd) final;

	WIN_HMODULE						LoadDLL(const char* dllName);
	bool							UnloadDLL(const char* dllName);

	bool							ShouldQuit() { return m_shouldQuit; }

	virtual const pugi::xml_node&	CreateXmlNode(const char* nodeName = "") final;
	virtual pugi::xml_document		LoadXmlFromBuffer(const char* buffer, size_t size) final;
	virtual pugi::xml_document		LoadXmlFromFile(const char* filename) final;

private:
	bool							InitModule(const SystemInitParams& startupParams, const char* dllName);
	bool							InitRenderModule(SystemInitParams& startupParams);
	bool							InitPhysicsModule(const SystemInitParams& startupParams);
	bool							InitAIModule(const SystemInitParams& startupParams);
	bool							InitAnimModule(const SystemInitParams& startupParams);
	bool							InitScriptModule(const SystemInitParams& startupParams);
	bool							InitWorldEngine(const SystemInitParams& startupParams);
	//bool							InitFileSystem(const SystemInitParams& startupParams);
	//void							InitLog(const SystemInitParams& startupParams);
	//bool							InitEntitySystem(const SystemInitParams& startupParams);

	bool							CloseRenderModule();
	void							ShutDown();

private:
	WIN_HWND						m_hWnd;
	Environment						m_env;
	Timer							m_timer;
	Camera							m_viewCamera;
	//ResourceManager*				m_pResMan;
	//IFont*						m_pFont;
	//CmdLine*						m_pCmdLine;
	std::string						m_cmdLine;
	std::string						m_rootDir;
	std::string						m_binDir;
	int								m_height;
	int								m_width;
	bool							m_isDevMode;
	bool							m_isPaused;
	bool							m_shouldQuit;

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

	std::unordered_map <std::string, WIN_HMODULE>	m_dllHandles;
	std::vector<IWindowMessageHandler*>				m_winMsgHandlers;
};

#endif //SYSTEM_H
