#include "stdafx.h"
#include "AISystem.h"

#include <platform.inl>
#include <AIModule\IAIModule.h>
#include <Utils\StringUtils.h>
#include "AgentActions\LocomotionActions.h"
#include "AgentActions\ShooterActions.h"
#include "AgentActions\CombatActions.h"

AISystem* g_aiSystem = nullptr;

#ifdef USE_GOAP
AISystem::~AISystem()
{
	Release();
}

void AISystem::Update()
{
	for (auto& agent : m_agents)
	{
		agent.Update(0.0f);
	}
}

void AISystem::Init()
{
	m_agentActions.insert({ AgentActionType::Idle,		new AgentIdleAction() });
	m_agentActions.insert({ AgentActionType::MoveTo,	new AgentMoveToAction() });
	m_agentActions.insert({ AgentActionType::RunTo,		new AgentRunToAction() });
	m_agentActions.insert({ AgentActionType::Jump,		new AgentJumpAction() });
	m_agentActions.insert({ AgentActionType::PickUp,	new AgentPickUpAction() });

	m_agentActions.insert({ AgentActionType::Aim,		new AgentAimAction() });
	m_agentActions.insert({ AgentActionType::Shoot,		new AgentShootAction() });
	m_agentActions.insert({ AgentActionType::Reload,	new AgentReloadAction() });

	m_agentActions.insert({ AgentActionType::Punch,		new AgentPunchAction() });
	m_agentActions.insert({ AgentActionType::Kick,		new AgentKickAction() });

	LoadStates();
	LoadActions();
	LoadGoals();
}

void AISystem::Release()
{
	for (auto pAgentAction : m_agentActions)
		SAFE_DELETE(pAgentAction.second);
	m_agentActions.clear();
}

IAIAgent* AISystem::CreateAgent()
{
	m_agents.push_back( {} );
	return &m_agents.back();
}

void AISystem::DestroyAgent(IAIAgent* pAgent)
{
	auto it = std::find_if(m_agents.begin(), m_agents.end(), [&](AIAgent& agent) { return &agent == pAgent; });
	if (it != m_agents.end())
	{
		m_agents.erase(it);
		SAFE_RELEASE(pAgent);
	}
}

IAIAction* AISystem::GetAction(const char* name)
{
	auto it = std::find_if(m_goapActions.begin(), m_goapActions.end(), [&](AIAction& act) { return strcmp(act.GetName(), name) == 0; });
	if (it != m_goapActions.end())
		return &(*it);
	return nullptr;
}

AIGoal* AISystem::GetGoal(const char* name)
{
	assert(false); // Not implemented
	//auto it = std::find_if(m_goapGoals.begin(), m_goapGoals.end(), [&](AIGoal& goal) { strcmp(goal.GetName(), name) == 0 });
	//if (it != m_goapGoals.end())
	//	return &(*it);
	return nullptr;
}

AgentActionType AISystem::ActionNameToType(const char* name)
{
	std::string actionName(name);
	const char* actName = ToLower(actionName).c_str();

	if (strcmp(actName, "idle") == 0)
		return AgentActionType::Idle;
	if (strcmp(actName, "moveto") == 0)
		return AgentActionType::MoveTo;
	if (strcmp(actName, "runto") == 0)
		return AgentActionType::RunTo;
	if (strcmp(actName, "jump") == 0)
		return AgentActionType::Jump;
	if (strcmp(actName, "pickup") == 0)
		return AgentActionType::PickUp;
	if (strcmp(actName, "flee") == 0)
		return AgentActionType::Flee;
	if (strcmp(actName, "scout") == 0)
		return AgentActionType::Scout;

	if (strcmp(actName, "aim") == 0)
		return AgentActionType::Aim;
	if (strcmp(actName, "shoot") == 0)
		return AgentActionType::Shoot;
	if (strcmp(actName, "reload") == 0)
		return AgentActionType::Reload;

	if (strcmp(actName, "punch") == 0)
		return AgentActionType::Punch;
	if (strcmp(actName, "kick") == 0)
		return AgentActionType::Kick;

	return AgentActionType::Idle;
}

IAgentAction* AISystem::AgentActionFromName(const char* name)
{
	AgentActionType type = ActionNameToType(name);
	return AgentActionFromType(type);
}

IAgentAction* AISystem::AgentActionFromType(AgentActionType type)
{
	return m_agentActions[type];
}

void AISystem::ModifyWorldModel(const std::vector<AIWorldState>& effects)
{
	for (const auto& effect : effects)
	{
		auto it = std::find_if(m_worldModel.begin(), m_worldModel.end(), [&](AIWorldState& state) { return state.Name == effect.Name; });
		if (it != m_worldModel.end())
		{
			it->Value = effect.Value;
		}
	}
}

void AISystem::LoadStates()
{
	using namespace pugi;
	xml_document doc = gEnv->pSystem->LoadXmlFromFile("Data/AI/states.xml");
	auto statesRoot = doc.child("world_states");

	for (xml_node stateNode = statesRoot.first_child(); stateNode; stateNode = stateNode.next_sibling())
	{
		if (std::string(stateNode.name()) != "state")
			continue;

		const char*	act_name	= stateNode.attribute("name").as_string();
		int			act_cost	= stateNode.attribute("cost").as_int(1);

		//m_goapStates.push_back(action);
	}
}

void AISystem::LoadActions()
{
	using namespace pugi;
	xml_document doc = gEnv->pSystem->LoadXmlFromFile("Data/AI/actions.xml");
	auto actionsRoot = doc.child("goap_actions");

	for (xml_node actionNode = actionsRoot.first_child(); actionNode; actionNode = actionNode.next_sibling())
	{
		if (std::string(actionNode.name()) != "action")
			continue;

		const char*	act_name	= actionNode.attribute("name").as_string();
		int			act_cost	= actionNode.attribute("cost").as_int(1);

		AIAction action = {};
		action.SetName(act_name);
		action.SetCost(act_cost);
		action.Parse(actionNode);
		action.SetAgentAction(AgentActionFromName(act_name));

		m_goapActions.push_back(action);
	}
}

void AISystem::LoadGoals()
{
	using namespace pugi;

}
#endif //USE_GOAP

class AIModule final : public IAIModule
{
public:
	virtual			~AIModule() { SAFE_RELEASE(gEnv->pAISystem); }
	const char*		GetName() const final { return "AIModule"; }
	bool			Initialize(Environment& env, const SystemInitParams& initParams) final
	{
		g_aiSystem = new AISystem();
		ISystem* pSystem = env.pSystem;
		env.pAISystem = g_aiSystem;
		return env.pAISystem != nullptr;
	}
};

extern "C" DLL_EXPORT IEngineModule * CreateModule(ISystem * pSystem)
{
	auto pModule = new AIModule();
	// TODO: Subscribe to system events
	return pModule;
}
