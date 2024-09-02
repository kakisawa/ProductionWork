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

void Collision::DrawMain(const float radius, unsigned int color, bool isFill)
{
	DrawCapsule3D(m_pos, m_vertexPos,
		radius, 32, color, color, isFill);	// “–‚½‚è”»’è•`‰æ
}

void Collision::DrawAttack(const float radius, unsigned int color, bool isFill)
{
	DrawCapsule3D(m_pos, m_attackRange,
		radius, 32, color, color, isFill);	// “–‚½‚è”»’è•`‰æ
}

void Collision::UpdateCol(VECTOR pos, VECTOR upPos,VECTOR attackRange)
{
	m_pos = pos;
	m_vertexPos = upPos;
	m_attackRange = attackRange;
}

bool Collision::IsCollision(const Collision& col)
{
	return false;
}
