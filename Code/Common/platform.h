#ifndef PLATFORM_H
#define PLATFORM_H
#pragma once

#include "PlatformDefines.h"

#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)

#ifdef PLATFORM_WINDOWS

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif //WIN32_LEAN_AND_MEAN

#define NOMINMAX

#define LoadLib(libName) ::LoadLibraryA(libName)
#define FreeLib(libHandle) ::FreeLibrary((HMODULE)(libHandle))

#include <windows.h>

#endif //PLATFORM_WINDOWS

#endif //PLATFORM_H