#ifndef INTERFACE_ENTITY_SYSTEM_H
#define INTERFACE_ENTITY_SYSTEM_H
#pragma once

#include "IEntity.h"

struct IEntitySystem
{
	virtual				~IEntitySystem() {}

	virtual void		Init() = 0;
	virtual void		ShutDown() = 0;

	virtual void		Load() = 0;
	virtual void		Unload() = 0;

	virtual IEntity*	LoadEntity(const char* name) = 0;
	virtual IEntity*	GetEntity(const char* name) = 0;
};

#endif //INTERFACE_ENTITY_SYSTEM_H
