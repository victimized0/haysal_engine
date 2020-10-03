#ifndef WORLD_SCENE_H
#define WORLD_SCENE_H
#pragma once

#include "SceneNode.h"

class Scene
{
public:
			Scene();
			~Scene();

			Scene(Scene const&)				= delete;
			Scene(Scene&&)					= delete;
			Scene& operator=(Scene const&)	= delete;
			Scene& operator=(Scene&&)		= delete;


private:

};

#endif //WORLD_SCENE_H
