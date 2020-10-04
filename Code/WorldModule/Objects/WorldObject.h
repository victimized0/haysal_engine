#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#pragma once

#include <WorldModule\IWorldObject.h>

class WorldObject : public IWorldObject
{
public:
				WorldObject();
	virtual		~WorldObject();

				WorldObject(WorldObject const&)				= delete;	// Temporarily disable
				WorldObject(WorldObject&&)					= delete;	// Temporarily disable
				WorldObject& operator=(WorldObject const&)	= delete;	// Temporarily disable
				WorldObject& operator=(WorldObject&&)		= delete;	// Temporarily disable

	Matrix&		GetTransform() override;
	Vec3		GetPosition() override;
	Quat		GetRotation() override;
	float		GetScale() override;

	void		Translate(Vec3 t) override;
	void		Rotate(Quat r) override;
	void		Scale(float s) override;

private:
	Matrix			m_transform;
	InstanceInfo	m_instInfo;
	
};

#endif //GAME_OBJECT_H
