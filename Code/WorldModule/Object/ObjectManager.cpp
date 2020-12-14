#include "stdafx.h"
#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::RenderObjects(IRenderView* pIRndView)
{
	for (IWorldObj* pObj : m_objects)
	{
		if (pObj->GetMaterial()->GetFlags() & MaterialFlags::DontRender)
			continue;	// Do not render objects flagged with do not render :)

		Matrix worldMat = pObj->GetWorldMatrix();
		if (IWorldObj* pParent = pObj->GetParent())
			worldMat = worldMat * pParent->GetWorldMatrix();

		RenderParams params;
		params.pMaterial	= pObj->GetMaterial();
		params.Matrix		= worldMat;
		//params.pRenderNode = pObj->GetRenderNode();

		pObj->Render(params, pIRndView);
	}
}

void ObjectManager::LoadObject(const char* filename)
{
	auto pObj = std::make_unique<WorldObject>();
	pObj->Load(filename);

	auto name = Path::GetNameWithoutExt(filename);
	pObj->SetGeometryName(name.c_str());

	m_objectsMap[name.c_str()] = std::move(pObj);
	m_objects.push_back(pObj.get());
}

void ObjectManager::UnloadObject(WorldObject* pObj)
{
	// TODO: What should even happen here??
}

void ObjectManager::DeleteObject(WorldObject* pObj)
{
	auto it = std::find_if(m_objectsMap.begin(), m_objectsMap.end(), [&](std::unique_ptr<WorldObject> const& pObject) -> bool { return pObject.get() == pObj; });
	if (it != m_objectsMap.end())
		m_objectsMap.erase(it);

	auto it2 = std::find_if(m_objects.begin(), m_objects.end(), [&](WorldObject* pObject) -> bool { return pObject == pObj; });
	if (it2 != m_objects.end())
		m_objects.erase(it2);
}

void ObjectManager::DoFrustumCulling(const Camera* pCamera)
{
	for (WorldObject* pObj : m_objects)
	{
		if (!pCamera->IsInsideFrustum(pObj->GetBoundingBox()))
			pObj->GetMaterial()->SetFlags(MaterialFlags::DontRender);
			//m_frustumObjects.push_back(pObj);
	}
}

void ObjectManager::EndFrame()
{
	for (WorldObject* pObj : m_objects)
	{
		pObj->GetMaterial()->ResetFlags(MaterialFlags::DontRender);
	}
}

void ObjectManager::BeginOcclusionCulling()
{
	// TODO: Put this stuff in a worker thread
	//IOcclusionQuery* pQuery = gEnv->pRenderer->CreateQuery(QueryType::Occlusion);
	//
	//for (WorldObject* pObj : m_frustumObjects) {
	//	gEnv->pRenderer->DoQuery(pQuery);
	//}
}

void ObjectManager::FinishOcclusionCulling()
{
	// TODO: Retrieve query results
}

void ObjectManager::TestOcclusion(const AABB& aabb)
{
	// TODO: Check how many pixels are visible within the given bounding box
	// If num visible pixels > threshold then return true
	// Otherwise return false
}

IWorldObj* ObjectManager::FindObject(int id)
{
	if (id < 0 || id > m_objects.size() - 1)
		return nullptr;
	return m_objects[id];
}

IWorldObj* ObjectManager::FindObject(const char* filename)
{
	auto it = m_objectsMap.find(filename);
	if (it != m_objectsMap.end())
		return it->second.get();
	return nullptr;
}
