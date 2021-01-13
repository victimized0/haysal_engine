#ifndef GAME_FRAMEWORK_H
#define GAME_FRAMEWORK_H
#pragma once

#include <GameFramework\IGameFramework.h>

class GameFramework final : public IGameFramework
{
public:
					GameFramework();
	virtual			~GameFramework();

	// Inherited via IGameFramework
	virtual bool	Init() final;
	virtual void	Release() final;
	virtual void	Update(float dt) final;
	// ~Inherited via IGameFramework

};

#endif //GAME_FRAMEWORK_H
