#pragma once
#include "DxLib.h"

class Collision
{
public:
	Collision();
	virtual ~Collision();

	// •`‰æ
	void DrawMain(const float radius,unsigned int color, bool isFill);
	void DrawAttack(const float radius, unsigned int color, bool isFill);

	void UpdateCol(VECTOR pos,VECTOR upPos, VECTOR attackRange);	// “–‚½‚è”»’è‚ÌXV

	bool IsCollision(const Collision& col);		// “–‚½‚è”»’è

private:
	VECTOR m_pos;
	VECTOR m_vertexPos;
	VECTOR m_attackRange;
};