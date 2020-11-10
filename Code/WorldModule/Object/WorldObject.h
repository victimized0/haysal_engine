#ifndef WORLD_OBJECT_H
#define WORLD_OBJECT_H
#pragma once

#include <WorldModule\IWorldObj.h>

class IndexedMesh;

class WorldObject final : public IWorldObj
{
public:
									WorldObject();
									~WorldObject();

	void							Init();
	void							ShutDown();

	int								AddRef() final;
	int								Release() final;
	int								GetRefCount() const final { return m_refCount; }

	inline SubObj&					GetSubObj(int index) { return m_subObjects[index]; }

	int								GetSubObjectsCount() const final { return m_subObjects.size(); }
	void							SetSubObjectsCount(int count) final;
	IWorldObj*						GetParentObject() const final { return m_pParent; }
	SubObj*							GetSubObject(int index) final;
	SubObj*							FindSubObject(const char* nodeName) final;
	SubObj&							AddSubObject(IWorldObj* pObj) final;
	bool							RemoveSubObject(int index) final;
	bool							IsSubObject() const final { return m_isSubObj; }

	bool							IsUnloadable() const final { return m_canUnload; }
	void							SetFlags(uint32 flags) final { m_flags = flags; }
	uint32							GetFlags() const final { return m_flags; }

	const char*						GetFilePath() final { return m_fileName.c_str(); }
	void							SetFilePath(const char* szFileName) final { m_fileName = szFileName; }
	const char*						GetGeometryName() final { return m_geometryName.c_str(); }
	void							SetGeometryName(const char* szGeoName) final { m_geometryName = szGeoName; }

	IIndexedMesh*					GetIndexedMesh(bool createfNone = false) final;
	IIndexedMesh*					CreateIndexedMesh() final;
	void							ReleaseIndexedMesh() final { SAFE_RELEASE(m_pIndexedMesh); }
	IRenderMesh*					GetRenderMesh() const final { return m_pRenderMesh.get(); }
	void							SetRenderMesh(std::unique_ptr<IRenderMesh>&& pRenderMesh);

	void							SetMaterial(IMaterial* pMaterial) final { m_pMaterial.reset(pMaterial); }
	IMaterial*						GetMaterial() const final { return m_pMaterial.get(); }

	bool							Load(const char* filepath);
	void							Refresh(uint32 flags) final;
	void							Invalidate(bool bPhysics = false) final;

	void							Render(const RenderParams& params, const RenderPassInfo& passInfo) final;

	AABB							GetBoundingBox() const final { return m_boundingBox; }

	int								GetLoadedTrisCount() { return m_loadedTrisCount; }
	int								GetLoadedVertsCount() { return m_loadedVertsCount; }

	IWorldObj*						UpdateVertices(std::vector<Vec3>& pVertices, std::vector<Vec3>&& pNormals, int iVtx0, int nVtx) final;
	IWorldObj*						SkinVertices(std::vector<Vec3>& pSkelVtx, const Matrix& mtxSkelToMesh) final;

	void							DebugDraw() final;
	void							GetStatistics(Statistics& stats) final;

	// TODO: Physics
	// PhysGeo*						GetPhysGeom()
	// void							SetPhysGeom()
	//IPhysEntity*					GetPhysEntity() const final { return nullptr; }
	//bool							HasPhysics() const final;
	//int							Physicalize(IPhysEntity* pent, pe_geomparams* pgp, int id = -1, const char* szPropsOverride = 0) final;
	// ~Physics

private:
	std::vector<SubObj>				m_subObjects;

	std::shared_ptr<IMaterial>		m_pMaterial;
	std::unique_ptr<IRenderMesh>	m_pRenderMesh;
	std::unique_ptr<IndexedMesh>	m_pIndexedMesh;
	WorldObject*					m_pParent;

	std::string						m_fileName;
	std::string						m_geometryName;

	AABB							m_boundingBox;
	uint32							m_flags;

	int								m_loadedTrisCount;
	int								m_loadedVertsCount;
	int								m_refCount;

	bool							m_isSubObj : 1;
	bool							m_canUnload : 1;

	// TODO: Implement physics
	//PhysGeomArray					m_physGeomInfo;
	//float							m_physMass;
	//float							m_physDensity;
	// ~Implement physics
};

#endif //WORLD_OBJECT_H
