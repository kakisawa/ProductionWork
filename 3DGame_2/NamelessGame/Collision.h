#pragma once
#include "DxLib.h"

class Collision
{
public:
	Collision();
	virtual ~Collision();

	// �`��
	void DrawMain(const float radius,unsigned int color, bool isFill);
	void DrawAttack(const float radius, unsigned int color, bool isFill);

	void UpdateCol(VECTOR pos,VECTOR upPos, VECTOR attackRange);	// �����蔻��̍X�V

	bool IsCollision(const Collision& col);		// �����蔻��

private:
	VECTOR m_pos;
	VECTOR m_vertexPos;
	VECTOR m_attackRange;
};