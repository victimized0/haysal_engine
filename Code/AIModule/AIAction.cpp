#include "stdafx.h"
#include "AIAction.h"

AIAction::AIAction()
	: m_pAgentAction(nullptr)
	, m_cost(0)
{
}

AIAction::~AIAction()
{
	m_effects.clear();
	m_preconds.clear();
}

void AIAction::AddPrecond(AIWorldState& state)
{
	m_preconds.push_back(state);
}

void AIAction::AddEffect(AIWorldState& state)
{
	m_effects.push_back(state);
}

void AIAction::SetCost(int cost)
{
	m_cost = cost;
}

void AIAction::SetName(const char* name)
{
	m_name = name;
}

void AIAction::Parse(const pugi::xml_node& actionNode)
{
	using namespace pugi;

	auto preconds = actionNode.child("preconditions");
	for (xml_node stateNode = preconds.first_child(); stateNode; stateNode = stateNode.next_sibling())
	{
		if (std::string(stateNode.name()) != "state")
			continue;

		const char* state_name	= stateNode.attribute("name").as_string();
		bool		state_value = stateNode.attribute("value").as_bool();

		AIWorldState state = {};
		state.Name	= state_name;
		state.Value = state_value;

		AddPrecond(state);
	}

	auto effects = actionNode.child("effects");
	for (xml_node stateNode = effects.first_child(); stateNode; stateNode = stateNode.next_sibling())
	{
		if (std::string(stateNode.name()) != "state")
			continue;

		const char* state_name	= stateNode.attribute("name").as_string();
		bool		state_value = stateNode.attribute("value").as_bool();

		AIWorldState state = {};
		state.Name	= state_name;
		state.Value = state_value;

		AddEffect(state);
	}
}
