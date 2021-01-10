#include "stdafx.h"
#include "ActionPlanner.h"

ActionPlanner::~ActionPlanner()
{
}

void ActionPlanner::Initialise()
{
	//m_states.resize(AP_MAX_STATES);
	m_actions.resize(AP_MAX_ACTIONS);
}

void ActionPlanner::Clear()
{
	//m_states.clear();
	m_actions.clear();
}

void ActionPlanner::Release()
{
	Clear();
}

bool ActionPlanner::SetWorldModel(AIWorldState* pWModel, const char* atomName, bool value)
{
	m_wm = g_aiSystem->GetWorldModel();
	return true;
}

bool ActionPlanner::AddActionPrecondition(const char* actionName, const char* atomName, bool value)
{
	return true;
}

bool ActionPlanner::AddActionEffect(const char* actionName, const char* atomName, bool value)
{
	return true;
}

bool ActionPlanner::SetActionCost(const char* actionName, int cost)
{
	return true;
}

std::queue<AIAction> ActionPlanner::Plan(const AIWorldModel& wm, AIGoal* pGoal)
{
	return std::queue<AIAction>();
}

#ifdef _DEBUG
void ActionPlanner::PrintDebugInfo()
{
}
#endif
