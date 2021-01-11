#ifndef ACTION_PLANNER_H
#define ACTION_PLANNER_H
#pragma once

#define AP_MAX_STATES	64
#define AP_MAX_ACTIONS	64

struct AStarNode
{
	static int s_count;

	AIWorldModel	WS;
	IAIAction*		pAct;

	int				ParentId;
	int				Id;
	int				G;			// Cost
	int				H;			// Heuristic: recursion number aka number of nodes from start to this node

	AStarNode()
		: G(0)
		, H(0)
		, WS("Unnamed")
		, ParentId(0)
		, pAct(nullptr)
	{
		Id = ++s_count;
	}

	AStarNode(AIWorldModel state, int g, int h, int parentId, IAIAction* pAction)
		: WS(state)
		, G(g)
		, H(h)
		, ParentId(parentId)
		, pAct(pAction)
	{
		Id = ++s_count;
	}

	int F() const { return G + H; }

	inline bool operator<(const AStarNode& other)
	{
		return this->F() < other.F();
	}

	inline bool operator==(const AStarNode& other)
	{
		return this->Id == other.Id;
	}
};

class ActionPlanner final : public IAIActionPlanner
{
public:
										ActionPlanner() = default;
	virtual 							~ActionPlanner();

	// Inherited via IAIActionPlanner
	virtual void						Initialise()	final;
	virtual void						Clear()			final;
	virtual void						Release()		final;

	virtual int							Plan(const AIWorldModel& start, const AIWorldModel& goal, const std::vector<IAIAction*>& actions, std::vector<IAIAction*>& outPlan) final;
	// ~Inherited via IAIActionPlanner

private:
	bool								IsVisited(const AIWorldModel& ws);
	AStarNode&							PopFromFrontier();
	void								PushToFrontier(AStarNode&& node);

private:
	std::vector<AStarNode>				m_lstFrontier;
	std::vector<AStarNode>				m_lstVisited;
};

#endif //ACTION_PLANNER_H
