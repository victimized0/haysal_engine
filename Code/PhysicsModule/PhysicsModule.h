#ifndef PHYSICS_MODULE_H
#define PHYSICS_MODULE_H
#pragma once

#include <PhysicsModule\IPhysicsModule.h>

class PhysicsModule : public IPhysicsModule
{
public:
	virtual			~PhysicsModule();

	const char*		GetName() const final;
	bool			Initialize(Environment& env, const SystemInitParams& initParams) final;
};

#endif //PHYSICS_MODULE_H
