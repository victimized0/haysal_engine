#ifndef INDEXED_MESH_H
#define INDEXED_MESH_H
#pragma once

#include <WorldModule\IMesh.h>

class IndexedMesh
	: public IIndexedMesh
	//, public Mesh
{
public:
											IndexedMesh() = default;
	virtual									~IndexedMesh() = default;

	// Inherited via IIndexedMesh
	virtual void							GetMeshDesc(MeshDesc& desc) const override;
	virtual void							SetMesh(IMesh& other)		override;
	virtual void							UpdateBoundingBox()			override;
	virtual void							Release()					override	{ delete this; }
	virtual AABB							GetBoundingBox()	const	override	{ return m_boundingBox; }
	virtual Vec3*							GetPositionsData()			override	{ return m_positions.data(); }
	virtual std::vector<Vec3>&				GetPositions()				override	{ return m_positions; }
	virtual const std::vector<Vec3>&		GetPositions()		const	override	{ return m_positions; }
	virtual Vec3*							GetNormalsData()			override	{ return m_normals.data(); }
	virtual std::vector<Vec3>&				GetNormals()				override	{ return m_normals; }
	virtual const std::vector<Vec3>&		GetNormals()		const	override	{ return m_normals; }
	virtual Vec4*							GetColoursData()			override	{ return m_colors.data(); }
	virtual std::vector<Vec4>&				GetColours()				override	{ return m_colors; }
	virtual const std::vector<Vec4>&		GetColours()		const	override	{ return m_colors; }
	virtual Vec2*							GetTexCoordsData()			override	{ return m_texCoords.data(); }
	virtual std::vector<Vec2>&				GetTexCoords()				override	{ return m_texCoords; }
	virtual const std::vector<Vec2>&		GetTexCoords()		const	override	{ return m_texCoords; }
	virtual uint32*							GetIndicesData()			override	{ return m_indices.data(); }
	virtual std::vector<uint32>&			GetIndices()				override	{ return m_indices; }
	virtual const std::vector<uint32>&		GetIndices()		const	override	{ return m_indices; }
	virtual MeshFace*						GetFacesData()				override	{ return m_faces.data(); }
	virtual std::vector<MeshFace>&			GetFaces()					override	{ return m_faces; }
	virtual const std::vector<MeshFace>&	GetFaces()			const	override	{ return m_faces; }
	virtual uint32							GetFacesCount()		const	override	{ return m_faces.size(); }
	virtual uint32							GetVerticesCount()	const	override	{ return m_positions.size(); }
	virtual uint32							GetTexCoordCount()	const	override	{ return m_texCoords.size(); }
	virtual uint32							GetIndicesCount()	const	override	{ return m_indices.size(); }
	virtual uint32							GetNormalsCount()	const	override	{ return m_normals.size(); }
	virtual uint32							GetColoursCount()	const	override	{ return m_colors.size(); }
	// ~Inherited via IIndexedMesh

public:
	std::vector<MeshFace>					m_faces;
	std::vector<Vec4>						m_colors;
	std::vector<Vec3>						m_positions;
	std::vector<Vec3>						m_normals;
	std::vector<Vec2>						m_texCoords;
	std::vector<uint32>						m_indices;
	AABB									m_boundingBox;

};

#endif //INDEXED_MESH_H

