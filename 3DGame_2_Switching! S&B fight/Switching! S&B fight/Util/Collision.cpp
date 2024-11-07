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

void Collision::Init()
{
	m_pos = kInitVec;
	m_vertexPos = kInitVec;
	m_attackRange = kInitVec;
	m_colRadius = 0.0f;
	m_attackColRadius = 0.0f;
}

void Collision::DrawMain(unsigned int color, bool isFill)
{
	DrawCapsule3D(m_pos, m_vertexPos,
		m_colRadius, 32, color, color, isFill);	// 当たり判定描画
}

void Collision::DrawAttack(unsigned int color, bool isFill)
{
	DrawCapsule3D(VAdd(m_pos, VGet(0.0f, 3.0f, 0.0f)), VAdd(m_attackRange, VGet(0.0f, 3.0f, 0.0f)),
		m_attackColRadius, 32, color, color, isFill);	// 当たり判定描画
}

/// <summary>
/// // 当たり判定の更新
/// </summary>
/// <param name="pos">座標</param>
/// <param name="upPos">当たり判定頂点座標</param>
/// <param name="attackRange">攻撃範囲</param>
/// <param name="colRadius">当たり判定半径</param>
/// <param name="attackColRadius">攻撃範囲半径</param>
void Collision::UpdateCol(VECTOR pos, VECTOR upPos,VECTOR attackRange, float colRadius,float attackColRadius)
{
	m_pos = pos;
	m_vertexPos = upPos;
	m_attackRange = attackRange;
	m_colRadius = colRadius;
	m_attackColRadius = attackColRadius;
}

/// <summary>
/// 攻撃が当たったかどうかの判定
/// </summary>
/// <param name="col">もう片方の当たり判定</param>
/// <returns>攻撃が当たっていたらtrueを返す</returns>
bool Collision::IsAttackCollision(const Collision& col)
{
	return HitCheck_Capsule_Capsule(VAdd(m_pos, VGet(0.0f, 3.0f, 0.0f)),
		VAdd(m_attackRange, VGet(0.0f, 3.0f, 0.0f)), 
		m_attackColRadius,
		col.m_pos, col.m_vertexPos, col.m_colRadius);	
}

/// <summary>
/// 体が当たったかどうかの判定
/// </summary>
/// <param name="col">もう片方の当たり判定</param>
/// <returns>体にあたっていたらtrueを返す</returns>
bool Collision::IsBodyCollision(const Collision& col)
{
	return HitCheck_Capsule_Capsule(m_pos, m_vertexPos, m_colRadius,
		col.m_pos, col.m_vertexPos, col.m_colRadius);
}
