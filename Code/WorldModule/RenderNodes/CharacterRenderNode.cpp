#include "stdafx.h"
#include "CharacterRenderNode.h"

CharacterRenderNode::CharacterRenderNode()
{

}

CharacterRenderNode::~CharacterRenderNode()
{

}

const char* CharacterRenderNode::GetName() const
{
    return nullptr;
}

const char* CharacterRenderNode::GetEntityClassName() const
{
    return nullptr;
}

Vec3 CharacterRenderNode::GetPos(bool bWorldOnly /*= true*/) const
{
    return Vec3();
}

const AABB CharacterRenderNode::GetAABB() const
{
    return AABB();
}

void CharacterRenderNode::SetAABB(const AABB& aabb)
{
}

void CharacterRenderNode::Translate(const Vec3& delta)
{
}

void CharacterRenderNode::Render(const RenderParams& info, IRenderView* pRenderView)
{
}

void CharacterRenderNode::SetMaterial(IMaterial* pMaterial)
{
}
