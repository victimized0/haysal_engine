#ifndef ENTITY_SYSTEM_H
#define ENTITY_SYSTEM_H
#pragma once

#include <EntitySystem\IEntitySystem.h>
#include "Entity.h"

class EntitySystem : public IEntitySystem
{
public:
							EntitySystem();
	virtual					~EntitySystem();

	virtual void			Init() override;
	virtual void			ShutDown() override;

	virtual void			Load() override;
	virtual void			Unload() override;

	virtual IEntity*		LoadEntity(const char* name) override;
	virtual IEntity*		GetEntity(const char* name) override;

private:
	std::vector<IEntity*>	m_entities; // TODO: Load entities only on demand

};

#endif //ENTITY_SYSTEM_H
