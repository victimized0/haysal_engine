#ifndef WORLD_MODULE_H
#define WORLD_MODULE_H
#pragma once

#include <WorldModule\IWorldModule.h>

class WorldModule final : public IWorldModule
{
public:
	virtual			~WorldModule();

	const char*		GetName() const final;
	bool			Initialize(Environment& env, const SystemInitParams& initParams) final;
};

#endif //WORLD_MODULE_H
