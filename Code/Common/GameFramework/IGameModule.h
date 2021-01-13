#ifndef INTERFACE_HAME_MODULE_H
#define INTERFACE_HAME_MODULE_H
#pragma once

#include <System\IEngineModule.h>

struct IGameModule : public IEngineModule
{
	virtual ~IGameModule() {}
};

#endif //INTERFACE_HAME_MODULE_H
