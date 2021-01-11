#include "StdAfx.h"
#include "BaseAgentAction.h"

BaseAgentAction::BaseAgentAction()
	: m_pOwnerAct(nullptr)
	, m_isCompleted(false)
{
}

BaseAgentAction::~BaseAgentAction()
{
	Release();
}

void BaseAgentAction::Init(IAIAction* pAct)
{
	m_pOwnerAct = pAct;
}

void BaseAgentAction::Release()
{
	m_pOwnerAct = nullptr;
}

AgentActionType BaseAgentAction::GetType() const
{
	return AgentActionType();
}
