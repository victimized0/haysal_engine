#ifndef INTERFACE_SYSTEM_H
#define INTERFACE_SYSTEM_H
#pragma once

#ifdef SYSTEM_EXPORTS
#define SYSTEM_API DLL_EXPORT
#else
#define SYSTEM_API DLL_IMPORT
#endif

struct ISystem;
struct IPhysics;
struct IRenderer;
struct IAIModule;
struct IScripts;
struct IAnimModule;
struct IWorldModule;

struct IEngineModule;
struct SystemInitParams;

struct Environment
{
	ISystem*		pSystem		= nullptr;
	IPhysics*		pPhysics	= nullptr;
	IRenderer*		pRenderer	= nullptr;
	IWorldModule*	pWorld		= nullptr;
	IAIModule*		pAI			= nullptr;
	IScripts*		pScripts	= nullptr;
	IAnimModule*	pAnimation	= nullptr;
};

struct ISystem
{
	virtual						~ISystem() {}

	virtual Environment*		GetEnv() = 0;
	virtual const char*			GetRootFolder() const = 0;
	virtual bool				IsDevMode() const = 0;

	virtual bool				Update() = 0;
	virtual bool				DoFrame() = 0;
	virtual void				RenderBegin() = 0;
	virtual void				RenderEnd() = 0;

	virtual void				Quit() = 0;

	//virtual ILocManager*		GetLocalizationManager() = 0;
	//virtual IResourceManager*	GetIResourceManager() = 0;
	//virtual ICmdLine*			GetICmdLine() = 0;
	//virtual ILog*				GetILog() = 0;
	//virtual IEntitySystem*	GetIEntitySystem() = 0;
	virtual IWorldModule*		GetIWorld() = 0;
	virtual IScripts*			GetIScripts() = 0;
	virtual IPhysics*			GetIPhysics() = 0;
	virtual IRenderer*			GetIRenderer() = 0;
	virtual IAIModule*			GetIAIModule() = 0;
	virtual IAnimModule*		GetIAnimModule() = 0;

	//virtual HWND				GetHWND() = 0;

	//virtual void				SetViewCamera(Camera& camera) = 0;
	//virtual const Camera&		GetViewCamera() const = 0;

	virtual void				SaveConfiguration() = 0;
	virtual void				LoadConfiguration(const char* sFilename) = 0;

	virtual void				ExecuteCommandLine() = 0;

	virtual IEngineModule*		LoadModule(const char* moduleName, const SystemInitParams& initParams) = 0;
	virtual bool				UnloadEngineModule(const char* moduleName) = 0;
	//ISystemEventListener
};

typedef ISystem* (*PFNLoadSystemInterface)(SystemInitParams& initParams);

//! Interface of the DLL.
extern "C"
{
	SYSTEM_API ISystem* LoadSystem(SystemInitParams& initParams);
}

#endif //INTERFACE_SYSTEM_H
