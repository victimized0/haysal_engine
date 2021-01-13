#ifndef FILE_HELPER_H
#define FILE_HELPER_H
#pragma once

#include <fstream>

namespace FileHelper
{
	inline void LoadToBuffer(const char* filepath, std::vector<char>& outBuffer)
	{
		std::ifstream file(filepath);

		file.seekg(0, std::ios::end);
		size_t fileLength = file.tellg();
		file.seekg(0, std::ios::beg);

		outBuffer.resize(fileLength);
		file.read(&outBuffer[0], outBuffer.size());
		file.close();
	}
}

#endif FILE_HELPER_H
