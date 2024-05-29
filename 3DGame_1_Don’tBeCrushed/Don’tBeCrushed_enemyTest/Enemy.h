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

	void GetAddModelScale(float scale) { m_addScale = scale; }
	void GetAddMove(bool ismove) { m_isAddMove = ismove; }
	void GetModelAngle(VECTOR angle) { m_angle = angle; }
	void GetMove(VECTOR move) { m_move = move; }
	
private:

	int m_model;		// �G���f��
	int m_modelBase;	// �G���f��(�x�[�X)
	float m_scale;		// �G�T�C�Y
	float m_addScale;	// �G�T�C�Y(�ǉ�������)
	bool m_isAddMove;	// �G���ړ����邩
	
	// �G
	VECTOR m_pos;		// �G���W
	VECTOR m_angle;		// �G����
	VECTOR m_move;		// �G�ړ���
};

