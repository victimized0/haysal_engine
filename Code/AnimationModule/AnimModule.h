#ifndef ANIMATION_MODULE_H
#define ANIMATION_MODULE_H
#pragma once

#include <AnimModule\IAnimModule.h>

class AnimationModule : public IAnimModule
{
public:
	virtual			~AnimationModule();

	const char*		GetName() const final;
	bool			Initialize(Environment& env, const SystemInitParams& initParams) final;
};

#endif //ANIMATION_MODULE_H
