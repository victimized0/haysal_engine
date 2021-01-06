#include "StdAfx.h"
#include <platform.inl>
#include <System\SystemInitParams.h>

// Advise notebook graphics drivers to prefer discrete GPU when no explicit application profile exists
extern "C"
{
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SystemInitParams startupParams = {};
    //startupParams.sLogFileName = "Game.log";

    //std::string cmdLine = GetCommandLine();
    //strcpy(startupParams.systemCmdLine, cmdLine.c_str());

    return InitialiseEngine(startupParams) ? EXIT_SUCCESS : EXIT_FAILURE;
}