#ifndef LUA_MODULE_H
#define LUA_MODULE_H
#pragma once

#include <ScriptsModule\IScriptsModule.h>

class LuaModule : public IScriptsModule
{
public:
	virtual			~LuaModule();

	const char*		GetName() const final;
	bool			Initialize(Environment& env, const SystemInitParams& initParams) final;
};

#endif //LUA_MODULE_H
