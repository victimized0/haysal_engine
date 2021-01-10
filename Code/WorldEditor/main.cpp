#include "stdafx.h"
#include <platform.inl>
#include <System\SystemInitParams.h>
#include "WorldEditor.h"
#include <QtWidgets/QApplication>
#include <RenderModule\IRenderer.h>

// Advise notebook graphics drivers to prefer discrete GPU when no explicit application profile exists
extern "C"
{
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
}

int main(int argc, char *argv[])
{
	// Enable dev mode

	SystemInitParams startupParams = {};
    startupParams.IsEditor = true;
    if (!InitialiseEngine(startupParams, true))
    {
        return EXIT_FAILURE;
    }

    QApplication a(argc, argv);
    WorldEditor w;
    w.show();

    //gEnv->pRenderer->SetHWND((HWND)w.winId());

    return a.exec();
}
