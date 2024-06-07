#pragma once
#include "DxLib.h"

class Enemy
{
public:
	Enemy(VECTOR pos);
	~Enemy();

	void Init();
	void Update();
	void Draw();
	void End();

	void SetAddModelScale(float scale) { m_addScale = scale; }
	void SetAddMove(bool ismove) { m_isAddMove = ismove; }
	void SetAttack(bool isattack) { m_isAttack = isattack; }
	void SetModelAngle(VECTOR angle) { m_angle = angle; }
	void SetMove(VECTOR move) { m_move = move; }
	VECTOR GetPos() { return m_pos; }
	
	
private:

	int m_model;		// �G���f��
	int m_modelBase;	// �G���f��(�x�[�X)
	float m_scale;		// �G�T�C�Y
	float m_addScale;	// �G�T�C�Y(�ǉ�������)

	float m_gravity;		// �G�ɂ�����d��

	bool m_isAddMove;	// �G���ړ����邩�t���O
	bool m_isAttack;	// �G�̍U���t���O
	bool m_isMoveStop;
	
	// �G
	VECTOR m_pos;		// �G���W
	VECTOR m_angle;		// �G����
	VECTOR m_move;		// �G�ړ���
};

