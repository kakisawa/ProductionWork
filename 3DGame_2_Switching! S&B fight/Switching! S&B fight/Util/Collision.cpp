#include "Collision.h"

namespace {
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);
}

Collision::Collision():
	m_pos(kInitVec),
	m_vertexPos(kInitVec)
{
}

Collision::~Collision()
{
}

void Collision::DrawMain(unsigned int color, bool isFill)
{
	DrawCapsule3D(m_pos, m_vertexPos,
		m_colRadius, 32, color, color, isFill);	// “–‚½‚è”»’è•`‰æ
}

void Collision::DrawAttack(unsigned int color, bool isFill)
{
	DrawCapsule3D(VAdd(m_pos, VGet(0.0f, 3.0f, 0.0f)), VAdd(m_attackRange, VGet(0.0f, 3.0f, 0.0f)),
		m_attackColRadius, 32, color, color, isFill);	// “–‚½‚è”»’è•`‰æ
}

void Collision::UpdateCol(VECTOR pos, VECTOR upPos,VECTOR attackRange, float colRadius,float attackColRadius)
{
	m_pos = pos;
	m_vertexPos = upPos;
	m_attackRange = attackRange;
	m_colRadius = colRadius;
	m_attackColRadius = attackColRadius;
}

bool Collision::IsAttackCollision(const Collision& col)
{
	return HitCheck_Capsule_Capsule(VAdd(m_pos, VGet(0.0f, 3.0f, 0.0f)),
		VAdd(m_attackRange, VGet(0.0f, 3.0f, 0.0f)), 
		m_attackColRadius,
		col.m_pos, col.m_vertexPos, col.m_colRadius);	
}

bool Collision::IsBodyCollision(const Collision& col)
{
	return HitCheck_Capsule_Capsule(m_pos, m_vertexPos, m_colRadius,
		col.m_pos, col.m_vertexPos, col.m_colRadius);
}
