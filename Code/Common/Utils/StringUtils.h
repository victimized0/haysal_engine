#ifndef STRING_UTILS_H
#define STRING_UTILS_H
#pragma once

#include <filesystem>

inline std::wstring ToWString(const char* str)
{
	std::string inStr(str);
	std::wstring outStr(inStr.begin(), inStr.end());
	return outStr;
}

inline std::string ToString(const wchar_t* wstr)
{
	std::wstring inStr(wstr);
	std::string outStr(inStr.begin(), inStr.end());
	return outStr;
}

namespace Path
{
	inline std::string GetNameFromPath(const std::string& path)
	{
		std::filesystem::path p(path);
		return p.filename().string();
	}

	inline std::string GetNameWithoutExt(const std::string& path)
	{
		std::filesystem::path p(path);
		return p.stem().string();
	}

	inline std::string GetPathWithoutExt(const std::string& path)
	{
		typename std::string::size_type const p(path.find_last_of('.'));
		return p > 0 && p != std::string::npos ? path.substr(0, p) : path;
	}

	inline std::string GetExtension(const std::string& path)
	{
		std::filesystem::path p(path);
		return p.extension().string();
	}

	inline std::wstring GetNameFromPath(const std::wstring& path)
	{
		std::filesystem::path p(path);
		return p.filename().wstring();
	}

	inline std::wstring GetNameWithoutExt(const std::wstring& path)
	{
		std::filesystem::path p(path);
		return p.stem().wstring();
	}

	inline std::wstring GetPathWithoutExt(const std::wstring& path)
	{
		typename std::wstring::size_type const p(path.find_last_of('.'));
		return p > 0 && p != std::wstring::npos ? path.substr(0, p) : path;
	}

	inline std::wstring GetExtension(const std::wstring& path)
	{
		std::filesystem::path p(path);
		return p.extension().wstring();
	}
}

#endif //STRING_UTILS_H
