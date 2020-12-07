#ifndef RENDER_MESH_H
#define RENDER_MESH_H
#pragma once

#include <WorldModule\IMesh.h>
#include <RenderModule\IRenderMesh.h>

class RenderMesh : public IRenderMesh
{
public:
								RenderMesh(const char* srcName);
	virtual						~RenderMesh();

	// Inherited via IRenderMesh
	virtual void				AddRef()					final;
	virtual int					Release()					final;
	virtual const char*			GetSourceName()		const	final	{ return m_srcName.c_str(); }
	virtual int					GetIndicesCount()			final	{ return m_idxCount; }
	virtual int					GetVerticesCount()			final	{ return m_vtxCount; }
	virtual VertexFormat		GetVertexFormat()			final	{ return m_vertexFormat; }
	virtual RenderMeshType		GetMeshType()				final	{ return m_type; }

	virtual GpuBuffer*			GetVertexBuffer()			final	{ return m_pVertexBuffer; }
	virtual GpuBuffer*			GetIndexBuffer()			final	{ return m_pIndexBuffer; }

	virtual size_t				SetMesh(IMesh& mesh, uint32 flags, const Vec3* pPosOffset) override;
	virtual IIndexedMesh*		GetIndexedMesh(IIndexedMesh* outMesh) override { return nullptr; }

	virtual void				UpdateVertices(void* pData, int vertsCount, int offset) override;
	virtual void				UpdateIndices(uint32* pData, int indicesCount, int offset) override;

	virtual void				Render(const RenderParams& params, IRenderView* pRenderView) override;
	// ~Inherited via IRenderMesh

private:
	std::vector<VF_PosNmlTex>	m_posNmlTex;
	std::string					m_srcName;
	VertexFormat				m_vertexFormat;
	PrimitiveTopology			m_topology;
	RenderMeshType				m_type;
	uint32						m_flags;
	GpuBuffer*					m_pVertexBuffer;
	GpuBuffer*					m_pIndexBuffer;
	uint32						m_vtxCount;
	uint32						m_idxCount;
	AABB						m_boundingBox;
	int							m_refCount;
};

#endif //RENDER_MESH_H
