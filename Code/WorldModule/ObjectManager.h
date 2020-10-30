#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H
#pragma once

class ObjectManager
{
public:
				ObjectManager();
				~ObjectManager();

	void		LoadObject(const char* filename);
	void		UnloadObject(CStatObj* pObj);

	void		BeginOcclusionCulling();
	void		FinishOcclusionCulling();
	void		TestOcclusion(const AABB& aabb);

	IStatObj*	FindStatObj(int id);
	IStatObj*	FindStatObj(const char* filename);
	bool		GetStatObjAABB(int type, AABB* ppAABB);

private:
	void		DeleteObject(CStatObj* pObj);

private:
	std::map<std::string, CStatObj*>	m_statObjMap;
	std::vector<CStatObj*>				m_statObjects;

};

#endif //OBJECT_MANAGER_H
