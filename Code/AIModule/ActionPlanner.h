#ifndef ACTION_PLANNER_H
#define ACTION_PLANNER_H
#pragma once

#define AP_MAX_STATES	64
#define AP_MAX_ACTIONS	64

struct AStarNode
{

};

class ActionPlanner final : public IAIActionPlanner
{
private:
	typedef std::vector<IAIAction*>		ActionsVector;

public:
						ActionPlanner() = default;
	virtual 			~ActionPlanner();

	// Inherited via IAIActionPlanner
	virtual void		Initialise()	final;
	virtual void		Clear()			final;
	virtual void		Release()		final;

	virtual bool		SetWorldModel			(AIWorldState* pWModel, const char* atomName, bool value)	final;
	virtual bool		AddActionPrecondition	(const char* actionName, const char* atomName, bool value)	final;
	virtual bool		AddActionEffect			(const char* actionName, const char* atomName, bool value)	final;
	virtual bool		SetActionCost			(const char* actionName, int cost)							final;
	virtual std::queue<AIAction> Plan			(const AIWorldModel& wm, AIGoal* pGoal)						final;

#ifdef _DEBUG
	virtual void		PrintDebugInfo			()															final;
#endif
	// ~Inherited via IAIActionPlanner

private:
	AIWorldModel		m_wm;
	ActionsVector		m_actions;

};

#endif //ACTION_PLANNER_H
