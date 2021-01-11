#ifndef INTERFACE_ENTITY_H
#define INTERFACE_ENTITY_H
#pragma once

struct IRenderNode;
struct IAIAgent;

struct IEntity
{
	virtual					~IEntity() {}
	virtual void			Release() = 0;

	virtual Vec3			GetWorldPos() = 0;
	virtual Vec3			GetLocalPos() = 0;
	virtual Quat			GetWorldRot() = 0;
	virtual Quat			GetLocalRot() = 0;
	virtual float			GetWorldScale() = 0;
	virtual float			GetLocalScale() = 0;
	virtual Matrix			GetWorldMatrix() = 0;
	virtual Matrix			GetLocalMatrix() = 0;

	virtual void			Translate(Vec3& delta) = 0;
	virtual void			Rotate(Quat& q) = 0;

	virtual const char*		GetName() = 0;

	//virtual PhysicsProxy* GetPhysicsProxy() = 0;
	virtual IRenderNode*	GetRenderNode() = 0;
	virtual IAIAgent*		GetAIAgent() = 0;

	virtual void			Parse(pugi::xml_node& entityNode) = 0;
	//virtual void Serialise() = 0;
};

#endif //INTERFACE_ENTITY_H
