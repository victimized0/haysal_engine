#ifndef ENTITY_H
#define ENTITY_H
#pragma once

#include <EntitySystem\IEntity.h>
#include <AIModule\IAISystem.h>

class Entity : public IEntity
{
public:
							Entity();
	virtual					~Entity();

	// Inherited via IEntity
	virtual void			Release() override;
	virtual const char*		GetName() override			{ return m_name.c_str(); }

	virtual Vec3			GetWorldPos() override;
	virtual Quat			GetWorldRot() override;
	virtual float			GetWorldScale() override;

	virtual Vec3			GetLocalPos() override		{ return m_pos; }
	virtual Quat			GetLocalRot() override		{ return m_rot; }
	virtual float			GetLocalScale() override	{ return m_scale; }

	virtual Matrix			GetWorldMatrix() override;
	virtual Matrix			GetLocalMatrix() override;

	virtual IRenderNode*	GetRenderNode() override    { return m_pRenderNode; }
	virtual IAIAgent*		GetAIAgent() override		{ return m_pAiAgent; }

	virtual void			Translate(Vec3& delta) override;
	virtual void			Rotate(Quat& q) override;

	virtual void			Parse(pugi::xml_node& entityNode) override;
	// ~Inherited via IEntity

private:
	std::string				m_name;
	Quat					m_rot;
	Vec3					m_pos;
	float					m_scale;

	IRenderNode*			m_pRenderNode;
	IAIAgent*				m_pAiAgent;

};

#endif //ENTITY_H
