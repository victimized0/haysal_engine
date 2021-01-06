#ifndef PLATFORM_INLINE
#define PLATFORM_INLINE
#pragma once

#include "platform.h"
#include "Library.h"
#include "System\ISystem.h"

Environment* gEnv = nullptr;

extern "C" DLL_EXPORT void ModuleInitISystem(ISystem * pSystem, const char* moduleName)
{
	if (gEnv) // Already registered.
		return;

	if (pSystem)
		gEnv = pSystem->GetEnv();
}

#if PLATFORM_WINDOWS
std::string FindExecFolder()
{
	char path[MAX_PATH];
	GetModuleFileNameA(nullptr, path, sizeof(path));

	return std::string(path);
}

std::string FindEngineFolder()
{
	std::string engineDir = FindExecFolder();
	size_t pos = engineDir.find("Bin");
	if (pos != std::string::npos)
		engineDir.erase(pos, engineDir.length());
	engineDir += "Engine\\";
	return engineDir;
}

std::string GetRootFolder()
{
	std::string rootDir = FindExecFolder();
	size_t pos = rootDir.find("Bin");
	if (pos != std::string::npos)
		rootDir.erase(pos, rootDir.length());
	return rootDir;
}

bool InitialiseEngine(SystemInitParams& startupParams, bool manualUpdate = false)
{
	//FindRootFolderAndSetAsCurrentWorkingDirectory();

	Library systemLibrary("System");
	if (!systemLibrary.IsLoaded())
	{
		return false;
	}

	auto LoadSystemFunc = (PFNLoadSystemInterface)systemLibrary.GetProcedureAddress("LoadSystem");
	if (LoadSystemFunc(startupParams, manualUpdate) != nullptr)
	{
		if (manualUpdate)
		{
			// Forward ownership to the function caller
			// This is done since the engine loop will be updated outside of this function scope
			// In other cases we would be exiting the engine at this point.
			systemLibrary.ReleaseOwnership();
		}
	}
	else
	{
		return false;
	}

	return true;
}

#else
#error FindExecFolder not implemented
#error FindEngineFolder not implemented
#error GetRootFolder not implemented
#endif

#endif //PLATFORM_INLINE
