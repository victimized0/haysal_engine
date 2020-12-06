#ifndef INTERFACE_RENDER_MESH_H
#define INTERFACE_RENDER_MESH_H
#pragma once

struct IMesh;
struct IIndexedMesh;

enum class RenderMeshType : uint8
{
	Immmutable	= 0,
	Static		= 1,
	Dynamic		= 2,
};

struct IRenderMesh
{
	virtual						~IRenderMesh() {}

	virtual void				AddRef() = 0;
	virtual int					Release() = 0;

	virtual const char*			GetSourceName() const = 0;
	virtual int					GetIndicesCount() = 0;
	virtual int					GetVerticesCount() = 0;
	virtual VertexFormat		GetVertexFormat() = 0;
	virtual RenderMeshType		GetMeshType() = 0;

	virtual size_t				SetMesh(IMesh& mesh, uint32 flags, const Vec3* pPosOffset, bool requiresLock) = 0;
	virtual IIndexedMesh*		GetIndexedMesh(IIndexedMesh* outMesh) = 0;

	virtual bool				UpdateVertices(const void* pVertBuffer, int vertsCount, int offset, bool requiresLock = true) = 0;
	virtual bool				UpdateIndices(const uint32* pIndBuffer, int indicesCount, int offset, bool requiresLock = true) = 0;

	virtual void                Render(const struct RenderParams& params, IRenderView* pRenderView) = 0;
	//virtual void				DebugRender() = 0;

};

#endif //INTERFACE_RENDER_MESH_H
