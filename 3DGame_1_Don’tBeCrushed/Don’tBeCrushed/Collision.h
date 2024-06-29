#pragma once
#include "DxLib.h"

class Collision
{
public:
	Collision();
	virtual ~Collision();

	// �`��
	void PlayerDraw(unsigned int color, bool isFill);
	void EnemyDraw(unsigned int color, bool isFill);

	// (���Ղ��Č����ꍇ��)�������W����E����W���Z�b�g
	void SetLB(VECTOR pos, float width, float height);

	// �c�����̓G��(���Ղ��Č����ꍇ��)�������W����E����W���Z�b�g
	void SetPortrait(VECTOR pos, float width, float height,float depth, bool flag);
	// �������̓G��(���Ղ��Č����ꍇ��)�������W����E����W���Z�b�g
	void SetLandscape(VECTOR pos, float width, float height,float depth, bool flag);

	// ��`���m�̓����蔻��
	bool IsCollision(int num, const Collision& col);

private:

	struct Player {
		VECTOR m_leftUp;		// �����X���W
		VECTOR m_leftBottom;	// �����Z���W
		VECTOR m_rightUp;		// �E���X���W
		VECTOR m_rightBottom;	// �E���Z���W
	}p1;

	struct Enmey {
		VECTOR m_leftUp;		// �����X���W
		VECTOR m_leftBottom;	// �����Z���W
		VECTOR m_rightUp;		// �E���X���W
		VECTOR m_rightBottom;	// �E���Z���W
	}e1;

	
};

