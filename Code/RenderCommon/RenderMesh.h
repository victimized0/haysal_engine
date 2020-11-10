#ifndef RENDER_MESH_H
#define RENDER_MESH_H
#pragma once

#include <WorldModule\IMesh.h>
#include <RenderModule\IRenderMesh.h>

class RenderMesh : public IRenderMesh
{
public:
								RenderMesh();
	virtual						~RenderMesh();

	// Inherited via IRenderMesh
	virtual void				AddRef() override;
	virtual int					Release() override;
	virtual bool				CanUpdate() override;
	virtual bool				CanRender() override;
	virtual const char*			GetSourceName() const override;
	virtual int					GetIndicesCount() override;
	virtual int					GetVerticesCount() override;
	virtual VertexFormat		GetVertexFormat() override;
	virtual RenderMeshType		GetMeshType() override;
	virtual size_t				SetMesh(IMesh& mesh, uint32 flags, const Vec3* pPosOffset, bool requiresLock) override;
	virtual IIndexedMesh*		GetIndexedMesh() override;
	virtual bool				UpdateVertices(const void* pVertBuffer, int vertsCount, int offset, bool requiresLock = true) override;
	virtual bool				UpdateIndices(const uint32* pIndBuffer, int indicesCount, int offset, bool requiresLock = true) override;
	virtual void				Render(const RenderPassInfo& passInfo) override;
	// ~Inherited via IRenderMesh

private:
	VertexFormat				m_vertexFormat;
	PrimitiveTopology			m_topology;
	RenderMeshType				m_type;
	uint32						m_flags;
	IBuffer						m_vtxBuffer;
	IBuffer						m_idxBuffer;
	uint32						m_vtxCount;
	uint32						m_idxCount;

};

#endif //RENDER_MESH_H
