#ifndef LIBRARY_H
#define LIBRARY_H
#pragma once

#include "PlatformDefines.h"

typedef void* WIN_HMODULE;

#if defined(PLATFORM_WINDOWS) || defined(PLATFORM_DURANGO)
	#if PLATFORM_WINDOWS
		#define LoadLib(libName) ::LoadLibraryA(libName)
	#elif PLATFORM_DURANGO
		#define LoadLib(libName) ::LoadLibraryExA(libName, 0, 0)
	#endif

	#define FreeLib(handle) ::FreeLibrary((HMODULE)(handle))
	#define SafeFreeLib(handle) if(handle) FreeLib(handle)
	#define GetProcAddress(handle, procName) ::GetProcAddress((HMODULE)(handle), procName)
#else
	#define GetProcAddress(handle, procName)
#endif

class Library
{
public:
	Library(const char* path) : m_hModule(nullptr)
	{
		if (path != nullptr)
		{
			m_hModule = LoadLib(path);
		}
	}

	~Library()
	{
		if (m_hModule != nullptr)
		{
			FreeLib(m_hModule);
			m_hModule = nullptr;
		}
	}

				Library(Library const&)				= delete;
				Library(Library&&)					= delete;
	Library&	operator=(Library const&)			= delete;
	Library&	operator=(Library&&)				= delete;

	bool		IsLoaded() const { return m_hModule != nullptr; }

	void* GetProcedureAddress(const char* procName)
	{
		return GetProcAddress(m_hModule, procName);
	}

private:
	WIN_HMODULE m_hModule;

};

#endif //LIBRARY_H
