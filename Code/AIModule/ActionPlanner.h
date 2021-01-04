#ifndef ACTION_PLANNER_H
#define ACTION_PLANNER_H
#pragma once

#define AP_MAX_ATOMS	64
#define AP_MAX_ACTIONS	64

#include <AIModule\IAISystem.h>

class ActionPlanner final : public IAIActionPlanner
{
private:
	typedef std::vector<std::string>	StatesVector;
	typedef std::vector<AIAction>		ActionsVector;

public:
					ActionPlanner() = default;
	virtual 		~ActionPlanner();

	// Inherited via IAIActionPlanner
	virtual void	Initialise()	final;
	virtual void	Clear()			final;
	virtual void	Release()		final;

	virtual bool	SetWorldModel			(AIWorldState* pWModel, const char* atomName, bool value)	final;
	virtual bool	AddActionPrecondition	(const char* actionName, const char* atomName, bool value)	final;
	virtual bool	AddActionEffect			(const char* actionName, const char* atomName, bool value)	final;
	virtual bool	SetActionCost			(const char* actionName, int cost)							final;
	virtual std::vector<IAIAction> Plan(AIWorldState* pWM, AIGoal* pGoal) final;

#ifdef _DEBUG
	virtual void	PrintDebugInfo			()															final;
#endif
	// ~Inherited via IAIActionPlanner

private:
	StatesVector	m_states;
	ActionsVector	m_actions;

};

#endif //ACTION_PLANNER_H
