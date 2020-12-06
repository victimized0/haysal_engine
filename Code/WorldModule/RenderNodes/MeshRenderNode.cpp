#include "stdafx.h"
#include "MeshRenderNode.h"

MeshRenderNode::MeshRenderNode()
{

}

MeshRenderNode::~MeshRenderNode()
{

}

const char* MeshRenderNode::GetName() const
{
    return nullptr;
}

const char* MeshRenderNode::GetEntityClassName() const
{
    return nullptr;
}

Vec3 MeshRenderNode::GetPos(bool bWorldOnly /*= true*/) const
{
    return Vec3();
}

const AABB MeshRenderNode::GetAABB() const
{
    return AABB();
}

void MeshRenderNode::SetAABB(const AABB& aabb)
{
}

void MeshRenderNode::Translate(const Vec3& delta)
{
}

void MeshRenderNode::Render(const RenderParams& info, IRenderView* pRenderView)
{
}
