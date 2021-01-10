#ifndef AI_MODULE_H
#define AI_MODULE_H
#pragma once

#include <platform.inl>
#include <AIModule\IAIModule.h>
#include <AIModule\IAISystem.h>

#ifdef USE_GOAP
class AISystem final : public IAISystem
{
public:
								AISystem() {}
	virtual						~AISystem();

	virtual void				Update()	final;
	virtual void				Init()		final;
	virtual void				Release()	final;

	virtual IAIAgent*			CreateAgent() final;
	virtual void				DestroyAgent(IAIAgent* pAgent) final;

	virtual IAIAction*			GetAction(const char* name)	final;
	virtual AIGoal*				GetGoal(const char* name)	final;

private:
	void						LoadStates();
	void						LoadActions();
	void						LoadGoals();

private:
	std::vector<AIAgent>		m_agents;
	std::vector<AIWorldState>	m_goapStates;
	std::vector<AIAction>		m_goapActions;
	std::vector<AIGoal>			m_goapGoals;

};
#endif //USE_GOAP

#endif //AI_MODULE_H
