#include "stdafx.h"
#include "AIAgent.h"

#include <System\ISystem.h>

AIAgent::AIAgent()
	: m_pCurrentGoal(nullptr)
{
}

AIAgent::~AIAgent()
{
}

void AIAgent::Init()
{
}

void AIAgent::Release()
{
	m_actionsSet.clear();
	m_pCurrentGoal = nullptr; // Non-owning membership.
}

void AIAgent::AddAction(IAIAction *pAction)
{
	auto it = std::find(m_actionsSet.begin(), m_actionsSet.end(), pAction);
	if (it != m_actionsSet.end())
		return;	// Action is already included in this agent's actions set

	if (auto pActInst = reinterpret_cast<AIAction*>(pAction))
		m_actionsSet.push_back(pActInst);
}

void AIAgent::SetGoal(AIGoal* pNewGoal)
{
	if (m_pCurrentGoal != pNewGoal)
		m_pCurrentGoal = pNewGoal;
}

void AIAgent::Parse(pugi::xml_node& agentNode, AgentDesc& agentDesc)
{
    using namespace pugi;
    auto actionsSetNode = agentNode.child("actions_set");
    if (!actionsSetNode.empty())
    {
        for (xml_node actionNode = actionsSetNode.first_child(); actionNode; actionNode = actionNode.next_sibling())
        {
            if (std::string(actionNode.name()) != "action")
                continue;

            const char* act_name = actionNode.attribute("name").as_string();
            if (IAIAction* pAct = gEnv->pAISystem->GetAction(act_name))
                agentDesc.ActionsSet.push_back(pAct);
        }
    }

    auto goalsSetNode = agentNode.child("goals_set");
    if (!goalsSetNode.empty())
    {
        for (xml_node goalNode = goalsSetNode.first_child(); goalNode; goalNode = goalNode.next_sibling())
        {
            if (std::string(goalNode.name()) != "goal")
                continue;

            const char* goal_name = goalNode.attribute("name").as_string();
            if (AIGoal* pGoal = gEnv->pAISystem->GetGoal(goal_name))
                agentDesc.GoalsSet.push_back(pGoal);
        }
    }
}
