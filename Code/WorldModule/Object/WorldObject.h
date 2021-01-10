#ifndef WORLD_OBJECT_H
#define WORLD_OBJECT_H
#pragma once

#include <WorldModule\IWorldObj.h>
#include <RenderModule\IRenderMesh.h>
#include "IndexedMesh.h"

class WorldObject final : public IWorldObj
{
public:
									WorldObject();
	virtual 						~WorldObject();

	void							Init();
	void							ShutDown();

	virtual int						AddRef()									final;
	virtual int						Release()									final;
	virtual int						GetRefCount()						const	final	{ return m_refCount; }

	inline SubObj&					GetSubObj(int index)								{ return m_subObjects[index]; }
	virtual int						GetSubObjectsCount()				const	final	{ return m_subObjects.size(); }
	virtual void					SetSubObjectsCount(int count)				final;
	virtual IWorldObj*				GetParent()							const	final	{ return m_pParent; }
	virtual SubObj*					GetSubObject(int index)						final;
	virtual SubObj*					FindSubObject(const char* nodeName)			final;
	virtual SubObj&					AddSubObject(IWorldObj* pObj)				final;
	virtual bool					RemoveSubObject(int index)					final;
	virtual bool					IsSubObject()						const	final	{ return m_isSubObj; }

	virtual bool					IsUnloadable()						const	final	{ return m_canUnload; }
	virtual void					SetFlags(uint32 flags)						final	{ m_flags |= flags; }
	virtual void					ResetFlags(uint32 flags)					final	{ m_flags &= ~flags; }
	virtual uint32					GetFlags()							const	final	{ return m_flags; }

	virtual const char*				GetFilePath()								final	{ return m_srcFileName.c_str(); }
	virtual void					SetFilePath(const char* szFileName)			final	{ m_srcFileName = szFileName; }
	virtual const char*				GetGeometryName()							final	{ return m_geometryName.c_str(); }
	virtual void					SetGeometryName(const char* szGeoName)		final	{ m_geometryName = szGeoName; }
	
	virtual void					SetIndexedMesh(IndexedMesh* pIndexedMesh)   final   { m_pIndexedMesh.reset(pIndexedMesh);}
	virtual IIndexedMesh*			GetIndexedMesh(bool createfNone = false)	final	{ return m_pIndexedMesh.get(); }
	virtual IIndexedMesh*			CreateIndexedMesh()							final;
	virtual void					ReleaseIndexedMesh()						final	{ SAFE_RELEASE(m_pIndexedMesh); }
	virtual IRenderMesh*			GetRenderMesh()						const	final	{ return m_pRenderMesh.get(); }
	void							SetRenderMesh(IRenderMesh* pRenderMesh);

	virtual void					SetMaterial(IMaterial* pMaterial)			final	{ m_pMaterial.reset(pMaterial); }
	virtual IMaterial*				GetMaterial()						const	final	{ return m_pMaterial.get(); }

	bool							Load(const char* filepath);
	virtual void					Refresh(uint32 flags)						final;
	virtual void					Invalidate(bool bPhysics = false)			final;

	virtual void					Render(const RenderParams& params, IRenderView* pRenderView) final;

	virtual AABB					GetBoundingBox()					const	final	{ return m_boundingBox; }

	int								GetLoadedTrisCount()								{ return m_loadedTrisCount; }
	int								GetLoadedVertsCount()								{ return m_loadedVertsCount; }

	virtual IWorldObj*				UpdateVertices(std::vector<Vec3>& pVertices, std::vector<Vec3>&& pNormals, int iVtx0, int nVtx) final;
	virtual IWorldObj*				SkinVertices(std::vector<Vec3>& pSkelVtx, const Matrix& mtxSkelToMesh) final;

	virtual void					DebugDraw()									final;
	virtual void					GetStatistics(Statistics& stats)			final;

	virtual void					SetMatrix(const Matrix& m)							{ m_worldMat = m; }
	virtual Matrix					GetWorldMatrix()							final	{ return m_worldMat; }

	// TODO: Physics
	// PhysGeo*						GetPhysGeom()
	// void							SetPhysGeom()
	//IPhysEntity*					GetPhysEntity() const final { return nullptr; }
	//virtual bool					GetPhysProps(float& mass, float& density) override;
	//bool							HasPhysics() const final;
	//int							Physicalize(IPhysEntity* pent, pe_geomparams* pgp, int id = -1, const char* szPropsOverride = 0) final;
	// ~Physics

private:
	std::vector<SubObj>				m_subObjects;

	std::shared_ptr<IMaterial>		m_pMaterial;
	std::unique_ptr<IRenderMesh>	m_pRenderMesh;
	std::unique_ptr<IndexedMesh>	m_pIndexedMesh;
	WorldObject*					m_pParent;

	std::string						m_srcFileName;
	std::string						m_geometryName;

	Matrix							m_worldMat;
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
