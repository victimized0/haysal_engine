#ifndef INTERFACE_AI_SYSTEM_H
#define INTERFACE_AI_SYSTEM_H
#pragma once

struct AIWorldState;
struct IAIAction;
struct AgentDesc;
struct IAIAgent;

typedef AIWorldState AIGoal;
typedef std::vector<AIWorldState> AIWorldModel;

enum class AgentActionType : uint32
{
	Idle = 0,
	MoveTo,
	RunTo,
	Jump,
	PickUp,
	Flee,
	Scout,
	
	Aim,
	Shoot,
	Reload,

	Punch,
	Kick,

	AACount
};

enum class AgentStance
{
	Idle,
	Moving,
	Attacking
};

struct IAgentAction
{
	virtual					~IAgentAction() {}
	virtual void			Init(IAIAction* pAct) = 0;
	virtual void			Release() = 0;
	virtual void			Execute(IAIAgent* pAgent) = 0;
	virtual AgentActionType	GetType() const = 0;
	virtual bool			IsCompleted() const = 0;
	virtual void			OnComplete() = 0;
};

struct IAIAgent
{
	virtual					~IAIAgent() {}

	virtual void			Init() = 0;
	virtual void			Release() = 0;
	virtual void			Update(float dt) = 0;

	virtual void			AddAction(IAIAction* pAction) = 0;
	virtual void			AddGoal(AIGoal* pGoal) = 0;
	virtual void			SetGoal(AIGoal* pGoal) = 0;

	virtual void			Parse(pugi::xml_node& agentNode, AgentDesc& agentDesc) = 0;
	virtual AgentStance		GetStance() const = 0;
	//virtual void	Serialise() = 0;
};

struct AIWorldState
{
	inline void Initialise()
	{
		Values		= 0;
		IgnoreList	= -1;
		Value		= false;
	}

	inline void Parse(pugi::xml_node& stateNode)
	{
		using namespace pugi;
		if (std::string(stateNode.name()) != "state")
			return;

		Name	= stateNode.attribute("name").as_string();
		Value	= stateNode.attribute("value").as_bool();
	}

	int64		Values;
	int64		IgnoreList;

	std::string	Name;
	bool		Value;
};

struct IAIAction
{
	virtual					~IAIAction() {}
	virtual bool			IsCompleted() const = 0;
	virtual void			AddPrecond(AIWorldState& state) = 0;
	virtual void			AddEffect(AIWorldState& state) = 0;
	virtual void			SetCost(int cost) = 0;
	virtual void			SetName(const char* name) = 0;
	virtual const char*		GetName() = 0;
	virtual void			SetAgentAction(IAgentAction* pAct) = 0;
	virtual IAgentAction*	GetAgentAction() const = 0;
	virtual void			Parse(const pugi::xml_node& actNode) = 0;
	virtual const std::vector<AIWorldState>& GetEffects() const = 0;
	virtual const std::vector<AIWorldState>& GetPreconds() const = 0;
};

struct IAIActionPlanner
{
	virtual			~IAIActionPlanner() {}

	virtual void	Initialise() = 0;
	virtual void	Clear() = 0;
	virtual void	Release() = 0;

	virtual bool	SetWorldModel(AIWorldState* pWModel, const char* atomName, bool value) = 0;
	virtual bool	AddActionPrecondition(const char* actionName, const char* atomName, bool value) = 0;
	virtual bool	AddActionEffect(const char* actionName, const char* atomName, bool value) = 0;
	virtual bool	SetActionCost(const char* actionName, int cost) = 0;
	virtual std::queue<class AIAction> Plan(const AIWorldModel& wm, AIGoal* pGoal) = 0;


#ifdef _DEBUG
	virtual void	PrintDebugInfo() = 0;
#endif

};

struct AgentDesc
{
	std::vector<IAIAction*>	ActionsSet;
	std::vector<AIGoal*>	GoalsSet;
};

struct IAISystem
{
	virtual							~IAISystem() {}

	virtual void					Update() = 0;
	virtual void					Init() = 0;
	virtual void					Release() = 0;

	virtual IAIAgent*				CreateAgent() = 0;
	virtual void					DestroyAgent(IAIAgent* pAgent) = 0;

	virtual IAIAction*				GetAction(const char* name) = 0;
	virtual AIGoal*					GetGoal(const char* name) = 0;
	virtual const AIWorldModel&		GetWorldModel() const = 0;
	virtual void					ModifyWorldModel(const std::vector<AIWorldState>& effects) = 0;

	virtual AgentActionType			ActionNameToType(const char* name) = 0;
	virtual IAgentAction*			AgentActionFromName(const char* name) = 0;
	virtual IAgentAction*			AgentActionFromType(AgentActionType type) = 0;
};

#endif INTERFACE_AI_SYSTEM_H
