#include "stdafx.h"
#include "ActionPlanner.h"

int AStarNode::s_count = -1;

ActionPlanner::~ActionPlanner()
{
}

void ActionPlanner::Initialise()
{
}

void ActionPlanner::Clear()
{
}

void ActionPlanner::Release()
{
	Clear();
}

int ActionPlanner::Plan(const AIWorldState& start, const AIWorldState& goal, const std::vector<IAIAction*>& actions, std::vector<IAIAction*>& outPlan)
{
    if (start.MeetsGoal(goal))
        return -1; // World state is already in goal state!

    m_lstFrontier.clear();
    m_lstVisited.clear();

    AStarNode nodeStart(start, 0, start.DistanceTo(goal), 0, nullptr);
    m_lstFrontier.push_back( std::move(nodeStart) );

    while (m_lstFrontier.size() > 0)
    {
        AStarNode& curNode(PopFromFrontier());
        int planSize = 0;

        if (curNode.WS.MeetsGoal(goal))
        {
            do
            {
                outPlan.push_back(curNode.pAct);

                auto it = std::find_if(m_lstFrontier.begin(), m_lstFrontier.end(), [&](const AStarNode& node) { return node.Id == curNode.ParentId; });
                if (it == m_lstFrontier.end()) 
                    it = std::find_if(m_lstVisited.begin(), m_lstVisited.end(), [&](const AStarNode& node) { return node.Id == curNode.ParentId; });

                curNode = *it;
                ++planSize;
            } while (curNode.ParentId != 0);

            return planSize;
        }

        int outcomeNum = 0;
        for (const auto& act : actions)
        {
            if (act->IsWSConformPreconds(curNode.WS))
            {
                AIWorldState outcomeWS = act->ApplyEffects(curNode.WS);
                outcomeWS.Name = std::string("Outcome") + std::to_string(++outcomeNum);
                if (IsVisited(outcomeWS))
                    continue;

                auto outcomeIt = std::find_if(m_lstFrontier.begin(), m_lstFrontier.end(), [&](const AStarNode& node) { return node.WS.Params == outcomeWS.Params; });
                if (outcomeIt == m_lstFrontier.end())
                {
                    AStarNode outcomeNode(outcomeWS, curNode.G + act->GetCost(), outcomeWS.DistanceTo(goal), curNode.Id, act);
                    PushToFrontier(std::move(outcomeNode));
                }
                else
                {
                    if (curNode.G + act->GetCost() < outcomeIt->G)
                    {
                        outcomeIt->G        = curNode.G + act->GetCost();
                        outcomeIt->H        = outcomeWS.DistanceTo(goal);
                        outcomeIt->pAct     = act;
                        outcomeIt->ParentId = curNode.Id;

                        std::sort(m_lstFrontier.begin(), m_lstFrontier.end());
                    }
                }
            }
        }
    }

    return -1; // No path found!
}

bool ActionPlanner::IsVisited(const AIWorldState& ws)
{
	auto it = std::find_if(m_lstVisited.begin(), m_lstVisited.end(), [&](const AStarNode& node) { return node.WS.Params == ws.Params; });
	return it != m_lstVisited.end();
}

AStarNode& ActionPlanner::PopFromFrontier()
{
	assert(!m_lstFrontier.empty());
	m_lstVisited.push_back(std::move(m_lstFrontier.front()));
	m_lstFrontier.erase(m_lstFrontier.begin());
	return m_lstVisited.back();
}

void ActionPlanner::PushToFrontier(AStarNode&& node)
{
	auto it = std::lower_bound(m_lstFrontier.begin(), m_lstFrontier.end(), node);
	m_lstFrontier.emplace(it, std::move(node));
}
