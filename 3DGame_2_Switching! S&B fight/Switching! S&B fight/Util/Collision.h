#pragma once
#include "DxLib.h"

class Collision
{
public:
	Collision();
	virtual ~Collision();

	// •`‰æ
	void DrawMain(unsigned int color, bool isFill);
	void DrawAttack(unsigned int color, bool isFill);

	void UpdateCol(VECTOR pos,VECTOR upPos, VECTOR attackRange, float colRadius, float attackColRadius);	// “–‚½‚è”»’è‚ÌXV

	bool IsAttackCollision(const Collision& col);		// “–‚½‚è”»’è
	bool IsBodyCollision(const Collision& col);		// ‘Ì‚Ì“–‚½‚è”»’è

private:
	float m_attackColRadius;
	float m_colRadius;

	VECTOR m_pos;
	VECTOR m_vertexPos;
	VECTOR m_attackRange;
};