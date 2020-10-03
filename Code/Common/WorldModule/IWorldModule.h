#ifndef INTERFACE_WORLD_MODULE_H
#define INTERFACE_WORLD_MODULE_H
#pragma once

#include "System\IEngineModule.h"
#include <WorldModule\IWorldObject.h>

struct IWorldModule : public IEngineModule
{
	virtual ~IWorldModule() {}
};

struct ObjectInfo
{
	Vec3 Position;
	Quat Rotation;
	float Scale;


};

struct IWorldEngine
{
	virtual					~IWorldEngine() {}

	virtual void			Release() = 0;
	virtual void			ShutDown() = 0;

	virtual void			OnFrameStart() = 0;

	virtual IWorldObject*	CreateObject() = 0;
	virtual bool			DeleteObject(IWorldObject* pObj) = 0;
	//virtual IWorldObject*	FindObject(UUID uuid) = 0;

};

#endif //INTERFACE_WORLD_MODULE_H
