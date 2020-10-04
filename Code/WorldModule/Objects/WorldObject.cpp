#include "stdafx.h"
#include "WorldObject.h"

WorldObject::WorldObject()
{

}

WorldObject::~WorldObject()
{
}

Matrix& WorldObject::GetTransform()
{
	return m_transform;
}

Vec3 WorldObject::GetPosition()
{
	return m_transform.Translation();
}

Quat WorldObject::GetRotation()
{
	Vec3 s, p;
	Quat r;
	m_transform.Decompose(s, r, p);
	return r;
}

float WorldObject::GetScale()
{
	Vec3 s, p;
	Quat r;
	m_transform.Decompose(s, r, p);
	return r.x;
}

void WorldObject::Translate(Vec3 t)
{
	m_transform.Translate(t);
}

void WorldObject::Rotate(Quat r)
{
	m_transform = Matrix::Transform(m_transform, r);
}

void WorldObject::Scale(float s)
{
	// Not implemented
}
