#include "StdAfx.h"
#include "RenderMesh.h"

#include <WorldModule\IWorldObj.h>

RenderMesh::RenderMesh(const char* srcName, RenderMeshType type)
    : m_srcFileName(srcName)
    , m_type(type)
    , m_vertexFormat(VertexFormat::None)
    , m_topology(PrimitiveTopology::Undefined)
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
        //assert(false); // If the number is negative, it means that integer overflow took place => it was referenced way too many times.
    }

    if (count == 0)
    {
        // TODO: Delete the object from it's owner
    }

    SAFE_DELETE(m_pVertexBuffer);
    SAFE_DELETE(m_pIndexBuffer);

    return count;
}

void RenderMesh::SetMesh(IMesh& mesh, uint32 flags)
{
    m_pVertexBuffer = new GpuBuffer();
    m_pIndexBuffer  = new GpuBuffer();

    m_boundingBox   = mesh.GetBoundingBox();
    m_vtxCount      = mesh.GetVerticesCount();
    m_idxCount      = mesh.GetIndicesCount();

    Vec3* pPos = mesh.GetPositionsData();
    Vec3* pNml = mesh.GetNormalsData();
    Vec2* pTex = mesh.GetTexCoordsData();
    //Vec4* pCol = mesh.GetColoursData();

    m_vertexFormat = VertexFormat::PosNmlTex;

    m_posNmlTex.resize(m_vtxCount);
    for (size_t i = 0; i < m_posNmlTex.size(); ++i)
    {
        m_posNmlTex[i].Pos = pPos[i];
        m_posNmlTex[i].Nml = pNml[i];
        m_posNmlTex[i].Tex = pTex[i];
    }

    void*   pVtxData = m_posNmlTex.data();
    uint32* pIdxData = mesh.GetIndicesData();

    int vtxOffset = 0,
        idxOffset = 0;

    UpdateVertices(pVtxData, m_vtxCount, vtxOffset);
    UpdateIndices(pIdxData, m_idxCount, idxOffset);

    int subMeshCount = 0;  // mesh.GetSubMeshCount();
    for (int i = 0; i < subMeshCount; ++i)
    {
        int subVtxCnt = 0; // subMesh->GetVtxCount();
        int subIdxCnt = 0; // subMesh->GetIndCount();

        if (subVtxCnt == 0)
            continue;

        pVtxData = nullptr; // subMesh->GetVtxData();
        pIdxData = nullptr; // subMesh->GetIdxData();

        UpdateVertices(pVtxData, subVtxCnt, vtxOffset);
        UpdateIndices(pIdxData, subIdxCnt, idxOffset);

        vtxOffset += subVtxCnt;
        idxOffset += subIdxCnt;
    }
}

IIndexedMesh* RenderMesh::GetIndexedMesh(IIndexedMesh* outMesh)
{
    return nullptr;
}

void RenderMesh::UpdateVertices(void* pData, int vertsCount, int offset)
{
    //assert(false); // Currently engine isn't supposed to update static meshes
    if (m_pVertexBuffer->IsNullBuffer() || !m_pVertexBuffer->IsAvailable())
    {
        uint32 flags = ResourceFlags::BIND_VERTEX_BUFFER /*| ResourceFlags::USAGE_CPU_READ | ResourceFlags::USAGE_CPU_WRITE*/;
        m_pVertexBuffer->Create(vertsCount, VertexFormatsHelper::GetStride(m_vertexFormat), DXGI_FORMAT_UNKNOWN, flags, pData);
        return;
    }

    gRenderer->GetBufMan()->Write(m_pVertexBuffer, pData, vertsCount, offset);
}

void RenderMesh::UpdateIndices(uint32* pData, int indicesCount, int offset)
{
    //assert(false); // Currently engine isn't supposed to update static meshes
    if (m_pIndexBuffer->IsNullBuffer() || !m_pIndexBuffer->IsAvailable())
    {
        uint32 flags = ResourceFlags::BIND_INDEX_BUFFER /*| ResourceFlags::USAGE_CPU_READ | ResourceFlags::USAGE_CPU_WRITE*/;
        m_pIndexBuffer->Create(indicesCount, sizeof(uint32), DXGI_FORMAT_UNKNOWN, flags, pData);
        return;
    }

    gRenderer->GetBufMan()->Write(m_pIndexBuffer, pData, indicesCount, offset);
}

void RenderMesh::Render(const RenderParams& params, IRenderView* pRenderView)
{
    RenderItem rendItem;
    rendItem.pRenderMesh = this;
    rendItem.pShaderItem = params.pMaterial ? &params.pMaterial->GetShaderItem() : nullptr;
    rendItem.WorldMatrix = *params.pMatrix;
    rendItem.pRenderNode = params.pRenderNode;
    //rendItem.pShaderTechnique = &params.pMaterial->GetTechnique();

    if (params.Flags & MaterialFlags::CastShadows)
        pRenderView->Submit(rendItem, RenderListId::ShadowGen);
    if (!(params.Flags & MaterialFlags::NoDepth))
        pRenderView->Submit(rendItem, RenderListId::ZPrePass);
    if (params.Flags & MaterialFlags::LightSource)
        pRenderView->Submit(rendItem, RenderListId::LightPass);
    if (!(params.Flags & MaterialFlags::Unlit))
        pRenderView->Submit(rendItem, RenderListId::Opaque);
}
