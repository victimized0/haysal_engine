#ifndef LIBRARY_H
#define LIBRARY_H
#pragma once

#include "PlatformDefines.h"

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
		return ::GetProcAddress((HMODULE)(m_hModule), procName);
	}

private:
	HMODULE		m_hModule;
};

#endif //LIBRARY_H
