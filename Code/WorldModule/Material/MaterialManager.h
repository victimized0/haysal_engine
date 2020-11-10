#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H
#pragma once

#include <WorldModule\IMaterial.h>

class MaterialManager : public IMaterialManager
{
public:
				MaterialManager();
	virtual		~MaterialManager();

	// Inherited via IMaterialManager
	IMaterial*	FindMaterial(const char* name) const override;
	IMaterial*	CreateMaterial(const char* name, uint32 flags = 0) override;
	void		RenameMaterial(IMaterial* pMtl, const char* sNewName) override;
	IMaterial*	LoadMaterial(const char* name, bool makeIfNotFound = true, uint32 loadFlags = 0) override;
	//IMaterial*	LoadMaterialFromXml(const char* name) override;
	IMaterial*	GetDefaultMaterial() override;
	void		GetLoadedMaterials(IMaterial** pData, uint32& nObjCount) const override;
	void		RefreshMaterials() override;

private:
	std::map<std::string, IMaterial*>	m_mtlsMap;


};

#endif //MATERIAL_MANAGER_H
