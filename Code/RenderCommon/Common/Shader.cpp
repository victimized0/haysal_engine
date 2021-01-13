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
	SAFE_DELETE(m_pDeviceShader);

	if (m_flags & ShaderFlags::SystemShader)	// System shaders shouldn't be freed
		return -1;
	return BaseResource::Release();
}

void* Shader::GetOrCreateDeviceShader()
{
	if (m_pDeviceShader == nullptr)
	{
		HRESULT hr = DeviceFactory::Get().CreateDeviceShader(GetShaderBlob(), m_devShaderType, &m_pDeviceShader);
		if (SUCCEEDED(hr))
			return m_pDeviceShader;
	}
	return m_pDeviceShader;
}

int Shader::ForceRelease()
{
	int refCount = 0;
	m_flags &= ~ShaderFlags::SystemShader;
	refCount = Release();
	assert(refCount == 0);
	return refCount;
}

bool Shader::Reload()
{
	//gRenderer->GetShaderMan()->LoadShader(m_fileName, m_flags, nullptr);
	return true;
}

//std::vector<RenderElement*>* Shader::GetRenderElements(int techniqueId)
//{
//	return nullptr;
//}

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
	return 0;
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

uint32 ShaderItem::PostLoad()
{
	return 0;
}

bool ShaderItem::Update()
{
	return false;
}
