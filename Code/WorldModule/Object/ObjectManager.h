#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H
#pragma once

#include "WorldObject.h"
#include "RenderNodes\MeshRenderNode.h"

//struct WorldObjDeleter {
//	void operator()(WorldObject* pObj) { SAFE_DELETE(pObj); }
//};

typedef std::map<const char*, std::unique_ptr<WorldObject>> WorldObjMap;

class ObjectManager
{
public:
								ObjectManager();
								~ObjectManager();

	void						EndFrame();
	void						RenderObjects(IRenderView* pIRndView);
	void						RenderMeshNode(MeshRenderNode* pNode, IRenderView* pIRndView);

	IWorldObj*					LoadObject(const char* filename, const char* geomName = nullptr);
	void						UnloadObject(WorldObject* pObj);

	void						BeginOcclusionCulling();
	void						FinishOcclusionCulling();
	void						TestOcclusion(const AABB& aabb);

	IWorldObj*					FindObject(int id);
	IWorldObj*					FindObject(const char* filename);

	int							GetLoadedCount() const { return m_objects.size(); }

private:
	void						DoFrustumCulling(const Camera* pCamera);
	void						DeleteObject(WorldObject* pObj);

private:
	WorldObjMap					m_objectsMap;
	std::vector<WorldObject*>	m_objects;
	std::vector<WorldObject*>	m_frustumObjects;

};

#endif //OBJECT_MANAGER_H
