#include "stdafx.h"
#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::AddNode(OctreeNode* node, OctreeNode* parent /*= nullptr*/)
{
	if (parent)
	{
		parent->AddChild(node);
	}
	else
	{
		m_sceneRoot->AddChild(node);
	}
}

bool Scene::RemoveNode(OctreeNode* node)
{
	return false;
}

OctreeNode* Scene::Find(const char* name)
{
	return nullptr;
}
