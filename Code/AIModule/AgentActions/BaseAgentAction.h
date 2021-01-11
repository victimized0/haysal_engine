#ifndef BASE_AGENT_ACTION_H
#define BASE_AGENT_ACTION_H
#pragma once

class BaseAgentAction : public IAgentAction
{
public:
				BaseAgentAction();
	virtual		~BaseAgentAction();

	// Inherited via IAgentAction
	virtual void Init(IAIAction* pAct) override;
	virtual void Release() override;

	virtual bool IsCompleted() const override { return m_isCompleted; }

	virtual void Execute(IAIAgent* pAgent) override = 0;
	virtual AgentActionType GetType() const override = 0;
	// ~Inherited via IAgentAction

protected:
	IAIAction*	m_pOwnerAct;
	bool		m_isCompleted;

};

#endif //BASE_AGENT_ACTION_H
