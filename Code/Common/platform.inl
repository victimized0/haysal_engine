#ifndef PLATFORM_INLINE
#define PLATFORM_INLINE
#pragma once

#include "platform.h"
#include "ISystem.h"
#include "Library.h"

Environment* gEnv = nullptr;

bool InitialiseEngine(SystemInitParams& startupParams)
{
	//FindRootFolderAndSetAsCurrentWorkingDirectory();

	Library systemLibrary("System");
	if (!systemLibrary.IsLoaded())
	{
		return false;
	}

	PFNLoadSystemInterface LoadSystem = (PFNLoadSystemInterface)systemLibrary.GetProcedureAddress("LoadSystem");

	if (LoadSystem(startupParams) != nullptr)
	{
		//if (bManualEngineLoop)
		//{
			// Forward ownership to the function caller
			// This is done since the engine loop will be updated outside of this function scope
			// In other cases we would be exiting the engine at this point.
			//systemLibrary.ReleaseOwnership();
		//}
	}
	else
	{
		return false;
	}

	return true;
}

extern "C" DLL_EXPORT void ModuleInitISystem(ISystem * pSystem, const char* moduleName)
{
	if (gEnv) // Already registered.
		return;

	if (pSystem)
		gEnv = pSystem->GetEnv();
}

#endif //PLATFORM_INLINE
