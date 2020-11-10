#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H
#pragma once

class ShaderManager
{
public:
					ShaderManager();
					~ShaderManager();

	void			Init();
	void			ShutDown();

public:
	static Shader*	s_DefaultShader;
	static Shader*	s_HDRPostProcess;

private:


};

#endif //SHADER_MANAGER_H
