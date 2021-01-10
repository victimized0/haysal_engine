#ifndef AI_ACTION_H
#define AI_ACTION_H
#pragma once

#include <AIModule\IAISystem.h>

class AIAction : public IAIAction
{
public:
							AIAction();
	virtual					~AIAction();

	// Inherited via IAIAction
	virtual void			SetAgentAction(IAgentAction* pAct)					override { m_pAgentAction = pAct; m_pAgentAction->Init(this); }
	virtual IAgentAction*	GetAgentAction()							const	override { return m_pAgentAction; }

	virtual bool			IsCompleted()								const	override { return m_pAgentAction->IsCompleted(); }
	virtual void			AddPrecond	(AIWorldState& state)					override;
	virtual void			AddEffect	(AIWorldState& state)					override;
	virtual void			SetCost		(int cost)								override;
	virtual void			SetName		(const char* name)						override;
	virtual const char*		GetName		()										override { return m_name.c_str(); }
	virtual const std::vector<AIWorldState>& GetEffects()				const	override { return m_effects; }
	virtual const std::vector<AIWorldState>& GetPreconds()				const	override { return m_preconds; }

	virtual void			Parse		(const pugi::xml_node& actNode)			override;
	// ~Inherited via IAIAction

private:
	IAgentAction*				m_pAgentAction;
	std::string					m_name;
	std::vector<AIWorldState>	m_preconds;
	std::vector<AIWorldState>	m_effects;
	int							m_cost;
};

#endif //AI_ACTION_H
