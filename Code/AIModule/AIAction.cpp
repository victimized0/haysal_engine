#include "stdafx.h"
#include "AIAction.h"

AIAction::AIAction()
	: m_pAgentAction(nullptr)
	, m_cost(0)
{
}

AIAction::AIAction(std::string name, int cost)
	: m_pAgentAction(nullptr)
	, m_name(name)
	, m_cost(cost)
{
}

AIAction::~AIAction()
{
	m_effects.clear();
	m_preconds.clear();
}

AIWorldState AIAction::ApplyEffects(const AIWorldState& ws) const
{
	AIWorldState tmp(ws);
	for (const auto& effect : m_effects)
		tmp.SetValue(effect.first.c_str(), effect.second);
	return tmp;
}

void AIAction::AddPrecond(const char* name, bool value)
{
	m_preconds[name] = value;
}

void AIAction::AddEffect(const char* name, bool value)
{
	m_effects[name] = value;
}

void AIAction::SetCost(int cost)
{
	m_cost = cost;
}

void AIAction::SetName(const char* name)
{
	m_name = name;
}

bool AIAction::IsWSConformPreconds(const AIWorldState& ws) const
{
	for (const auto& precond : m_preconds)
	{
		if (ws.Params.at(precond.first) != precond.second)
			return false;
	}
	return true;
}
