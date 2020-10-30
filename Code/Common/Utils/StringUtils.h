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

	inline std::string GetPathWithoutExt(std::string const& filename)
	{
		typename std::string::size_type const p(filename.find_last_of('.'));
		return p > 0 && p != std::string::npos ? filename.substr(0, p) : filename;
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

	inline std::wstring GetPathWithoutExt(std::wstring const& filename)
	{
		typename std::wstring::size_type const p(filename.find_last_of('.'));
		return p > 0 && p != std::wstring::npos ? filename.substr(0, p) : filename;
	}
}

#endif //STRING_UTILS_H
