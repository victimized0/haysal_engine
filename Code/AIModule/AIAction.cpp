#include "stdafx.h"
#include "AIAction.h"


AIAction::AIAction()
	: m_cost(0)
{
}

AIAction::~AIAction()
{
	m_effects.clear();
	m_preconds.clear();
}

void AIAction::AddPrecond(const char* stateName, bool stateValue)
{
	m_preconds.insert({ stateName, stateValue });
}

void AIAction::AddEffect(const char* stateName, bool stateValue)
{
	m_effects.insert({ stateName, stateValue });
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

		AddPrecond(state_name, state_value);
	}

	auto effects = actionNode.child("effects");
	for (xml_node stateNode = effects.first_child(); stateNode; stateNode = stateNode.next_sibling())
	{
		if (std::string(stateNode.name()) != "state")
			continue;

		const char* state_name	= stateNode.attribute("name").as_string();
		bool		state_value = stateNode.attribute("value").as_bool();

		AddEffect(state_name, state_value);
	}
}
