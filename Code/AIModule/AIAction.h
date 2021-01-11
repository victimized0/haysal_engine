#ifndef AI_ACTION_H
#define AI_ACTION_H
#pragma once

#include <AIModule\IAISystem.h>

class AIAction : public IAIAction
{
public:
							AIAction();
							AIAction(std::string name, int cost);
	virtual					~AIAction();

	// Inherited via IAIAction
	virtual void			SetAgentAction(IAgentAction* pAct)					override { m_pAgentAction = pAct; m_pAgentAction->Init(this); }
	virtual IAgentAction*	GetAgentAction()							const	override { return m_pAgentAction; }

	virtual bool			IsCompleted	()								const	override { return m_pAgentAction->IsCompleted(); }
	virtual bool			CheckConform	(const AIWorldModel& ws)const	override;

	virtual AIWorldModel	ApplyEffects		(const AIWorldModel& ws)const	override;
	virtual void			AddPrecond	(const char* name, bool value)			override;
	virtual void			AddEffect	(const char* name, bool value)			override;

	virtual void			SetCost		(int cost)								override;
	virtual int				GetCost		()										override { return m_cost; }

	virtual void			SetName		(const char* name)						override;
	virtual const char*		GetName		()										override { return m_name.c_str(); }
	// ~Inherited via IAIAction

private:
	IAgentAction*				m_pAgentAction;
	std::map<std::string, bool>	m_preconds;
	std::map<std::string, bool>	m_effects;
	std::string					m_name;
	int							m_cost;
};

#endif //AI_ACTION_H
