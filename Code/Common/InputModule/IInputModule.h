#ifndef INTERFACE_INPUT_MODULE_H
#define INTERFACE_INPUT_MODULE_H
#pragma once

#include <System\IEngineModule.h>

struct IInputModule : public IEngineModule
{
	virtual ~IInputModule() {}
};

#endif //INTERFACE_INPUT_MODULE_H
