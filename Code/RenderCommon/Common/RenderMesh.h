#ifndef RENDER_MESH_H
#define RENDER_MESH_H
#pragma once

#include <WorldModule\IMesh.h>
#include <RenderModule\IRenderMesh.h>

class RenderMesh : public IRenderMesh
{
public:
								RenderMesh(const char* srcName, RenderMeshType type = RenderMeshType::Static);
	virtual						~RenderMesh();

	// Inherited via IRenderMesh
	virtual void				AddRef()						final;
	virtual int					Release()						final;
	virtual const char*			GetSourceName()			const	final	{ return m_srcFileName.c_str(); }
	virtual int					GetIndicesCount()		const	final	{ return m_idxCount; }
	virtual int					GetVerticesCount()		const	final	{ return m_vtxCount; }
	virtual VertexFormat		GetVertexFormat()		const	final	{ return m_vertexFormat; }
	virtual RenderMeshType		GetMeshType()			const	final	{ return m_type; }
	virtual PrimitiveTopology	GetPrimitiveTopology()	const	final	{ return m_topology; }

	virtual GpuBuffer*			GetVertexBuffer()				final	{ return m_pVertexBuffer; }
	virtual GpuBuffer*			GetIndexBuffer()				final	{ return m_pIndexBuffer; }

	virtual void				SetMesh(IMesh& mesh, uint32 flags) override;
	virtual IIndexedMesh*		GetIndexedMesh(IIndexedMesh* outMesh = nullptr) override;

	virtual void				UpdateVertices(void* pData, int vertsCount, int offset) override;
	virtual void				UpdateIndices(uint32* pData, int indicesCount, int offset) override;

	virtual void				Render(const RenderParams& params, IRenderView* pRenderView) override;
	// ~Inherited via IRenderMesh

private:
	std::vector<VF_PosNmlTex>	m_posNmlTex;
	std::string					m_srcFileName;
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
