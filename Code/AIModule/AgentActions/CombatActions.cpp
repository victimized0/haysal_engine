#include "stdafx.h"
#include "CombatActions.h"

// ---------------------------------------------------------
// AgentPunchAction
// ---------------------------------------------------------

AgentPunchAction::AgentPunchAction()
{
}

AgentPunchAction::~AgentPunchAction()
{
}

void AgentPunchAction::Init(IAIAction* pAct)
{
	BaseAgentAction::Init(pAct);
}

void AgentPunchAction::Release()
{
	BaseAgentAction::Release();
}

void AgentPunchAction::Execute(IAIAgent* pAgent)
{
}

// ---------------------------------------------------------
// AgentKickAction
// ---------------------------------------------------------

AgentKickAction::AgentKickAction()
{
}

AgentKickAction::~AgentKickAction()
{
}

void AgentKickAction::Init(IAIAction* pAct)
{
	BaseAgentAction::Init(pAct);
}

void AgentKickAction::Release()
{
}

void AgentKickAction::Execute(IAIAgent* pAgent)
{
}
