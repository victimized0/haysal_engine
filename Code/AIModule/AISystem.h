#ifndef AI_MODULE_H
#define AI_MODULE_H
#pragma once

#include <AIModule\IAISystem.h>
#include "AgentActions\BaseAgentAction.h"

#ifdef USE_GOAP
class AISystem final : public IAISystem
{
private:
	typedef std::map<AgentActionType, IAgentAction*> AgentActionsMap;

public:
									AISystem() {}
	virtual							~AISystem();

	virtual void					Update()	final;
	virtual void					Init()		final;
	virtual void					Release()	final;

	virtual IAIAgent*				CreateAgent() final;
	virtual void					DestroyAgent(IAIAgent* pAgent) final;

	virtual IAIAction*				GetAction(const char* name)	final;
	virtual AIGoal*					GetGoal(const char* name)	final;

	virtual AgentActionType			ActionNameToType(const char* name) final;
	virtual IAgentAction*			AgentActionFromName(const char* name) final;
	virtual IAgentAction*			AgentActionFromType(AgentActionType type) final;
	virtual const AIWorldModel&		GetWorldModel() const final { return m_worldModel; }
	virtual void					ModifyWorldModel(const std::vector<AIWorldState>& effects) final;

private:
	void							LoadStates();
	void							LoadActions();
	void							LoadGoals();

private:
	std::vector<AIAgent>			m_agents;
	std::vector<AIWorldState>		m_worldModel;
	std::vector<AIAction>			m_goapActions;
	std::vector<AIGoal>				m_goapGoals;
	AgentActionsMap					m_agentActions;

};
#endif //USE_GOAP

#endif //AI_MODULE_H
