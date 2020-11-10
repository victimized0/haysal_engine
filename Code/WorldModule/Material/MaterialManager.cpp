#include "stdafx.h"
#include "MaterialManager.h"

MaterialManager::MaterialManager()
{
}

MaterialManager::~MaterialManager()
{
}

IMaterial* MaterialManager::FindMaterial(const char* name) const
{
	return nullptr;
}

IMaterial* MaterialManager::CreateMaterial(const char* name, uint32 flags /*= 0*/)
{
	return nullptr;
}

void MaterialManager::RenameMaterial(IMaterial* pMtl, const char* newName)
{
}

IMaterial* MaterialManager::LoadMaterial(const char* name, bool makeIfNotFound /*= true*/, uint32 loadFlags /*= 0*/)
{
	return nullptr;
}

IMaterial* MaterialManager::GetDefaultMaterial()
{
	return nullptr;
}

void MaterialManager::GetLoadedMaterials(IMaterial** pData, uint32& nObjCount) const
{
}

void MaterialManager::RefreshMaterials()
{
}
