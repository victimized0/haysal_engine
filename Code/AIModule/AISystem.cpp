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
	m_agentActions.insert({ AgentActionType::Scout,		new AgentScoutAction() });
	m_agentActions.insert({ AgentActionType::Flee,		new AgentFleeAction() });

	m_agentActions.insert({ AgentActionType::Aim,		new AgentAimAction() });
	m_agentActions.insert({ AgentActionType::Shoot,		new AgentShootAction() });
	m_agentActions.insert({ AgentActionType::Reload,	new AgentReloadAction() });

	m_agentActions.insert({ AgentActionType::Punch,		new AgentPunchAction() });
	m_agentActions.insert({ AgentActionType::Kick,		new AgentKickAction() });
}

void AISystem::Release()
{
	for (auto& pAgentAction : m_agentActions)
		SAFE_DELETE(pAgentAction.second);
	m_agentActions.clear();
}

void AISystem::ProcessStimulus(AIStimulus stimulus)
{
	for (auto& agent : m_agents)
	{
		if (agent.PerceptsStimulus(stimulus.Type))
			agent.ProcessStimulus(stimulus);
	}
}

IAIAgent* AISystem::CreateAgent()
{
	m_agents.push_back( { nullptr } );
	IAIAgent* pAgent = &m_agents.back();
	return pAgent;
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
