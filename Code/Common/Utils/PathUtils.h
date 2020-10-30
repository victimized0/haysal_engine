#ifndef PATH_UTILS_H
#define PATH_UTILS_H
#pragma once

#if PLATFORM_WINDOWS
#define WIN_SLASH '\\'
#define STR_SLASH WIN_SLASH
#elif PLATFORM_POSIX
#define POS_SLASH '/'
#define STR_SLASH POS_SLASH
#endif

namespace Path {
	inline std::string AppendSlash(std::string str)
	{
		const char slash = STR_SLASH;
		size_t lastCharPos = str.length() - 1;
		if (str[lastCharPos] != STR_SLASH)
			str.append(&slash);
		return str;
	}

	inline std::string TrimSlash(std::string str)
	{
		size_t lastCharPos = str.length() - 1;
		if (str[lastCharPos] == STR_SLASH)
			str.erase(lastCharPos);
		return str;
	}

	inline std::string AppendPath(std::string str, std::string pathToAppend)
	{
#if PLATFORM_WINDOWS
		std::replace(pathToAppend.begin(), pathToAppend.end(), '/', WIN_SLASH);
#endif
		str = AppendSlash(str);
		str.append(pathToAppend);
		if (str.find('.') == 0)
			str = AppendSlash(str);
		return str;
	}

	inline std::string TrimPath(std::string str, std::string pathToTrim)
	{
		size_t trimPos = str.find(pathToTrim);
		if (trimPos > 0)
			str.erase(trimPos, str.length());
		str = AppendSlash(str);
		return str;
	}

	inline bool HasPath(const std::string& str, std::string pathToFind)
	{
		size_t findPos = str.find(pathToFind);
		return (findPos > 0);
	}
}

#endif //PATH_UTILS_H
