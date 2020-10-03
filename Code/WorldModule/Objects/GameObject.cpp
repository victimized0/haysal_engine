#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{
}

Matrix& GameObject::GetTransform()
{
	return m_transform;
}

Vec3 GameObject::GetPosition()
{
	return m_transform.Translation();
}

Quat GameObject::GetRotation()
{
	Vec3 s, p;
	Quat r;
	m_transform.Decompose(s, r, p);
	return r;
}

float GameObject::GetScale()
{
	Vec3 s, p;
	Quat r;
	m_transform.Decompose(s, r, p);
	return r.x;
}

void GameObject::Translate(Vec3 t)
{
	m_transform.Translate(t);
}

void GameObject::Rotate(Quat r)
{
	m_transform = Matrix::Transform(m_transform, r);
}

void GameObject::Scale(float s)
{
	// Not implemented
}
