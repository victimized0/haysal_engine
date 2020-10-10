#include "stdafx.h"
#include "LightSourceNode.h"

LightSourceNode::LightSourceNode()
{

}

LightSourceNode::~LightSourceNode()
{

}

const char* LightSourceNode::GetName() const
{
    return nullptr;
}

const char* LightSourceNode::GetEntityClassName() const
{
    return nullptr;
}

Vec3 LightSourceNode::GetPos(bool bWorldOnly /*= true*/) const
{
    return Vec3();
}

const AABB LightSourceNode::GetAABB() const
{
    return AABB();
}

void LightSourceNode::SetAABB(const AABB& aabb)
{
}

void LightSourceNode::Translate(const Vec3& delta)
{
}

void LightSourceNode::Render(const RenderInfo& info)
{
}
