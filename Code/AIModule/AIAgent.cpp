#include "stdafx.h"
#include "AIAgent.h"

#include <System\ISystem.h>

AIAgent::AIAgent(IEntity* pOwner)
    : m_pOwningEntity(pOwner)
	, m_pCurrentGoal(nullptr)
    , m_currentStance(AgentStance::Idle)
    , m_ws("Root")
{
}

AIAgent::~AIAgent()
{
    Release();
}

void AIAgent::Init()
{
    m_ap.Initialise();
    ProcessNoise({}); // TODO: Delete this
}

void AIAgent::Release()
{
    m_ap.Release();

    for (IAIAction* pAct : m_actionsSet)
        SAFE_DELETE(pAct);

	m_actionsSet.clear();
    m_goalsSet.clear();

	m_pCurrentGoal = nullptr;
    m_pOwningEntity = nullptr;
}

void AIAgent::Update(float dt)
{
    if (m_pCurrentGoal == nullptr)
        return;

    int pathSize = -1;
    if (m_pCurrentGoal && m_actionsPlan.empty())
    {
        pathSize = m_ap.Plan(m_ws, *m_pCurrentGoal, m_actionsSet, m_actionsPlan);
#ifdef _DEBUG
        OutputDebugStringA("AIAgent actions plan: \n");
        for (IAIAction* pAct : m_actionsPlan)
        {
            OutputDebugStringA(pAct->GetName());
            OutputDebugStringA("\n");
        }
#endif
    }

    IAIAction* pCurAct = nullptr;
    while (pCurAct == nullptr)
    {
        if (m_actionsPlan.empty())
        {
            m_pCurrentGoal = nullptr;
            break;
        }

        pCurAct = m_actionsPlan.back();
        if (pCurAct && pCurAct->IsCompleted())
        {
            m_actionsPlan.erase(m_actionsPlan.end() - 1);
            pCurAct = nullptr;
        }
    }

    if (pCurAct)
        pCurAct->GetAgentAction()->Execute(this);
}

bool AIAgent::PerceptsStimulus(AIStimulusType type)
{
    auto it = std::find(m_perceptions.begin(), m_perceptions.end(), type);
    return it != m_perceptions.end();
}

void AIAgent::ProcessStimulus(AIStimulus stimulus)
{
    switch (stimulus.Type)
    {
    case AIStimulusType::Vision:
        ProcessVision(stimulus);
        break;
    case AIStimulusType::Noise:
        ProcessNoise(stimulus);
        break;
    }
}

void AIAgent::Parse(pugi::xml_node& agentNode)
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
            int			act_cost = actionNode.attribute("cost").as_int(1);

            AIAction* pAct = new AIAction(act_name, act_cost);
            pAct->SetAgentAction(g_aiSystem->AgentActionFromName(act_name));

            auto precondNodes = actionNode.child("preconditions");
            for (xml_node paramNode = precondNodes.first_child(); paramNode; paramNode = paramNode.next_sibling())
            {
                if (std::string(paramNode.name()) != "param")
                    continue;

                const char* name    = paramNode.attribute("name").as_string();
                int			value   = paramNode.attribute("value").as_bool();

                pAct->AddPrecond(name, value);
            }

            auto effectNodes = actionNode.child("effects");
            for (xml_node paramNode = effectNodes.first_child(); paramNode; paramNode = paramNode.next_sibling())
            {
                if (std::string(paramNode.name()) != "param")
                    continue;

                const char* name    = paramNode.attribute("name").as_string();
                int			value   = paramNode.attribute("value").as_bool();

                pAct->AddEffect(name, value);
            }

            m_actionsSet.push_back(pAct);
        }
    }

    auto goalsSetNode = agentNode.child("goals_set");
    if (!goalsSetNode.empty())
    {
        for (xml_node goalNode = goalsSetNode.first_child(); goalNode; goalNode = goalNode.next_sibling())
        {
            if (std::string(goalNode.name()) != "goal")
                continue;

            const char* goal_name   = goalNode.attribute("name").as_string();
            float goal_priority     = goalNode.attribute("priority").as_float();
            AIGoal goal(goal_name);
            goal.Priority = goal_priority;

            for (xml_node paramNode = goalNode.first_child(); paramNode; paramNode = paramNode.next_sibling())
            {
                if (std::string(paramNode.name()) != "param")
                    continue;

                const char* name    = paramNode.attribute("name").as_string();
                int			value   = paramNode.attribute("value").as_bool();

                goal.SetValue(name, value);
            }

            m_goalsSet.push_back(goal);
        }
    }

    auto wsSetNode = agentNode.child("world_states_set");
    if (!wsSetNode.empty())
    {
        for (xml_node paramNode = wsSetNode.first_child(); paramNode; paramNode = paramNode.next_sibling())
        {
            if (std::string(paramNode.name()) != "param")
                continue;

            const char* name    = paramNode.attribute("name").as_string();
            int			value   = paramNode.attribute("value").as_bool();

            // TODO: value should be retrieved from an entire world model (from AISystem, for example?)
            m_ws.SetValue(name, value);
        }
    }
}

void AIAgent::ProcessVision(AIStimulus stimulus)
{
    m_curStimulus = stimulus;

    // Hardcoded for now
    auto it = std::find_if(m_goalsSet.begin(), m_goalsSet.end(), [](const AIGoal& goal) { return strcmp(goal.Name.c_str(), "kill_enemy") == 0 ; });
    if (it != m_goalsSet.end())
        m_pCurrentGoal = &*it;
}

void AIAgent::ProcessNoise(AIStimulus stimulus)
{
    m_curStimulus = stimulus;

    // Hardcoded for now
    auto it = std::find_if(m_goalsSet.begin(), m_goalsSet.end(), [](const AIGoal& goal) { return strcmp(goal.Name.c_str(), "kill_enemy") == 0; });
    if (it != m_goalsSet.end())
        m_pCurrentGoal = &*it;
}
