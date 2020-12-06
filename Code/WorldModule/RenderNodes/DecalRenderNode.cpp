#include "stdafx.h"
#include "DecalRenderNode.h"

DecalRenderNode::DecalRenderNode()
{

}

DecalRenderNode::~DecalRenderNode()
{

}

const char* DecalRenderNode::GetName() const
{
    return nullptr;
}

const char* DecalRenderNode::GetEntityClassName() const
{
    return nullptr;
}

Vec3 DecalRenderNode::GetPos(bool bWorldOnly /*= true*/) const
{
    return Vec3();
}

const AABB DecalRenderNode::GetAABB() const
{
    return AABB();
}

void DecalRenderNode::SetAABB(const AABB& aabb)
{
}

void DecalRenderNode::Translate(const Vec3& delta)
{
}

void DecalRenderNode::Render(const RenderParams& info, IRenderView* pRenderView)
{
}
