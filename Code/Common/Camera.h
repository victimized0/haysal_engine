#ifndef CAMERA_H
#define CAMERA_H
#pragma once

#include "Math\SimpleMath.h"
#include <algorithm>

class Camera
{
private:
	typedef DirectX::SimpleMath::Vector2 Vec2;
	typedef DirectX::SimpleMath::Vector3 Vec3;
	typedef DirectX::SimpleMath::Vector4 Vec4;
	typedef DirectX::SimpleMath::Quaternion Quat;
	typedef DirectX::SimpleMath::Matrix Matrix;
	typedef DirectX::BoundingBox AABB;

public:
	Camera()
		: Target(0.0f, 1.0f, 0.0f)
		, Radius(50.0f)
		, m_width(0)
		, m_height(0)
		, m_fov(g_PIdiv3)
		, m_theta(g_PIdiv2)
		, m_phi(g_PIdiv2)
		, m_nearZ(0.1f)
		, m_farZ(1000.0f)
		, m_projRatio(0.0f)
	{ }

	inline void		Move(float d)
	{
		Target.y += d;
	}

	inline void		Rotate(float angleX, float angleY)
	{
		m_theta	+= angleX;
		m_phi	+= angleY;
		m_phi	= std::clamp(m_phi, 0.01f, g_PI - 0.01f);
	}

	inline void		Zoom(float d)
	{
		Radius += d;
	}

	inline void		CalculateMatrices();
	inline void		SetFrustum(int w, int h, float fov, float nearZ, float farZ);

	inline Matrix	GetView() const { return m_viewMtx; }
	inline Matrix	GetProj(int width, int height) const { return m_projMtx; }
	inline Vec3		GetPosition() const;

	inline int      GetWidth() const { return m_width; }
	inline int      GetHeight() const { return m_height; }
	inline float    GetFov() const { return m_fov; }
	inline float	GetNearPlane() const { return m_nearZ; }
	inline float	GetFarPlane() const { return m_farZ; }
	inline float	GetProjRatio() const { return m_projRatio; }

private:
	Matrix			m_viewMtx;
	Matrix			m_projMtx;

	int				m_width;
	int				m_height;
	float			m_fov;
	float			m_projRatio;
	float			m_nearZ;
	float			m_farZ;
	float			m_theta;
	float			m_phi;

public:
	float			Radius;
	Vec3			Target;

};

inline Camera::Vec3 Camera::GetPosition() const
{
	float sinphi	= sinf(m_phi);
	float costheta	= cosf(m_theta);
	float cosphi	= cosf(m_phi);
	float sintheta	= sinf(m_theta);

	return Vec3(Target.x + Radius * sinphi * costheta,	// X
				Target.y + Radius * cosphi,				// Y
				Target.z + Radius * sinphi * sintheta);	// Z
}

inline void Camera::CalculateMatrices()
{
	m_viewMtx = Matrix::CreateLookAt(GetPosition(), Target, Vec3(0,1,0));
	m_projMtx = Matrix::CreatePerspectiveFieldOfView(m_fov, m_projRatio, m_nearZ, m_farZ);
}

inline void Camera::SetFrustum(int w, int h, float fov, float nearZ, float farZ)
{
	m_width		= w;
	m_height	= h;
	m_fov		= fov;
	m_nearZ		= nearZ;
	m_farZ		= farZ;
	m_projRatio = static_cast<float>(m_width) / static_cast<float>(m_height);

	// TODO: Update Frustum when it comes to frustum culling implementation.
}

#endif //CAMERA_H
