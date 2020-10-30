#ifndef INTERFACE_WORLD_OBJECT_H
#define INTERFACE_WORLD_OBJECT_H
#pragma once

struct IIndexedMesh;
struct IMaterial;

enum class SubObjType
{
	Mesh,
	Point,
	Dummy,
	Camera,
	Light,
};

struct IWorldObj
{
	struct SubObj
	{
		SubObj() {}

		SubObjType				Type;
		std::string				Name;
		IWorldObj*				pParent;
		Matrix					WorldMat;
		Matrix					LocalMat;
		IWorldObj*				pWorldObj;
		unsigned int			IsIdentity	: 1;
		unsigned int			IsHidden	: 1;
	};
	struct Statistics
	{
		int  VerticesCount;
		int  IndicesCount;
		int  MeshSize;
		int  DrawCallsCount;
		int  SubMeshesCount;
		int  RefsNum;

		Statistics() { Reset(); }

		void Reset()
		{
			VerticesCount	= 0;
			IndicesCount	= 0;
			MeshSize		= 0;
			DrawCallsCount	= 0;
			SubMeshesCount	= 0;
			RefsNum			= 0;
		}
	};

	virtual					~IWorldObj() {}

	virtual const char*		GetFilePath() = 0;
	virtual void			SetFilePath(const char* filename) = 0;
	virtual const char*		GetGeometryName() = 0;
	virtual void			SetGeometryName(const char* name) = 0;

	virtual void			Refresh(uint32 flags) = 0;
	virtual void			Invalidate(bool bPhysics = false) = 0;

	virtual void			SetFlags(uint32 flags) = 0;
	virtual uint32			GetFlags() const = 0;

	virtual IIndexedMesh*	GetIndexedMesh(bool createIfNone = false) = 0;
	virtual IIndexedMesh*	CreateIndexedMesh() = 0;
	virtual void			FreeIndexedMesh() = 0;

	virtual IWorldObj*		UpdateVertices(std::vector<Vec3>& pVertices, std::vector<Vec3>&& pNormals, int iVtx0, int nVtx) = 0;
	virtual IWorldObj*		SkinVertices(std::vector<Vec3>& pSkelVtx, const Matrix& mtxSkelToMesh) = 0;

	// TODO: Add physics
	//virtual void			SetPhysGeom(phys_geometry* pPhysGeom, int type = 0) = 0;
	//virtual int				Physicalize(IPhysicalEntity* pent, pe_geomparams* pgp, int id = -1, const char* szPropsOverride = 0) = 0;
	virtual bool			GetPhysProps(float& mass, float& density) = 0;

	//virtual bool			RayIntersection(SRayHitInfo& hitInfo, IMaterial* pCustomMtl = 0) = 0;
	// ~TODO: Add physics

	virtual void			SetMaterial(IMaterial* pMaterial) = 0;

	inline Vec3				GetAABBMin() const { return GetAABB().min; }
	inline Vec3				GetAABBMax() const { return GetAABB().max; }
	virtual void			SetAABBMin(const Vec3& min) = 0;
	virtual void			SetBBoxMax(const Vec3& max) = 0;

	virtual int				GetSubObjectCount() const = 0;
	virtual void			SetSubObjectCount(int count) = 0;

	virtual SubObj*			GetSubObject(int index) = 0;
	virtual bool			IsSubObject() const = 0;
	virtual IWorldObj*		GetParentObject() const = 0;
	virtual SubObj*			FindSubObject(const char* name) = 0;
	virtual bool			RemoveSubObject(int index) = 0;
	virtual SubObj&			AddSubObject(IWorldObj* pObj) = 0;


	//virtual void			DebugDraw(const GeometryDebugDrawInfo& info) = 0;

	virtual void			GetStatistics(Statistics& stats) = 0;
};

#endif //INTERFACE_WORLD_OBJECT_H
