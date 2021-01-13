#ifndef INTERFACE_GAME_FRAMEWORK
#define INTERFACE_GAME_FRAMEWORK
#pragma once

struct IGameFramework
{
	virtual			~IGameFramework() {}
	virtual bool	Init() = 0;
	virtual void	Release() = 0;
	virtual void	Update(float dt) = 0;
};

#endif //INTERFACE_GAME_FRAMEWORK
