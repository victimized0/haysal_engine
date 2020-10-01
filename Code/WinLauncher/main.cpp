#include "StdAfx.h"
#include <platform.inl>
#include "SystemInitParams.h"

// Advise notebook graphics drivers to prefer discrete GPU when no explicit application profile exists
extern "C"
{
    // nVidia
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    // AMD
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

bool InitialiseEngine(SystemInitParams& startupParams)
{
	//FindRootFolderAndSetAsCurrentWorkingDirectory();

	Library systemLibrary("System");
	if (!systemLibrary.IsLoaded())
	{
		return false;
	}

	auto LoadSystemFunc = (PFNLoadSystemInterface)systemLibrary.GetProcedureAddress("LoadSystem");

	if (LoadSystemFunc(startupParams) != nullptr)
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

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    SystemInitParams startupParams;
    //startupParams.sLogFileName = "Game.log";

    //std::string cmdLine = GetCommandLine();
    //strcpy(startupParams.systemCmdLine, cmdLine.c_str());

    return InitialiseEngine(startupParams) ? EXIT_SUCCESS : EXIT_FAILURE;
}