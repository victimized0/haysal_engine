#ifndef INTERFACE_SYSTEM_H
#define INTERFACE_SYSTEM_H
#pragma once

#ifdef SYSTEM_EXPORTS
#define SYSTEM_API DLL_EXPORT
#else
#define SYSTEM_API DLL_IMPORT
#endif

#include "IEngineModule.h"
#include "IWindowMessageHandler.h"
#include "..\Camera.h"

// Have to define this because other platforms might not have Windows types
typedef void* WIN_HWND;
typedef void* WIN_HMODULE;

struct ISystem;
struct IPhysics;
struct IWorldEngine;
struct IRenderer;
struct IAISystem;
struct IScripts;
struct IAnimModule;
struct ITimer;
struct IEntitySystem;

struct SystemInitParams;

struct Environment
{
	ISystem*		pSystem			= nullptr;
	IPhysics*		pPhysics		= nullptr;
	IRenderer*		pRenderer		= nullptr;
	IWorldEngine*	pWorld			= nullptr;
	IAISystem*		pAISystem		= nullptr;
	IEntitySystem*	pEntitySystem	= nullptr;
	IScripts*		pScripts		= nullptr;
	IAnimModule*	pAnimation		= nullptr;
	ITimer*			pTimer			= nullptr;
};

extern Environment* gEnv;

struct ISystem
{
	virtual							~ISystem() {}

	virtual Environment*			GetEnv() = 0;
	virtual const char*				GetRootDir() const = 0;
	virtual std::string				GetEngineDir() const = 0;
	virtual std::string				GetDataDir() const = 0;
	virtual std::string				GetAbsPath(const char* path) const = 0;
	virtual bool					IsDevMode() const = 0;

	virtual bool					Update() = 0;
	virtual bool					DoFrame() = 0;
	virtual void					RenderBegin() = 0;
	virtual void					RenderEnd() = 0;

	virtual void					Quit() = 0;

	//virtual IResourceManager*		GetIResourceManager() = 0;
	//virtual ICmdLine*				GetICmdLine() = 0;
	//virtual ILog*					GetILog() = 0;
	virtual IEntitySystem*			GetIEntitySystem() = 0;
	virtual IWorldEngine*			GetIWorld() = 0;
	virtual IScripts*				GetIScripts() = 0;
	virtual IPhysics*				GetIPhysics() = 0;
	virtual IRenderer*				GetIRenderer() = 0;
	virtual IAISystem*				GetIAISystem() = 0;
	virtual IAnimModule*			GetIAnimModule() = 0;

	virtual WIN_HWND				GetHWND() = 0;

	virtual void					SetViewCamera(Camera& camera) = 0;
	virtual const Camera&			GetViewCamera() const = 0;

	virtual void					SaveConfiguration() = 0;
	virtual void					LoadConfiguration(const char* sFilename) = 0;

	virtual void					ExecuteCommandLine() = 0;

	virtual IEngineModule*			LoadModule(const char* moduleName, const SystemInitParams& initParams) = 0;
	virtual bool					UnloadModule(const char* moduleName) = 0;

	virtual void*					GetWndProcHandler() = 0;
	virtual void					RegisterWindowMessageHandler(IWindowMessageHandler* pHandler) = 0;
	virtual void					UnregisterWindowMessageHandler(IWindowMessageHandler* pHandler) = 0;
	virtual int						PumpWindowMessage(bool bAll, WIN_HWND hWnd) = 0;

	//ISystemEventListener
	virtual const pugi::xml_node&	CreateXmlNode(const char* nodeName = "") = 0;
	virtual pugi::xml_document		LoadXmlFromBuffer(const char* buffer, size_t size) = 0;
	virtual pugi::xml_document		LoadXmlFromFile(const char* filename) = 0;
};

typedef ISystem* (*PFNLoadSystemInterface)(SystemInitParams& initParams, bool manualUpdate);

//! Interface of the DLL.
extern "C"
{
	SYSTEM_API ISystem* LoadSystem(SystemInitParams& initParams, bool manualUpdate = false);
}

#endif //INTERFACE_SYSTEM_H
