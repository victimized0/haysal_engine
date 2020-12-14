#ifndef FILE_HELPER_H
#define FILE_HELPER_H
#pragma once

#include <fstream>

namespace FileHelper
{
	inline void LoadToBuffer(const char* filepath, char* pOutBuffer, size_t* pOutSize)
	{
		std::vector<char> buffer;
		std::ifstream file(filepath);

		file.seekg(0, std::ios::end);
		size_t fileLength = file.tellg();
		file.seekg(0, std::ios::beg);

		buffer.reserve(fileLength);
		file.read(&buffer[0], sizeof(char) * fileLength);

		std::copy(buffer.begin(), buffer.end(), pOutBuffer);
		*pOutSize = fileLength;

		file.close();
	}
}

#endif FILE_HELPER_H
