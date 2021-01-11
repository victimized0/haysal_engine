#include "stdafx.h"
#include "LocomotionActions.h"

#include "RenderModule\IRenderNode.h"

// ---------------------------------------------------------
// AgentIdleAction
// ---------------------------------------------------------

AgentIdleAction::AgentIdleAction()
{
}

AgentIdleAction::~AgentIdleAction()
{
}

void AgentIdleAction::Init(IAIAction* pAct)
{
	BaseAgentAction::Init(pAct);
}

void AgentIdleAction::Release()
{
	BaseAgentAction::Release();
}

void AgentIdleAction::Execute(IAIAgent* pAgent)
{
	// TODO: Delete this action, seems to be needless
}

// ---------------------------------------------------------
// AgentMoveToAction
// ---------------------------------------------------------

AgentMoveToAction::AgentMoveToAction()
{
}

AgentMoveToAction::~AgentMoveToAction()
{
}

void AgentMoveToAction::Init(IAIAction* pAct)
{
	BaseAgentAction::Init(pAct);
}

void AgentMoveToAction::Release()
{
	BaseAgentAction::Release();
}

void AgentMoveToAction::Execute(IAIAgent* pAgent)
{
	assert(pAgent);
	if (!pAgent)
		return;

	IEntity* pEnt = pAgent->GetOwner();
	if (!pEnt) return;

	AIStimulus* pStimulus = pAgent->GetStimulus();
	if (!pStimulus) return;

	Vec3 speed(0.1f, 0.1f, 0.1f); // TODO: Speed needs to come from Physics
	pEnt->Translate(speed);

	//pEnt->PlayAnimation(Walk, loop: true);

	if (pEnt->GetRenderNode()->GetAABB().Intersects(pStimulus->pTarget->GetRenderNode()->GetAABB()))
	{
		//pEnt->StopAnimation(Walk);
		m_isCompleted = true;
	}
}

// ---------------------------------------------------------
// AgentRunToAction
// ---------------------------------------------------------

AgentRunToAction::AgentRunToAction()
{
}

AgentRunToAction::~AgentRunToAction()
{
}

void AgentRunToAction::Init(IAIAction* pAct)
{
	BaseAgentAction::Init(pAct);
}

void AgentRunToAction::Release()
{
	BaseAgentAction::Release();
}

void AgentRunToAction::Execute(IAIAgent* pAgent)
{
	assert(pAgent);
	if (!pAgent)
		return;

	IEntity* pEnt = pAgent->GetOwner();
	if (!pEnt) return;

	AIStimulus* pStimulus = pAgent->GetStimulus();
	if (!pStimulus) return;

	Vec3 speed(1.0f, 1.0f, 1.0f); // TODO: Speed and acceleration need to come from Physics
	pEnt->Translate(speed);

	//pEnt->PlayAnimation(Run, loop: true);

	if (pEnt->GetRenderNode()->GetAABB().Intersects(pStimulus->pTarget->GetRenderNode()->GetAABB()))
	{
		//pEnt->StopAnimation(Run);
		m_isCompleted = true;
	}
}

// ---------------------------------------------------------
// AgentJumpAction
// ---------------------------------------------------------

AgentJumpAction::AgentJumpAction()
{
}

AgentJumpAction::~AgentJumpAction()
{
}

void AgentJumpAction::Init(IAIAction* pAct)
{
	BaseAgentAction::Init(pAct);
}

void AgentJumpAction::Release()
{
	BaseAgentAction::Release();
}

void AgentJumpAction::Execute(IAIAgent* pAgent)
{
	assert(pAgent);
	if (!pAgent)
		return;

	//if (IEntity* pEnt = pAgent->GetOwner())
	//	pEnt->PlayAnimation(Jump, loop: false);

	m_isCompleted = true;
}

// ---------------------------------------------------------
// AgentPickUpAction
// ---------------------------------------------------------

AgentPickUpAction::AgentPickUpAction()
{
}

AgentPickUpAction::~AgentPickUpAction()
{
}

void AgentPickUpAction::Init(IAIAction* pAct)
{
	BaseAgentAction::Init(pAct);
}

void AgentPickUpAction::Release()
{
	BaseAgentAction::Release();
}

void AgentPickUpAction::Execute(IAIAgent* pAgent)
{
	assert(pAgent);
	if (!pAgent)
		return;

	//if (IEntity* pEnt = pAgent->GetOwner())
	//	pEnt->PlayAnimation(PickUp, loop: false);

	m_isCompleted = true;
}

// ---------------------------------------------------------
// AgentFleeAction
// ---------------------------------------------------------

AgentFleeAction::AgentFleeAction()
{
}

AgentFleeAction::~AgentFleeAction()
{
}

void AgentFleeAction::Init(IAIAction* pAct)
{
	BaseAgentAction::Init(pAct);
}

void AgentFleeAction::Release()
{
	BaseAgentAction::Release();
}

void AgentFleeAction::Execute(IAIAgent* pAgent)
{
	// Reverse of MoveTo
}

// ---------------------------------------------------------
// AgentScoutAction
// ---------------------------------------------------------

AgentScoutAction::AgentScoutAction()
{
}

AgentScoutAction::~AgentScoutAction()
{
}

void AgentScoutAction::Init(IAIAction* pAct)
{
	BaseAgentAction::Init(pAct);
}

void AgentScoutAction::Release()
{
	BaseAgentAction::Release();
}

void AgentScoutAction::Execute(IAIAgent* pAgent)
{
	assert(pAgent);
	if (!pAgent)
		return;

	IEntity* pEnt = pAgent->GetOwner();
	if (!pEnt) return;

	AIStimulus* pStimulus = pAgent->GetStimulus();
	if (!pStimulus) return;

	Vec3 speed(0.1f, 0.1f, 0.1f); // TODO: Speed needs to come from Physics
	pEnt->Translate(speed);

	//pEnt->PlayAnimation(Walk, loop: true);

	if (pEnt->GetRenderNode()->GetAABB().Contains(pStimulus->Pos))
	{
		//pEnt->StopAnimation(Walk);
		m_isCompleted = true;
	}
}
