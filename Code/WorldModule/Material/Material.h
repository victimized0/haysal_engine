#ifndef MATERIAL_H
#define MATERIAL_H
#pragma once

#include <WorldModule\IMaterial.h>

class Material final : public IMaterial
{
public:
									Material();
	virtual							~Material();

	// Inherited via IMaterial
	virtual void					SetName(const char* name)				final { m_matName = name; }
	virtual const char*				GetName()						const	final { return m_matName.c_str(); }
	virtual void					SetFlags(uint32 flags)					final { m_flags |= flags; }
	virtual uint32					GetFlags()						const	final { return m_flags; }
	virtual void					ResetFlags(uint32 flags)				final { m_flags &= ~flags; }
	virtual bool					IsDefault()						const	final { assert(false); return false; /*NOT IMPLEMENTED!!!*/ }
	virtual void					SetShaderItem(const ShaderItem& item)	final { m_shaderItem = item; }
	virtual ShaderItem&				GetShaderItem()							final { return m_shaderItem; }
	virtual const ShaderItem&		GetShaderItem()					const	final { return m_shaderItem; }
	virtual ShaderItem&				GetShaderItem(int submtlSlotNum)		final { assert(false); return m_shaderItem; /*NOT IMPLEMENTED!!!*/ }
	virtual const ShaderItem&		GetShaderItem(int submtlSlotNum) const	final { assert(false); return m_shaderItem; /*NOT IMPLEMENTED!!!*/ }

	virtual bool					SetGetMaterialParamFloat(const char* sParamName, float& v, bool bGet) final;
	virtual bool					SetGetMaterialParamVec3(const char* sParamName, Vec3& v, bool bGet) final;
	virtual void					SetTexture(int textureId, int textureSlot = static_cast<int>(ShaderResourceTextureSlot::Diffuse)) final;
	virtual void					SetSubTexture(int textureId, int subMaterialSlot, int textureSlot = static_cast<int>(ShaderResourceTextureSlot::Diffuse)) final;
	// ~Inherited via IMaterial

private:
	std::string						m_matName;
	uint32							m_flags;
	ShaderItem						m_shaderItem;

};

#endif //MATERIAL_H
