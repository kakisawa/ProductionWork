#pragma once
#include "DxLib.h"

class Player
{
public:
	Player();
	~Player();

	void Init();
	void Update();
	void Draw();
	void End();

	VECTOR GetPos()const { return m_pos; }

private:
	int m_model;		// �v���C���[���f��

	float m_playerScele;	// �v���C���[�T�C�Y
	
	VECTOR m_pos;			// �v���C���[�ʒu
	VECTOR m_move;			// �ړ���
};