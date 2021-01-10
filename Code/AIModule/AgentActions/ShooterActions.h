#ifndef SHOOTER_ACTIONS_H
#define SHOOTER_ACTIONS_H
#pragma once

#include "BaseAgentAction.h"

class AgentAimAction final : public BaseAgentAction
{
public:
					AgentAimAction();
	virtual			~AgentAimAction();

	// Inherited via IAgentAction
	virtual void	Init(IAIAction* pAct)		final;
	virtual void	Release()					final;
	virtual void	Execute(IAIAgent* pAgent)	final;

	virtual AgentActionType	GetType() const final { return AgentActionType::Aim; }
	// ~Inherited via IAgentAction

private:

};

class AgentShootAction final : public BaseAgentAction
{
public:
					AgentShootAction();
	virtual			~AgentShootAction();

	// Inherited via IAgentAction
	virtual void	Init(IAIAction* pAct)		final;
	virtual void	Release()					final;
	virtual void	Execute(IAIAgent* pAgent)	final;

	virtual AgentActionType	GetType() const final { return AgentActionType::Shoot; }
	// ~Inherited via IAgentAction

private:

};

class AgentReloadAction final : public BaseAgentAction
{
public:
					AgentReloadAction();
	virtual			~AgentReloadAction();

	// Inherited via IAgentAction
	virtual void	Init(IAIAction* pAct)		final;
	virtual void	Release()					final;
	virtual void	Execute(IAIAgent* pAgent)	final;

	virtual AgentActionType	GetType() const final { return AgentActionType::Reload; }
	// ~Inherited via IAgentAction

private:

};

#endif //SHOOTER_ACTIONS_H
