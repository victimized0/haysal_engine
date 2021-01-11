#include "stdafx.h"
#include "ShooterActions.h"

// ---------------------------------------------------------
// AgentAimAction
// ---------------------------------------------------------

AgentAimAction::AgentAimAction()
{
}

AgentAimAction::~AgentAimAction()
{
}

void AgentAimAction::Init(IAIAction* pAct)
{
	BaseAgentAction::Init(pAct);
}

void AgentAimAction::Release()
{
	BaseAgentAction::Release();
}

void AgentAimAction::Execute(IAIAgent* pAgent)
{
	assert(pAgent);
	if (!pAgent)
		return;

	IEntity* pEnt = pAgent->GetOwner();
	if (!pEnt) return;

	AIStimulus* pStimulus = pAgent->GetStimulus();
	if (!pStimulus) return;

	if (pStimulus->pTarget)
	{
		Vec3 lookat = pStimulus->pTarget->GetWorldPos() - pEnt->GetWorldPos();
		Quat dRot = Quat::CreateFromYawPitchRoll(lookat.x, lookat.y, lookat.z);
		pEnt->Rotate(dRot);
	}

	//pEnt->PlayAnimation(Aim, loop: false);
	m_isCompleted = true;
}

// ---------------------------------------------------------
// AgentShootAction
// ---------------------------------------------------------

AgentShootAction::AgentShootAction()
{
}

AgentShootAction::~AgentShootAction()
{
}

void AgentShootAction::Init(IAIAction* pAct)
{
	BaseAgentAction::Init(pAct);
}

void AgentShootAction::Release()
{
	BaseAgentAction::Release();
}

void AgentShootAction::Execute(IAIAgent* pAgent)
{
	assert(pAgent);
	if (!pAgent)
		return;

	IEntity* pEnt = pAgent->GetOwner();
	if (!pEnt) return;

	AIStimulus* pStimulus = pAgent->GetStimulus();
	if (!pStimulus) return;

	if (pStimulus->pTarget)
	{
		//pEnt->PlayAnimation(Shoot, loop: false);
		//pStimulus->pTarget->AsPlayer()->SetDamage(5.0f); // Hardcoded damage for now...
	}

	//if (pStimulus->pTarget->AsPlayer()->GetHealth() == 0.0f)
		m_isCompleted = true;
}

// ---------------------------------------------------------
// AgentReloadAction
// ---------------------------------------------------------

AgentReloadAction::AgentReloadAction()
{
}

AgentReloadAction::~AgentReloadAction()
{
}

void AgentReloadAction::Init(IAIAction* pAct)
{
	BaseAgentAction::Init(pAct);
}

void AgentReloadAction::Release()
{
	BaseAgentAction::Release();
}

void AgentReloadAction::Execute(IAIAgent* pAgent)
{
}
