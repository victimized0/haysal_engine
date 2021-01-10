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

inline std::string& ToLower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return str;
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

namespace String {
	inline void split(std::vector<std::string> &outVec, const std::string& srcStr, const std::string& delim)
	{
		assert(delim.size() > 0);

		size_t start = 0, end = 0;
		while (end != std::string::npos)
		{
			end = srcStr.find(delim, start);

			size_t pos = end == std::string::npos ? std::string::npos : end - start;
			outVec.push_back(srcStr.substr(start, pos));

			start = end > (std::string::npos - delim.size()) ? std::string::npos : end + delim.size();
		}
	}
}

#endif //STRING_UTILS_H
