#include "StdAfx.h"
#include "ShaderManager.h"
#include <Utils\CRC.h>
#include <sstream>

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
	ShutDown();
}

void ShaderManager::Init()
{
	m_srcShadersDir		= Path::AppendPath(gEnv->pSystem->GetEngineDir(), "Shaders\\hlsl");
	m_shaderCacheDir	= Path::AppendPath(gEnv->pSystem->GetDataDir(), "Shaders\\");
	m_shaderCacheFile	= Path::AppendPath(m_shaderCacheDir, "shaders_cache.txt");

	ParseShaderCacheFile();
}

void ShaderManager::ShutDown()
{
	m_shaderCache.clear();
}

HRESULT ShaderManager::CompileShader(const ShaderInfo& info, ShaderBlob* pOutBlob)
{
	HRESULT hr = S_OK;
	GpuBlob* shaderBlob = nullptr;
	GpuBlob* errorBlob = nullptr;

	uint32 compileFlags = 0;
#ifndef RELEASE
	compileFlags |= (D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION);
#endif
	
	std::string srcFile		= Path::AppendPath(m_srcShadersDir, info.ShaderName);
	std::wstring filepath	= ToWString(srcFile.c_str());
	std::string csoPath		= Path::AppendPath(m_shaderCacheDir, info.ShaderName);
	std::wstring savePath	= ToWString(csoPath.c_str());

	hr = D3DCompileFromFile(filepath.c_str(),
							info.Macros.data(), D3D_COMPILE_STANDARD_FILE_INCLUDE,
							info.EntryPoint.c_str(), info.Target.c_str(),
							compileFlags, 0,
							&shaderBlob, &errorBlob
	);

	if (FAILED(hr))
	{
		if (errorBlob)
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());

		SAFE_RELEASE(errorBlob);
		SAFE_RELEASE(shaderBlob);
	}
	else
	{
		hr = D3DWriteBlobToFile(shaderBlob, savePath.c_str(), TRUE);
		assert(hr == S_OK);
		if (SUCCEEDED(hr))
		{
			pOutBlob->m_pData = static_cast<GpuBlob*>(shaderBlob);
			SaveShaderCacheFile();
		}
	}

	SAFE_RELEASE(errorBlob);
	return hr;
}

void ShaderManager::LoadShader(const char* filename, Shader* pOutShader)
{
	if (filename == nullptr)
		return;

	GpuBlob* pOutBlob = nullptr;

	std::string shaderName  = Path::GetNameFromPath(filename);					// Passed filename should already be a shader name but just in case.
	std::string csoFilepath = Path::AppendPath(m_shaderCacheDir, shaderName);
	std::string srcFilepath = Path::AppendPath(m_srcShadersDir, shaderName);

	auto it = m_shaderCache.find("shaderName");
	if (it != m_shaderCache.end())												// Check whether a compiled shader already exists
	{
		uint32 srcChecksum = CalculateShaderCRC(srcFilepath.c_str());
		if (srcChecksum == it->second.CRC32)									// Compare whether a compiled shader checksum equals to the loading shader
		{																		// If source shader code didn't change, we just load the .cso shader into blob
			std::wstring widePath = ToWString(csoFilepath.c_str());
			HRESULT hr = D3DReadFileToBlob(widePath.c_str(), &pOutBlob);		// TODO: When and where will the blob get Released()? A potential memory leak!!!
		}
		else																	// Else, if shader source code did change or there was any mistake - recompile the .cso shader
		{
			CompileShader(it->second, pOutShader->GetShaderBlob());
		}
	}
#ifndef RELEASE																	// Only available in dev and test builds
	else																		// If compiled shader doesn't exist or if checksum doesn't match, compile the shader at runtime
	{
		CompileShader(pOutShader->GetInfo(), pOutShader->GetShaderBlob());
	}
#endif
}

void ShaderManager::PreloadBinaryShaders()
{
	ParseShaderCacheFile();
}

void ShaderManager::LoadSystemShaders()
{
}

uint32 ShaderManager::CalculateShaderCRC(const char* shaderPath)
{
	char* pBufferData = nullptr; size_t size;
	FileHelper::LoadToBuffer(shaderPath, pBufferData, &size);
	return pBufferData ? CRC::Calculate(pBufferData, size, CRC::CRC_32()) : 0;
}

void ShaderManager::ParseShaderCacheFile()
{
	assert(m_shaderCacheFile.size() == 0);					// It's supposed that we load shader cache file into memory only once?
	std::string line;
	std::ifstream shaderCache(m_shaderCacheFile.c_str());

	while (std::getline(shaderCache, line)) {
		std::vector<std::string> header;
		String::split(header, line, ":");

		if (header.size() < 5)
			continue; // Invalid entry

		ShaderInfo info;
		info.CRC32		= std::stoi(header[0]);
		info.ShaderName = header[1];
		info.EntryPoint = header[2];
		info.Target		= header[3];

		std::vector<std::string> macros;
		String::split(macros, header[4], ";");
		
		for (auto& macro : macros)
		{
			std::vector<std::string> kv;
			String::split(kv, macro, "=");
			if (kv.size() == 1)
				kv.push_back("1");
			info.Macros.push_back({ kv[0].c_str(), kv[1].c_str() });
		}
		info.Macros.push_back({ NULL, NULL });

		m_shaderCache[info.ShaderName] = info;
	}

	shaderCache.close();
}

void ShaderManager::SaveShaderCacheFile()
{
	std::vector<std::string> shaderHeaders;
	std::ofstream shaderCache(m_shaderCacheFile.c_str());
	std::ostream_iterator<std::string> oit(shaderCache, "\n");

	for (auto& shader : m_shaderCache)
	{
		std::ostringstream stringHeader;
		stringHeader << std::to_string(shader.second.CRC32) << ":" << shader.second.ShaderName << ":" << shader.second.EntryPoint << ":" << shader.second.Target << ":";

		for (auto& macro : shader.second.Macros)
			stringHeader << macro.Name << "=" << macro.Definition << ";";

		shaderHeaders.push_back( stringHeader.str() );
	}

	std::copy(shaderHeaders.begin(), shaderHeaders.end(), oit);
}
