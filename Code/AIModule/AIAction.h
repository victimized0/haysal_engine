#ifndef AI_ACTION_H
#define AI_ACTION_H
#pragma once

#include <AIModule\IAISystem.h>

class AIAction : public IAIAction
{
public:
							AIAction();
	virtual					~AIAction();

	// Inherited via IAIAction
	virtual void			AddPrecond	(const char* stateName, bool stateValue)	override;
	virtual void			AddEffect	(const char* stateName, bool stateValue)	override;
	virtual void			SetCost		(int cost)									override;
	virtual void			SetName		(const char* name)							override;
	virtual const char*		GetName		() override { return m_name.c_str(); }

	virtual void			Parse		(const pugi::xml_node& actNode)				override;
	// ~Inherited via IAIAction

private:
	std::string					m_name;
	std::map<std::string, bool>	m_preconds;
	std::map<std::string, bool>	m_effects;
	int							m_cost;
};

#endif //AI_ACTION_H
