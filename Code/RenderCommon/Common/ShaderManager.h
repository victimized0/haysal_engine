#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H
#pragma once

class Shader;
struct ShaderBlob;
struct ShaderInfo;

class ShaderManager
{
public:
					ShaderManager();
					~ShaderManager();

	void			Init();
	void			ShutDown();

	HRESULT			CompileShader(const ShaderInfo& info, ShaderBlob* pOutBlob);
	void			LoadShader(const char* filename, Shader* pOutShader);

	void			PreloadBinaryShaders();
	void			LoadSystemShaders();

private:
	uint32			CalculateShaderCRC(const char* shaderPath);
	void			LoadShaderCombinations();
	void			SaveShaderCombinations();

public:
	static Shader*	s_DefaultVS;
	static Shader*	s_DefaultPS;
	static Shader*	s_HDRPostProcess;

private:
	std::map<std::string, ShaderInfo> m_shaderCache;
	std::string		m_shaderCacheFile;
	std::string		m_srcShadersDir;
	std::string		m_shaderCacheDir;

};

#endif //SHADER_MANAGER_H
