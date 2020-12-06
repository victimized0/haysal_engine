#ifndef INTERFACE_MESH_H
#define INTERFACE_MESH_H
#pragma once

#include "..\RenderModule\VertexFormats.h"

struct MeshFace
{
	uint32 X;
	uint32 Y;
	uint32 Z;

	MeshFace(uint32 x, uint32 y, uint32 z)
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

#endif //INTERFACE_MESH_H
