#ifndef COMBAT_ACTIONS_H
#define COMBAT_ACTIONS_H
#pragma once

#include "BaseAgentAction.h"

class AgentPunchAction final : public BaseAgentAction
{
public:
					AgentPunchAction();
	virtual			~AgentPunchAction();

	// Inherited via IAgentAction
	virtual void	Init(IAIAction* pAct)		final;
	virtual void	Release()					final;
	virtual void	Execute(IAIAgent* pAgent)	final;

	virtual AgentActionType	GetType() const final { return AgentActionType::Punch; }
	// ~Inherited via IAgentAction

private:

};

class AgentKickAction final : public BaseAgentAction
{
public:
					AgentKickAction();
	virtual			~AgentKickAction();

	// Inherited via IAgentAction
	virtual void	Init(IAIAction* pAct)		final;
	virtual void	Release()					final;
	virtual void	Execute(IAIAgent* pAgent)	final;

	virtual AgentActionType	GetType() const final { return AgentActionType::Kick; }
	// ~Inherited via IAgentAction

private:

};

#endif //COMBAT_ACTIONS_H
