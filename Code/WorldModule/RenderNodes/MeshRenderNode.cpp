#include "stdafx.h"
#include "MeshRenderNode.h"

MeshRenderNode::MeshRenderNode()
    : m_pOwner(nullptr)
    , m_pWorldObj(nullptr)
    , m_pMaterial(nullptr)
    , m_matrix(Matrix::Identity)
{

}

MeshRenderNode::~MeshRenderNode()
{
    gEnv->pWorld->UnregisterEntity(this);
    m_pWorldObj = nullptr;
}

const char* MeshRenderNode::GetName() const
{
    if (m_pWorldObj)
        return m_pWorldObj->GetFilePath();
    return "NoWorldObjSet";
}

Vec3 MeshRenderNode::GetPos(bool bWorldOnly /*= true*/) const
{
    assert(bWorldOnly);
    return m_matrix.Translation();
}

void MeshRenderNode::SetMaterial(IMaterial* pMaterial)
{
    m_pMaterial = pMaterial;
}

void MeshRenderNode::SetMatrix(const Matrix& mat)
{
    if (m_matrix == mat)
        return;

    m_matrix = mat;

    CalculateBBox();
}

void MeshRenderNode::Translate(const Vec3& delta)
{
}

void MeshRenderNode::Render(const RenderParams& info, IRenderView* pRenderView)
{
    if (!m_pWorldObj)
        return;

    RenderParams params = info;
    params.pMatrix      = &m_matrix;
    params.pMaterial    = m_pMaterial;

    m_pWorldObj->Render(params, pRenderView);
}

void MeshRenderNode::Render(IRenderView* pRenderView)
{
    if (!m_pWorldObj)
        return;

    RenderParams params = {};
    params.pMatrix      = &m_matrix;
    params.pMaterial    = m_pMaterial;
    params.pRenderNode  = this;

    m_pWorldObj->Render(params, pRenderView);
}

IWorldObj* MeshRenderNode::GetWorldObj()
{
    return m_pWorldObj;
}

void MeshRenderNode::SetWorldObj(IWorldObj* pObj)
{
    if (m_pWorldObj == pObj)
        return;

    m_pWorldObj = dynamic_cast<WorldObject*>(pObj);
}

void MeshRenderNode::CalculateBBox()
{
    if (!m_pWorldObj)
        return;

    m_bbox = m_pWorldObj->GetBoundingBox();
    m_bbox.Transform(m_bbox, m_matrix);
}
