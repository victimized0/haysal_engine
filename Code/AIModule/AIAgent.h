#ifndef AI_AGENT_H
#define AI_AGENT_H
#pragma once

#include "ActionPlanner.h"
#include "EntitySystem\IEntity.h"

class AIAgent : public IAIAgent
{
public:
							AIAgent(IEntity* pOwner);
	virtual					~AIAgent();

	virtual void			Init()				override;
	virtual void			Release()			override;
	virtual void			Update(float dt)	override;
	
	virtual AIStimulus*		GetStimulus() override { return &m_curStimulus; }
	virtual IEntity*		GetOwner() override { return m_pOwningEntity;}
	virtual void			SetOwner(IEntity* pOwner) override { m_pOwningEntity = pOwner; }

	virtual bool			PerceptsStimulus(AIStimulusType type) override;
	virtual void			ProcessStimulus(AIStimulus stimulus) override;

	virtual void			Parse(pugi::xml_node& agentNode) override;
	virtual AgentStance		GetStance() const override { return m_currentStance; }

protected:
	virtual void			ProcessVision(AIStimulus stimulus);
	virtual void			ProcessNoise(AIStimulus stimulus);

private:
	IEntity*				m_pOwningEntity;

	ActionPlanner			m_ap;
	AIWorldModel			m_wm;
	AIStimulus				m_curStimulus;

	std::vector<IAIAction*>	m_actionsSet;
	std::vector<AIGoal>		m_goalsSet;
	std::vector<IAIAction*>	m_actionsPlan;

	AIGoal*					m_pCurrentGoal;
	AgentStance				m_currentStance;

	std::vector<AIStimulusType> m_perceptions;
};

#endif //AI_AGENT_H
