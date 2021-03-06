#ifndef INTERFACE_MATERIAL_H
#define INTERFACE_MATERIAL_H
#pragma once

#include <RenderModule\IShader.h>

struct ShaderItem;

enum MaterialFlags : uint8
{
	IsWireFrame	= BIT8(0),
	IsTwoSided	= BIT8(1),
	DontRender	= BIT8(2),
	CastShadows = BIT8(3),
	Unlit		= BIT8(4),
	LightSource	= BIT8(5),
	NoDepth		= BIT8(6),
};

struct IMaterial
{
	virtual						~IMaterial() {}

	virtual void				SetName(const char* name) = 0;
	virtual const char*			GetName() const = 0;

	virtual void				SetFlags(uint32 flags) = 0;
	virtual uint32				GetFlags() const = 0;
	virtual void				ResetFlags(uint32 flags) = 0;

	virtual bool				IsDefault() const = 0;

	virtual void				SetShaderItem(const ShaderItem& item) = 0;
	virtual ShaderItem&			GetShaderItem() = 0;
	virtual const ShaderItem&	GetShaderItem() const = 0;

	virtual ShaderItem&			GetShaderItem(int submtlSlotNum) = 0;
	virtual const ShaderItem&	GetShaderItem(int submtlSlotNum) const = 0;

	virtual bool				SetGetMaterialParamFloat(const char* sParamName, float& v, bool bGet) = 0;
	virtual bool				SetGetMaterialParamVec3(const char* sParamName, Vec3& v, bool bGet) = 0;
	virtual void				SetTexture(int textureId, int textureSlot = static_cast<int>(ShaderResourceTextureSlot::Diffuse)) = 0;
	virtual void				SetSubTexture(int textureId, int subMaterialSlot, int textureSlot = static_cast<int>(ShaderResourceTextureSlot::Diffuse)) = 0;
};

struct IMaterialManager
{
	virtual						~IMaterialManager() {}

	virtual IMaterial*			FindMaterial(const char* name) const = 0;
	virtual IMaterial*			CreateMaterial(const char* name, uint32 flags = 0) = 0;
	virtual void				RenameMaterial(IMaterial* pMtl, const char* newName) = 0;
	virtual IMaterial*			LoadMaterial(const char* name, bool makeIfNotFound = true, uint32 loadFlags = 0) = 0;
	//virtual IMaterial*			LoadMaterialFromXml(const char* sMtlName) = 0;
	virtual IMaterial*			GetDefaultMaterial() = 0;

	virtual void				GetLoadedMaterials(IMaterial** pData, uint32& nObjCount) const = 0;
	virtual void				RefreshMaterials() = 0;
};

#endif //INTERFACE_MATERIAL_H
