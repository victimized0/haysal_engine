#include "StdAfx.h"
#include "System.h"
#include <platform.inl>
#include "System\SystemInitParams.h"

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C" SYSTEM_API ISystem* LoadSystem(SystemInitParams& startupParams)
{
	std::unique_ptr<System> pSystem;
	{
		pSystem = std::make_unique<System>(startupParams);
		startupParams.pSystem = pSystem.get();
		ModuleInitISystem(pSystem.get(), "System");

		if (!pSystem->Initialise(startupParams))
		{
			pSystem.release();
			startupParams.pSystem = nullptr;
			gEnv->pSystem = nullptr;
			return nullptr;
		}
	}

	// run main loop
	pSystem->RunMainLoop();
	return nullptr;
};
