#pragma once
#include "DxLib.h"

class Collision
{
public:
	Collision();
	virtual ~Collision();

	// �`��
	void DrawMain(unsigned int color, bool isFill);
	void DrawAttack(unsigned int color, bool isFill);

	void UpdateCol(VECTOR pos,VECTOR upPos, VECTOR attackRange, float colRadius, float attackColRadius);	// �����蔻��̍X�V

	bool IsAttackCollision(const Collision& col);		// �����蔻��
	bool IsBodyCollision(const Collision& col);		// �̂̓����蔻��

private:
	float m_attackColRadius;
	float m_colRadius;

	VECTOR m_pos;
	VECTOR m_vertexPos;
	VECTOR m_attackRange;
};