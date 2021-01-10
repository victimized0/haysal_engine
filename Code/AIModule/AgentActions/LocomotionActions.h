#ifndef LOCOMOTION_ACTIONS_H
#define LOCOMOTION_ACTIONS_H
#pragma once

#include "BaseAgentAction.h"

class AgentIdleAction final : public BaseAgentAction
{
public:
					AgentIdleAction();
	virtual			~AgentIdleAction();

	// Inherited via IAgentAction
	virtual void	Init(IAIAction* pAct)		final;
	virtual void	Release()					final;
	virtual void	Execute(IAIAgent* pAgent)	final;

	virtual AgentActionType	GetType() const final { return AgentActionType::Idle; }
	// ~Inherited via IAgentAction

private:

};

class AgentMoveToAction final : public BaseAgentAction
{
public:
					AgentMoveToAction();
	virtual			~AgentMoveToAction();

	// Inherited via IAgentAction
	virtual void	Init(IAIAction* pAct)		final;
	virtual void	Release()					final;
	virtual void	Execute(IAIAgent* pAgent)	final;

	virtual AgentActionType	GetType() const final { return AgentActionType::MoveTo; }
	// ~Inherited via IAgentAction

private:

};

class AgentRunToAction final : public BaseAgentAction
{
public:
					AgentRunToAction();
	virtual			~AgentRunToAction();

	// Inherited via IAgentAction
	virtual void	Init(IAIAction* pAct)		final;
	virtual void	Release()					final;
	virtual void	Execute(IAIAgent* pAgent)	final;

	virtual AgentActionType	GetType() const final { return AgentActionType::RunTo; }
	// ~Inherited via IAgentAction

private:

};

class AgentJumpAction final : public BaseAgentAction
{
public:
					AgentJumpAction();
	virtual			~AgentJumpAction();

	// Inherited via IAgentAction
	virtual void	Init(IAIAction* pAct)		final;
	virtual void	Release()					final;
	virtual void	Execute(IAIAgent* pAgent)	final;

	virtual AgentActionType	GetType() const final { return AgentActionType::Jump; }
	// ~Inherited via IAgentAction

private:

};

class AgentPickUpAction final : public BaseAgentAction
{
public:
					AgentPickUpAction();
	virtual			~AgentPickUpAction();

	// Inherited via IAgentAction
	virtual void	Init(IAIAction* pAct)		final;
	virtual void	Release()					final;
	virtual void	Execute(IAIAgent* pAgent)	final;

	virtual AgentActionType	GetType() const final { return AgentActionType::PickUp; }
	// ~Inherited via IAgentAction

private:

};

class AgentFleeAction final : public BaseAgentAction
{
public:
					AgentFleeAction();
	virtual			~AgentFleeAction();

	// Inherited via IAgentAction
	virtual void	Init(IAIAction* pAct)		final;
	virtual void	Release()					final;
	virtual void	Execute(IAIAgent* pAgent)	final;

	virtual AgentActionType	GetType() const final { return AgentActionType::Flee; }
	// ~Inherited via IAgentAction

private:

};

class AgentScoutAction final : public BaseAgentAction
{
public:
					AgentScoutAction();
	virtual			~AgentScoutAction();

	// Inherited via IAgentAction
	virtual void	Init(IAIAction* pAct)		final;
	virtual void	Release()					final;
	virtual void	Execute(IAIAgent* pAgent)	final;

	virtual AgentActionType	GetType() const final { return AgentActionType::Scout; }
	// ~Inherited via IAgentAction

private:

};

#endif //LOCOMOTION_ACTIONS_H
