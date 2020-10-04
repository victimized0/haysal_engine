#ifndef PLATFORM_H
#define PLATFORM_H
#pragma once

#include "PlatformDefines.h"

#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)

// Safe memory helpers
#define SAFE_ACQUIRE(p)       { if (p) (p)->AddRef(); }
#define SAFE_DELETE(p)        { if (p) { delete (p);          (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p)  { if (p) { delete[] (p);        (p) = NULL; } }
#define SAFE_RELEASE(p)       { if (p) { (p)->Release();      (p) = NULL; } }
#define SAFE_RELEASE_FORCE(p) { if (p) { (p)->ReleaseForce(); (p) = NULL; } }

#ifdef PLATFORM_WINDOWS

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif //WIN32_LEAN_AND_MEAN

#define NOMINMAX
#define NODRAWTEXT
//#define NOGDI
#define NOBITMAP
#define NOMCX
#define NOSERVICE
#define NOHELP

#include <windows.h>

typedef unsigned long int uint64;
typedef long int int64;
typedef unsigned int uint32;
typedef int int32;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned char byte;
typedef char sbyte;
typedef byte uint8;
typedef sbyte int8;

#endif //PLATFORM_WINDOWS

#if PLATFORM_WINDOWS
//#include <WindowsSpecific.h>
//#pragma comment(lib, "rpcrt4.lib")  // UuidCreate - Minimum supported OS Win 2000

#include "Math\SimpleMath.h"
#endif

#if PLATFORM_LINUX
//#include <LinuxSpecific.h>
#error No uuid generator...
#error No math lib...
#endif

#if PLATFORM_DURANGO
//#include <DurangoSpecific.h>
#error No uuid generator...
#error No math lib...
#endif

#if PLATFORM_ORBIS
//#include <OrbisSpecific.h>
#error No uuid generator...
#error No math lib...
#endif

#define BIT8(x)  ((static_cast<uint8>(1)) << (x))
#define BIT16(x) ((static_cast<uint16>(1)) << (x))
#define BIT32(x) (1u << (x))
#define BIT64(x) (1ull << (x))

#include <algorithm>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>

#endif //PLATFORM_H