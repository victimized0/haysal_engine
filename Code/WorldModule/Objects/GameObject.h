#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#pragma once

#include <WorldModule\IWorldObject.h>

class GameObject : public IWorldObject
{
public:
				GameObject();
	virtual		~GameObject();

				GameObject(GameObject const&)				= delete;	// Temporarily disable
				GameObject(GameObject&&)					= delete;	// Temporarily disable
				GameObject& operator=(GameObject const&)	= delete;	// Temporarily disable
				GameObject& operator=(GameObject&&)			= delete;	// Temporarily disable

	Matrix&		GetTransform() override;
	Vec3		GetPosition() override;
	Quat		GetRotation() override;
	float		GetScale() override;

	void		Translate(Vec3 t) override;
	void		Rotate(Quat r) override;
	void		Scale(float s) override;

private:
	Matrix		m_transform;
	
};

#endif //GAME_OBJECT_H
