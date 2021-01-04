#ifndef AI_AGENT_H
#define AI_AGENT_H
#pragma once

#include <AIModule\IAISystem.h>

class AIAgent : public IAIAgent
{
public:
							AIAgent();
	virtual					~AIAgent();

	virtual void			Init()		override;
	virtual void			Release()	override;

	virtual void			AddAction(IAIAction* pAction)	override;
	virtual void			SetGoal(AIGoal* pGoal)			override;
	virtual void			Parse(pugi::xml_node& agentNode, AgentDesc& agentDesc)	override;

private:
	std::vector<IAIAction*>	m_actionsSet;
	AIGoal*					m_pCurrentGoal;

};

#endif //AI_AGENT_H
