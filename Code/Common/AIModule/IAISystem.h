#ifndef INTERFACE_AI_SYSTEM_H
#define INTERFACE_AI_SYSTEM_H
#pragma once

struct AIWorldState;
struct IAIAction;
struct IAIAgent;
struct AIStimulus;
struct IEntity;

typedef AIWorldState AIGoal;

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

enum class AIStimulusType
{
	Vision,
	Noise,

	Count
};

struct IAgentAction
{
	virtual					~IAgentAction() {}
	virtual void			Init(IAIAction* pAct) = 0;
	virtual void			Release() = 0;
	virtual void			Execute(IAIAgent* pAgent) = 0;
	virtual AgentActionType	GetType() const = 0;
	virtual bool			IsCompleted() const = 0;
};

struct IAIAgent
{
	virtual					~IAIAgent() {}

	virtual void			Init() = 0;
	virtual void			Release() = 0;
	virtual void			Update(float dt) = 0;
	
	virtual AIStimulus*		GetStimulus() = 0;
	virtual IEntity*		GetOwner() = 0;
	virtual void			SetOwner(IEntity* pOwner) = 0;

	virtual bool			PerceptsStimulus(AIStimulusType type) = 0;
	virtual void			ProcessStimulus(AIStimulus stimulus) = 0;

	virtual void			Parse(pugi::xml_node& agentNode) = 0;
	virtual AgentStance		GetStance() const = 0;
};

struct AIWorldState
{
	float						Priority;
	std::string					Name;
	std::map<std::string, bool>	Params;

	AIWorldState(const char* name)
		: Name(name)
		, Priority(0.0f)
	{}

	inline void SetValue(const char* paramName, const bool value)
	{
		Params[paramName] = value;
	}

	inline bool GetValue(const char* paramName) const
	{
		return Params.at(paramName);
	}

	inline bool MeetsGoal(const AIGoal& goal) const
	{
		for (const auto& kv : goal.Params)
		{
			if (this->Params.at(kv.first) != kv.second) 
				return false;
		}

		return true;
	}

	inline int DistanceTo(const AIGoal& goal) const
	{
		int dist = 0;
		for (const auto& kv : goal.Params) {
			auto it = this->Params.find(kv.first);
			if (it == this->Params.end() || it->second != kv.second)
				++dist;
		}

		return dist;
	}

	inline bool operator==(const AIWorldState& other)
	{
		for (const auto& kv : other.Params)
		{
			if (this->Params.at(kv.first) != kv.second)
				return false;
		}

		return true;
	}
};

struct IAIAction
{
	virtual					~IAIAction() {}
	virtual bool			IsCompleted() const = 0;
	virtual bool			IsWSConformPreconds(const AIWorldState& ws) const = 0;
	virtual AIWorldState	ApplyEffects(const AIWorldState& ws) const = 0;

	virtual void			AddPrecond(const char* name, bool value) = 0;
	virtual void			AddEffect(const char* name, bool value) = 0;

	virtual void			SetCost(int cost) = 0;
	virtual int				GetCost() = 0;

	virtual void			SetName(const char* name) = 0;
	virtual const char*		GetName() = 0;

	virtual void			SetAgentAction(IAgentAction* pAct) = 0;
	virtual IAgentAction*	GetAgentAction() const = 0;
};

struct IAIActionPlanner
{
	virtual			~IAIActionPlanner() {}

	virtual void	Initialise() = 0;
	virtual void	Clear() = 0;
	virtual void	Release() = 0;

	virtual int		Plan(const AIWorldState& start, const AIWorldState& goal, const std::vector<IAIAction*>& actions, std::vector<IAIAction*>& outPlan) = 0;
};

struct IAISystem
{
	virtual						~IAISystem() {}

	virtual void				Update() = 0;
	virtual void				Init() = 0;
	virtual void				Release() = 0;

	virtual void				ProcessStimulus(AIStimulus stimulus) = 0;

	virtual IAIAgent*			CreateAgent() = 0;
	virtual void				DestroyAgent(IAIAgent* pAgent) = 0;

	virtual AgentActionType		ActionNameToType(const char* name) = 0;
	virtual IAgentAction*		AgentActionFromName(const char* name) = 0;
	virtual IAgentAction*		AgentActionFromType(AgentActionType type) = 0;
};

struct AIStimulus
{
	AIStimulusType Type;
	Vec3 Pos;
	float Radius;
	IEntity* pTarget;
};

#endif INTERFACE_AI_SYSTEM_H
