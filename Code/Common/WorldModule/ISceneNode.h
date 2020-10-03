#ifndef INTERFACE_SCENE_NODE_H
#define INTERFACE_SCENE_NODE_H
#pragma once

struct ISceneNode
{
	virtual				~ISceneNode() {}
	virtual void		AddChild(ISceneNode* child) = 0;
	virtual bool		RemoveChild(ISceneNode* child) = 0;
	virtual void		SetParent(ISceneNode* parent) = 0;
	virtual bool		HasChildren() = 0;
	virtual int			ChildrenCount() = 0;
	//virtual const AABB&	GetBoundingBox() = 0;

};

#endif INTERFACE_SCENE_NODE_H
