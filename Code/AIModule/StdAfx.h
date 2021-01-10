#ifndef AI_STD_AFX_H
#define AI_STD_AFX_H
#pragma once

#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)

#define USE_GOAP 1

#include <platform.h>
#include "AIAction.h"
#include "AIAgent.h"
#include "AISystem.h"

extern AISystem* g_aiSystem;

#endif //AI_STD_AFX_H