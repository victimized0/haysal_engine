#ifndef INTERFACE_MESH_H
#define INTERFACE_MESH_H
#pragma once

#include "..\RenderModule\VertexFormats.h"

struct MeshFace
{
	uint16 X;
	uint16 Y;
	uint16 Z;

	MeshFace()
		: X(0), Y(0), Z(0)
	{}

	MeshFace(uint16 x, uint16 y, uint16 z)
		: X(x), Y(y), Z(z)
	{}
};

struct IMesh
{
	virtual									~IMesh() {}
	virtual void							Release() = 0;
	virtual void							SetMesh(IMesh& other) = 0;
	virtual void							UpdateBoundingBox() = 0;
	virtual AABB							GetBoundingBox() const = 0;

	virtual Vec3*							GetPositionsData() = 0;
	virtual std::vector<Vec3>&				GetPositions() = 0;
	virtual const std::vector<Vec3>&		GetPositions() const = 0;

	virtual Vec3*							GetNormalsData() = 0;
	virtual std::vector<Vec3>&				GetNormals() = 0;
	virtual const std::vector<Vec3>&		GetNormals() const = 0;

	virtual Vec4*							GetColoursData() = 0;
	virtual std::vector<Vec4>&				GetColours() = 0;
	virtual const std::vector<Vec4>&		GetColours() const = 0;

	virtual Vec2*							GetTexCoordsData() = 0;
	virtual std::vector<Vec2>&				GetTexCoords() = 0;
	virtual const std::vector<Vec2>&		GetTexCoords() const = 0;

	virtual uint32*							GetIndicesData() = 0;
	virtual std::vector<uint32>&			GetIndices() = 0;
	virtual const std::vector<uint32>&		GetIndices() const = 0;

	virtual MeshFace*						GetFacesData() = 0;
	virtual std::vector<MeshFace>&			GetFaces() = 0;
	virtual const std::vector<MeshFace>&	GetFaces() const = 0;

	virtual uint32							GetFacesCount() const = 0;
	virtual uint32							GetVerticesCount() const = 0;
	virtual uint32							GetTexCoordCount() const = 0;
	virtual uint32							GetIndicesCount() const = 0;
	virtual uint32							GetNormalsCount() const = 0;
	virtual uint32							GetColoursCount() const = 0;
};

struct IIndexedMesh : public IMesh
{
	struct MeshDesc
	{
		const MeshFace*	pFaces;
		const Vec3*		pVertices;
		const Vec3*		pNormals;
		const Vec4*		pColors;
		const Vec2*		pTexCoords;
		const uint32*	pIndices;
		uint32			FacesCount;
		uint32			VertsCount;
		uint32			TexCoordCount;
		uint32			IndicesCount;
	};

	virtual				~IIndexedMesh() {}
	virtual void		GetMeshDesc(MeshDesc& desc) const = 0;

};

struct ISkinnedMesh : public IMesh
{
	virtual ~ISkinnedMesh() {}
};

//class Mesh
//{
//public:
//	Mesh()
//		: pIndices(nullptr)
//		, pPositions(nullptr)
//		, pNormals(nullptr)
//		, pTexCoords(nullptr)
//		, pColors(nullptr)
//		, IndicesCount(0)
//		, PositionsCount(0)
//		, NormalsCount(0)
//		, TexCoordsCount(0)
//		, ColorsCount(0)
//	{}
//
//	uint32* pIndices;
//	Vec3*	pPositions;
//	Vec3*	pNormals;
//	Vec2*	pTexCoords;
//	Vec3*	pColors;
//	AABB	BBox;
//
//	uint32  IndicesCount;
//	uint32  PositionsCount;
//	uint32  NormalsCount;
//	uint32  TexCoordsCount;
//	uint32  ColorsCount;
//};

#endif //INTERFACE_MESH_H
