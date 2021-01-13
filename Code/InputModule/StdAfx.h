#ifndef INPUT_STDAFX_H
#define INPUT_STDAFX_H
#pragma once

#include <platform.h>
#include <wrl/client.h>

// Helper smart-pointers
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN10) || (defined(_XBOX_ONE) && defined(_TITLE)) || !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
struct virtual_deleter { void operator()(void* p) noexcept { if (p) VirtualFree(p, 0, MEM_RELEASE); } };
#endif

struct aligned_deleter { void operator()(void* p) noexcept { _aligned_free(p); } };

struct handle_closer { void operator()(HANDLE h) noexcept { if (h) CloseHandle(h); } };

using ScopedHandle = std::unique_ptr<void, handle_closer>;

inline HANDLE safe_handle(HANDLE h) noexcept { return (h == INVALID_HANDLE_VALUE) ? nullptr : h; }

#endif //INPUT_STDAFX_H
