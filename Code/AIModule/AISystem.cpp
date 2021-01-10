#include "stdafx.h"
#include "AISystem.h"

#ifdef USE_GOAP
AISystem::~AISystem()
{
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
	LoadStates();
	LoadActions();
	LoadGoals();
}

void AISystem::Release()
{
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
		IAISystem* pAISystem = new AISystem();
		ISystem* pSystem = env.pSystem;
		env.pAISystem = pAISystem;
		return env.pAISystem != nullptr;
	}
};

extern "C" DLL_EXPORT IEngineModule * CreateModule(ISystem * pSystem)
{
	auto pModule = new AIModule();
	// TODO: Subscribe to system events
	return pModule;
}
