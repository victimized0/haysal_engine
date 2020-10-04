#ifndef INTERFACE_WORLD_OBJECT_H
#define INTERFACE_WORLD_OBJECT_H
#pragma once

struct InstanceInfo
{
	Vec3 Position;
	Quat Rotation;
	float Scale;

};

struct WorldObjectDescr
{

};

struct IWorldObject
{
	virtual				~IWorldObject() {}

	virtual Matrix&		GetTransform() = 0;
	virtual Vec3		GetPosition() = 0;
	virtual Quat		GetRotation() = 0;
	virtual float		GetScale() = 0;

	virtual void		Translate(Vec3 t) = 0;
	virtual void		Rotate(Quat r) = 0;
	virtual void		Scale(float s) = 0;

	//virtual UUID		GetId() = 0;
};

#endif //INTERFACE_WORLD_OBJECT_H
