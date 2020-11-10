#include "StdAfx.h"
#include "Shader.h"

Shader::Shader()
	: m_shaderType(ShaderType::General)
	, m_flags(0)
	, m_refreshFrameId(0)
{
	memset(m_textureSlots, 0, sizeof(m_textureSlots));
}

Shader::~Shader()
{

}

int Shader::Release()
{
	if (m_flags & ShaderFlags::System)	// System shaders shouldn't be freed
		return -1;
	return BaseResource::Release();
}

int Shader::ForceRelease()
{
	int refCount = 0;
	m_flags &= ~ShaderFlags::System;
	refCount = Release();
	assert(refCount == 0);
	return refCount;
}

bool Shader::Reload()
{
	return gRenderer->ShaderManager()->LoadShader(m_fileName, m_flags, nullptr);
}

std::vector<RenderElement*>* Shader::GetRenderElements(int techniqueId)
{
	return nullptr;
}

int Shader::GetTexId()
{
	return 0;
}

ITexture* Shader::GetBaseTexture(int* passNum, int* nTU)
{
	return nullptr;
}

uint32 Shader::GetUsedTextureTypes()
{
	return uint32();
}

ShaderTextureSlots* Shader::GetUsedTextureSlots(int techniqueId)
{
	return nullptr;
}

int Shader::GetTechniqueId(int techniqueIndex)
{
	return 0;
}

void Shader::Free()
{
}
