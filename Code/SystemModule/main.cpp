#include "StdAfx.h"
#include "System.h"

extern "C"
{
	SYSTEM_API ISystem* LoadSystem(SystemInitParams& startupParams)
	{
		std::unique_ptr<System> pSystem;
		{
			pSystem = std::make_unique<System>(startupParams);
			startupParams.pSystem = pSystem.get();

			if (!pSystem->Initialise(startupParams))
			{
				pSystem.release();
				startupParams.pSystem = nullptr;
				//gEnv->pSystem = nullptr;
				return nullptr;
			}
		}

		// run main loop
		pSystem->RunMainLoop();
		return nullptr;
	}
};
