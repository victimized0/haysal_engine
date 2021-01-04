#ifndef INTERFACE_AI_MODULE_H
#define INTERFACE_AI_MODULE_H
#pragma once

#include <System\IEngineModule.h>

struct IAIModule : public IEngineModule
{
	virtual ~IAIModule() {}
};

#endif //INTERFACE_AI_MODULE_H
