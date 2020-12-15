#ifndef INTERFACE_RENDER_MESH_H
#define INTERFACE_RENDER_MESH_H
#pragma once

#include <WorldModule\IMesh.h>

struct IMesh;
//struct IIndexedMesh;
enum class PrimitiveTopology : uint8;
class GpuBuffer;

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
	virtual int					GetIndicesCount() const = 0;
	virtual int					GetVerticesCount() const = 0;
	virtual VertexFormat		GetVertexFormat() const = 0;
	virtual RenderMeshType		GetMeshType() const = 0;
	virtual PrimitiveTopology	GetPrimitiveTopology() const = 0;

	virtual void				SetMesh(IMesh& mesh, uint32 flags, const Vec3* pPosOffset) = 0;
	//virtual IIndexedMesh*		GetIndexedMesh(IIndexedMesh* outMesh) = 0;

	virtual void				UpdateVertices(void* pVertBuffer, int vertsCount, int offset) = 0;
	virtual void				UpdateIndices(uint32* pIndBuffer, int indicesCount, int offset) = 0;

	virtual GpuBuffer*			GetVertexBuffer() = 0;
	virtual GpuBuffer*			GetIndexBuffer() = 0;

	virtual void                Render(const struct RenderParams& params, IRenderView* pRenderView) = 0;
	//virtual void				DebugRender() = 0;

};

#endif //INTERFACE_RENDER_MESH_H
