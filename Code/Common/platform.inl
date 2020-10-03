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

#endif //PLATFORM_INLINE
