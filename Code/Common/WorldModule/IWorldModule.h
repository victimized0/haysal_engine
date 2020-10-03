#ifndef INTERFACE_WORLD_MODULE_H
#define INTERFACE_WORLD_MODULE_H
#pragma once

#include "..\IEngineModule.h"

struct IWorldModule : public IEngineModule
{

};

struct IWorldEngine
{
	virtual				~IWorldEngine() {}
	virtual void		Release() = 0;
	virtual void		ShutDown() = 0;

};

#endif //INTERFACE_WORLD_MODULE_H
