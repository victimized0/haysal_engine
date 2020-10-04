#ifndef INTERFACE_WORLD_MODULE_H
#define INTERFACE_WORLD_MODULE_H
#pragma once

#include <System\IEngineModule.h>
#include <WorldModule\IRenderNode.h>

struct IWorldModule : public IEngineModule
{
	virtual ~IWorldModule() {}
};

struct IWorldEngine
{
	virtual					~IWorldEngine() {}

	virtual void			Release() = 0;
	virtual void			ShutDown() = 0;

	virtual void			OnFrameStart() = 0;

	//virtual IWorldObject*	CreateObject(const WorldObjectDescr& descr) = 0;
	//virtual bool			DeleteObject(IWorldObject* pObj) = 0;
	//virtual IWorldObject*	FindObject(UUID uuid) = 0;

};

#endif //INTERFACE_WORLD_MODULE_H
