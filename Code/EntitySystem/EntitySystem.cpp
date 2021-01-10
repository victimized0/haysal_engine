#include "stdafx.h"
#include "EntitySystem.h"

#include <System\ISystem.h>

EntitySystem::EntitySystem()
{
}

EntitySystem::~EntitySystem()
{
}

void EntitySystem::Init()
{
}

void EntitySystem::ShutDown()
{
}

void EntitySystem::Load()
{
	using namespace pugi;
	xml_document doc = gEnv->pSystem->LoadXmlFromFile("Data/Entities/EntitiesLib.xml");
	auto entRoot = doc.child("entities");

	for (xml_node entityNode = entRoot.first_child(); entityNode; entityNode = entityNode.next_sibling())
	{
		if (std::string(entityNode.name()) != "entity")
			continue;

		IEntity* pEntity = new Entity();
		pEntity->Parse(entityNode);
		m_entities.push_back(pEntity);
	}
}

void EntitySystem::Unload()
{
	for (auto* pEntity : m_entities)
	{
		SAFE_RELEASE(pEntity);
	}
	
	m_entities.clear();
}

IEntity* EntitySystem::LoadEntity(const char* name)
{
	using namespace pugi;
	xml_document doc = gEnv->pSystem->LoadXmlFromFile("Data/Entities/EntitiesLib.xml");
	auto entRoot = doc.child("entities");

	for (xml_node entityNode = entRoot.first_child(); entityNode; entityNode = entityNode.next_sibling())
	{
		if (std::string(entityNode.name()) != "entity")
			continue;

		if (entityNode.attribute("name").as_string() == std::string(name)) {
			IEntity* pEntity = new Entity();
			pEntity->Parse(entityNode);
			m_entities.push_back(pEntity);
			return pEntity;
		}
	}

	return nullptr;
}

IEntity* EntitySystem::GetEntity(const char* name)
{
	auto it = std::find_if( m_entities.begin(), m_entities.end(), [&](IEntity* pEnt) { return pEnt->GetName() == name; } );
	if (it != m_entities.end())
		return *it;
	return nullptr;
}
