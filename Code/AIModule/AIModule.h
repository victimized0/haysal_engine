#ifndef AI_MODULE_H
#define AI_MODULE_H
#pragma once

#include <AIModule\IAIModule.h>

class AIModule final : public IAIModule
{
public:
	virtual			~AIModule();

	const char*		GetName() const final;
	bool			Initialize(Environment& env, const SystemInitParams& initParams) final;
};

#endif //AI_MODULE_H
