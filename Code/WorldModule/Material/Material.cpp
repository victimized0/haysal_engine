#include "stdafx.h"
#include "Material.h"

Material::Material()
    : m_flags(0)
{
}

Material::~Material()
{
}

bool Material::SetGetMaterialParamFloat(const char* sParamName, float& v, bool bGet)
{
    return false;
}

bool Material::SetGetMaterialParamVec3(const char* sParamName, Vec3& v, bool bGet)
{
    return false;
}

void Material::SetTexture(int textureId, int textureSlot = static_cast<int>(ShaderResourceTextureSlot::Diffuse))
{
}

void Material::SetSubTexture(int textureId, int subMaterialSlot, int textureSlot = static_cast<int>(ShaderResourceTextureSlot::Diffuse))
{
}
