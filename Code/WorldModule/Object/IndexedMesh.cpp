#include "stdafx.h"
#include "IndexedMesh.h"

void IndexedMesh::SetMesh(IMesh& other)
{
	m_positions		= other.GetPositions();
	m_normals		= other.GetNormals();
	m_colors		= other.GetColours();
	m_texCoords		= other.GetTexCoords();
	m_indices		= other.GetIndices();
	m_faces			= other.GetFaces();
	m_boundingBox	= other.GetBoundingBox();
}

void IndexedMesh::UpdateBoundingBox()
{
	size_t vertsCount = static_cast<size_t>(GetVerticesCount());
	if (vertsCount == 0 || m_positions.size() == 0)
	{
		m_boundingBox = AABB();
		return;
	}

	AABB::CreateFromPoints(m_boundingBox, vertsCount, const_cast<const Vec3*>(m_positions.data()), sizeof(Vec3));
}

void IndexedMesh::GetMeshDesc(MeshDesc& desc) const
{
    desc.pFaces			= m_faces.data();
	desc.pVertices		= m_positions.data();
	desc.pNormals		= m_normals.data();
	desc.pColors		= m_colors.data();
	desc.pTexCoords		= m_texCoords.data();
	desc.pIndices		= m_indices.data();
	desc.FacesCount		= GetFacesCount();
	desc.VertsCount		= GetVerticesCount();
	desc.TexCoordCount	= GetTexCoordCount();
	desc.IndicesCount	= GetIndicesCount();
}
