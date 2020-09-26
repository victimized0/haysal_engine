#ifndef INTERFACE_ENGINE_MODULE_H
#define INTERFACE_ENGINE_MODULE_H
#pragma once

struct Environment;
struct SystemInitParams;

struct IEngineModule
{
	virtual ~IEngineModule() = default;
	virtual const char* GetName() const = 0;
	virtual bool Initialize(Environment& env, const SystemInitParams& initParams) = 0;
};

#endif //INTERFACE_ENGINE_MODULE_H
