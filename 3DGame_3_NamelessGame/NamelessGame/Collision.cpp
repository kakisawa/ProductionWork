#include "Collision.h"

namespace {
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// ベクトルの初期化
}

Collision::Collision():
	m_radius(0.0f),
	m_pos(kInitVec)
{
}

Collision::~Collision()
{
}

void Collision::Init()
{
	m_radius = 0.0f;
	m_pos = kInitVec;
}

void Collision::SphereUpdate(const VECTOR pos, const float radius)
{
	m_pos = pos;
	m_radius = radius;
}

void Collision::CapsuleUpdate(const VECTOR pos, const VECTOR upPos, const float radius)
{
	m_pos = pos;
	m_vertexPos = upPos;
	m_radius = radius;
}

void Collision::SphereDraw(unsigned int color, bool isFill)
{
	DrawSphere3D(m_pos, m_radius, 32, color, color, isFill);
}

void Collision::CapsuleDraw(unsigned int color, bool isFill)
{
	DrawCapsule3D(m_pos, m_vertexPos, m_radius, 32, color, color, isFill);
}

bool Collision::IsSphereCollision(const Collision& col)
{
	return HitCheck_Sphere_Sphere(m_pos, m_radius,
		col.m_pos, col.m_radius);
}

bool Collision::IsSphereToCapsuleCollision(const Collision& col)
{
	return HitCheck_Sphere_Capsule(col.m_pos, col.m_radius,
		m_pos, m_vertexPos, m_radius);
}