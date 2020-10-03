#ifndef WORLD_SCENE_NODE_H
#define WORLD_SCENE_NODE_H
#pragma once

#include <WorldModule\ISceneNode.h>

class OctreeNode : public ISceneNode
{
public:
						OctreeNode();
	virtual				~OctreeNode();

						OctreeNode(OctreeNode const&)				= delete;	// Temporarily delete
						OctreeNode(OctreeNode&&)					= delete;	// Temporarily delete
						OctreeNode& operator=(OctreeNode const&)	= delete;	// Temporarily delete
						OctreeNode& operator=(OctreeNode&&)		= delete;	// Temporarily delete

	void				AddChild(ISceneNode* child) override;
	bool				RemoveChild(ISceneNode* child) override;
	void				SetParent(ISceneNode* parent);

	OctreeNode*			GetChildren();
	bool				HasChildren() override;
	int					ChildrenCount() override;

private:
	std::string			m_name;
	ISceneNode*			m_parent;
	OctreeNode*			m_children[8];
	//AABB				m_boundingBox;

};

#endif //WORLD_SCENE_NODE_H
