#include "stdafx.h"
#include "VegetationNode.h"

VegetationNode::VegetationNode()
{

}

VegetationNode::~VegetationNode()
{

}

const char* VegetationNode::GetName() const
{
    return nullptr;
}

const char* VegetationNode::GetEntityClassName() const
{
    return nullptr;
}

Vec3 VegetationNode::GetPos(bool bWorldOnly /*= true*/) const
{
    return Vec3();
}

const AABB VegetationNode::GetAABB() const
{
    return AABB();
}

void VegetationNode::SetAABB(const AABB& aabb)
{
}

void VegetationNode::Translate(const Vec3& delta)
{
}

void VegetationNode::Render(const RenderParams& info, IRenderView* pRenderView)
{
}

void VegetationNode::SetMaterial(IMaterial* pMaterial)
{
}
