#include "RenderMesh.h"

RenderMesh::RenderMesh()
{
}

RenderMesh::~RenderMesh()
{
}

void RenderMesh::AddRef()
{
}

int RenderMesh::Release()
{
    return 0;
}

bool RenderMesh::CanUpdate()
{
    return false;
}

bool RenderMesh::CanRender()
{
    return false;
}

const char* RenderMesh::GetSourceName() const
{
    return nullptr;
}

int RenderMesh::GetIndicesCount()
{
    return 0;
}

int RenderMesh::GetVerticesCount()
{
    return 0;
}

InputLayoutHandle RenderMesh::GetVertexFormat()
{
    return InputLayoutHandle();
}

RenderMeshType RenderMesh::GetMeshType()
{
    return RenderMeshType();
}

size_t RenderMesh::SetMesh(IMesh& mesh, uint32 flags, const Vec3* pPosOffset, bool requiresLock)
{
    return size_t();
}

IIndexedMesh* RenderMesh::GetIndexedMesh()
{
    return nullptr;
}

bool RenderMesh::UpdateVertices(const void* pVertBuffer, int vertsCount, int offset, bool requiresLock = true)
{
    return false;
}

bool RenderMesh::UpdateIndices(const uint32* pIndBuffer, int indicesCount, int offset, bool requiresLock = true)
{
    return false;
}

void RenderMesh::Render(const RenderPassInfo& passInfo)
{
}
