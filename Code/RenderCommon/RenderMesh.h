#ifndef RENDER_MESH_H
#define RENDER_MESH_H
#pragma once

#include <WorldModule\IMesh.h>
#include <RenderModule\IRenderMesh.h>

struct PosNml
{
	Vec3 Pos;
	Vec3 Nml;
};

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

	virtual size_t				SetMesh(IMesh& mesh, uint32 flags, const Vec3* pPosOffset, bool requiresLock) override;
	virtual IIndexedMesh*		GetIndexedMesh(IIndexedMesh* outMesh) override;
	virtual bool				UpdateVertices(const void* pVertBuffer, int vertsCount, int offset, bool requiresLock = true) override;
	virtual bool				UpdateIndices(const uint32* pIndBuffer, int indicesCount, int offset, bool requiresLock = true) override;
	virtual void				Render(IRenderView* pRenderView) override;
	// ~Inherited via IRenderMesh

private:
	std::vector<PosNml>			m_posNml;
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
