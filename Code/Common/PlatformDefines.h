#ifndef PLATFORM_DEFINES_H
#define PLATFORM_DEFINES_H
#pragma once

#if defined(_DURANGO) || defined(_XBOX_ONE)
#define PLATFORM_CONSOLE 1
#define PLATFORM_DURANGO 1
#define PLATFORM_WINAPI  1
#elif defined(_ORBIS) || defined(__ORBIS__)
#define PLATFORM_CONSOLE 1
#define PLATFORM_ORBIS   1
#define PLATFORM_POSIX   1
#elif defined(_WIN32)
#define PLATFORM_DESKTOP 1
#define PLATFORM_WINDOWS 1
#define PLATFORM_WINAPI  1
#elif defined(__linux__) || defined(__linux)
#define PLATFORM_DESKTOP 1
#define PLATFORM_LINUX   1
#define PLATFORM_POSIX   1
#endif

#endif //PLATFORM_DEFINES_H
