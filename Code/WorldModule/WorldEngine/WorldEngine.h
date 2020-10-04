#ifndef WORLD_ENGINE_H
#define WORLD_ENGINE_H
#pragma once

#include <platform.inl>
#include <WorldModule\IWorldModule.h>

class WorldEngine;
extern WorldEngine* g_world;
typedef std::vector<std::unique_ptr<IRenderNode>> WorldObjectPtrsContainer;

class WorldEngine final : public IWorldEngine
{
public:
							WorldEngine();
	virtual					~WorldEngine();

	void					Release() final;
	void					ShutDown() final;
	
	void					OnFrameStart() final;

	//IWorldObject*			CreateObject(const WorldObjectDescr& descr) final;
	//bool					DeleteObject(IWorldObject* pObj) final;

private:
	//WorldObjectPtrsContainer	m_worldObjects;

};

#endif //WORLD_ENGINE_H
