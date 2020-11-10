#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H
#pragma once

#include "WorldObject.h"

class ObjectManager
{
public:
				ObjectManager();
				~ObjectManager();

	void		LoadObject(const char* filename);
	void		UnloadObject(WorldObject* pObj);

	void		BeginOcclusionCulling();
	void		FinishOcclusionCulling();
	void		TestOcclusion(const AABB& aabb);

	IWorldObj*	FindStatObj(int id);
	IWorldObj*	FindStatObj(const char* filename);
	bool		GetStatObjAABB(int type, AABB* ppAABB);

private:
	void		DeleteObject(WorldObject* pObj);

private:
	std::map<std::string, WorldObject*>	m_objectsMap;
	std::vector<WorldObject*>			m_objects;

};

#endif //OBJECT_MANAGER_H
