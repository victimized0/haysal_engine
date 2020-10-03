#ifndef WORLD_SCENE_H
#define WORLD_SCENE_H
#pragma once

#include "OctreeNode.h"

class Scene
{
public:
				Scene();
				~Scene();

				Scene(Scene const&)				= delete;
				Scene(Scene&&)					= delete;
				Scene& operator=(Scene const&)	= delete;
				Scene& operator=(Scene&&)		= delete;

	void		AddNode(OctreeNode* node, OctreeNode* parent = nullptr);
	bool		RemoveNode(OctreeNode* node);
	OctreeNode*	Find(const char* name);

private:
	OctreeNode*	m_sceneRoot;


};

#endif //WORLD_SCENE_H
