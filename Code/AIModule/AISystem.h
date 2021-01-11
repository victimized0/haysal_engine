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

	virtual void					ProcessStimulus(AIStimulus stimulus) final;

	virtual IAIAgent*				CreateAgent			()						final;
	virtual void					DestroyAgent		(IAIAgent* pAgent)		final;
	virtual AgentActionType			ActionNameToType	(const char* name)		final;
	virtual IAgentAction*			AgentActionFromName	(const char* name)		final;
	virtual IAgentAction*			AgentActionFromType	(AgentActionType type)	final;

private:
	std::vector<AIAgent>			m_agents;
	AgentActionsMap					m_agentActions;

};
#endif //USE_GOAP

#endif //AI_MODULE_H
