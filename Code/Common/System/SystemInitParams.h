#ifndef SYSTEM_INIT_PARAMS_H
#define SYSTEM_INIT_PARAMS_H
#pragma once

#include <System\ISystem.h>

struct SystemInitParams
{
	ISystem*	pSystem;
	bool		IsEditor;
};

#endif //SYSTEM_INIT_PARAMS_H
