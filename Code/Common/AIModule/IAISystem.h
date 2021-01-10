#ifndef INTERFACE_AI_SYSTEM_H
#define INTERFACE_AI_SYSTEM_H
#pragma once

struct AIWorldState;
struct IAIAction;
struct AgentDesc;

typedef AIWorldState AIGoal;

struct IAIAgent
{
	virtual ~IAIAgent() {}

	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update(float dt) = 0;

	virtual void AddAction(IAIAction* pAction) = 0;
	virtual void AddGoal(AIGoal* pGoal) = 0;
	virtual void SetGoal(AIGoal* pGoal) = 0;

	virtual void Parse(pugi::xml_node& agentNode, AgentDesc& agentDesc) = 0;
	//virtual void Serialise() = 0;
};

struct AIWorldState
{
	inline void Initialise()
	{
		Values = 0;
		IgnoreList = -1;
	}

	inline void Parse(pugi::xml_node& stateNode)
	{
		using namespace pugi;
		if (std::string(stateNode.name()) != "state")
			return;

		const char* state_name	= stateNode.attribute("name").as_string();
		bool		state_value = stateNode.attribute("value").as_bool();
	}

	int64 Values;
	int64 IgnoreList;
};

struct IAIAction
{
	virtual				~IAIAction() {}
	virtual void		AddPrecond(const char* stateName, bool stateValue) = 0;
	virtual void		AddEffect(const char* stateName, bool stateValue) = 0;
	virtual void		SetCost(int cost) = 0;
	virtual void		SetName(const char* name) = 0;
	virtual const char*	GetName() = 0;
	virtual void		Parse(const pugi::xml_node& actNode) = 0;
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

	virtual std::vector<IAIAction> Plan(AIWorldState* pWM, AIGoal* pGoal) = 0;

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
	virtual				~IAISystem() {}

	virtual void		Update() = 0;
	virtual void		Init() = 0;
	virtual void		Release() = 0;

	virtual IAIAgent*	CreateAgent() = 0;
	virtual void		DestroyAgent(IAIAgent* pAgent) = 0;

	virtual IAIAction*	GetAction(const char* name) = 0;
	virtual AIGoal*		GetGoal(const char* name) = 0;
};

#endif INTERFACE_AI_SYSTEM_H
