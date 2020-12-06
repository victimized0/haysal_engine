#include "StdAfx.h"
#include "RenderMesh.h"

RenderMesh::RenderMesh(const char* srcName)
    : m_srcName(srcName)
    , m_vertexFormat(VertexFormat::None)
    , m_topology(PrimitiveTopology::Undefined)
    , m_type(RenderMeshType::Immmutable)
    , m_flags(0)
    , m_vtxCount(0)
    , m_idxCount(0)
    , m_pVertexBuffer(nullptr)
    , m_pIndexBuffer(nullptr)
{
}

RenderMesh::~RenderMesh()
{
    Release();
}

void RenderMesh::AddRef()
{
    ++m_refCount; // TODO: add multithread support
}

int RenderMesh::Release()
{
    int count = --m_refCount; // TODO: add multithread support
    if (count < 0)
    {
        assert(false); // If the number is negative, it means that integer overflow took place => it was referenced way too many times.
    }

    if (count == 0)
    {
        // TODO: Delete the object from it's owner
    }

    return count;
}

size_t RenderMesh::SetMesh(IMesh& mesh, uint32 flags, const Vec3* pPosOffset, bool requiresLock)
{
    m_boundingBox   = mesh.GetBoundingBox();
    m_vtxCount      = mesh.GetVerticesCount();
    m_idxCount      = mesh.GetIndicesCount();
    int numSubmesh  = 1;// TODO: mesh.GetSubmeshCount();

    for (int i = 0; i < numSubmesh; ++i)
    {

    }
}

IIndexedMesh* RenderMesh::GetIndexedMesh(IIndexedMesh* outMesh)
{
    return nullptr;
}

bool RenderMesh::UpdateVertices(const void* pVertBuffer, int vertsCount, int offset, bool requiresLock = true)
{
    //m_pVertexBuffer->UpdateContent();
    return true;
}

bool RenderMesh::UpdateIndices(const uint32* pIndBuffer, int indicesCount, int offset, bool requiresLock = true)
{
    return true;
}

void RenderMesh::Render(IRenderView* pRenderView)
{
    RenderItem rendItem;
    rendItem.pRenderMesh = this;
    rendItem.WorldMatrix = m_matrix * m_pParent->GetWorld();

    if (m_flags & ObjectFlags::CastShadows)
        pRenderView->Submit(rendItem, RenderListId::ShadowGen);
    if (!(m_flags & ObjectFlags::NoDepth))
        pRenderView->Submit(rendItem, RenderListId::ZPrePass);
    if (m_flags & ObjectFlags::LightSource)
        pRenderView->Submit(rendItem, RenderListId::LightPass);
    if (!(m_flags & ObjectFlags::Unlit))
        pRenderView->Submit(rendItem, RenderListId::Opaque);
}
