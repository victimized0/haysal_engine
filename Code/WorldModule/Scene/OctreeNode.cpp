#include "StdAfx.h"
#include "OctreeNode.h"

OctreeNode::OctreeNode()
{
}

OctreeNode::~OctreeNode()
{
}

void OctreeNode::AddChild(ISceneNode* child)
{
	child->SetParent(this);
}

bool OctreeNode::RemoveChild(ISceneNode* child)
{
	return false;
}

void OctreeNode::SetParent(ISceneNode* parent)
{
	if (m_parent)
		return; // Already has parent. Reparenting needs some extra work; skipping it for now
	m_parent = parent;
}

bool OctreeNode::HasChildren()
{
	return false;
}

int OctreeNode::ChildrenCount()
{
	return 0;
}
