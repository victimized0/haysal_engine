#ifndef WORLD_ENGINE_H
#define WORLD_ENGINE_H
#pragma once

#include <platform.inl>
#include <WorldModule\IWorldModule.h>

class WorldEngine;
extern WorldEngine* g_world;

class WorldEngine final : public IWorldEngine
{
public:
				WorldEngine();
	virtual		~WorldEngine();

	void		Release() final;
	void		ShutDown() final;

private:

};

#endif //WORLD_ENGINE_H
