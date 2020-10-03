#ifndef WORLD_ENGINE_H
#define WORLD_ENGINE_H
#pragma once

#include <platform.inl>
#include <WorldModule\IWorldModule.h>
#include <Objects\GameObject.h>

class WorldEngine;
extern WorldEngine* g_world;

class WorldEngine final : public IWorldEngine
{
public:
							WorldEngine();
	virtual					~WorldEngine();

	void					Release() final;
	void					ShutDown() final;
	
	void					OnFrameStart() final;

	IWorldObject*			CreateObject() final;
	bool					DeleteObject(IWorldObject* pObj) final;

private:
	std::vector<GameObject>	m_worldObjects;

};

#endif //WORLD_ENGINE_H
