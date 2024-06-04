#pragma once
#include "DxLib.h"
#include "Rect.h"

class Player
{
public:
	Player();
	~Player();

	void Init();
	void Update();
	void Draw();
	void End();

private:
	int m_playerModel;		// �v���C���[���f��
	
	float m_playerScele;	// �v���C���[�T�C�Y
	float m_speed;			// �ړ����x

	VECTOR m_pos;			// �v���C���[�ʒu
	VECTOR m_move;			// �ړ���

	// �����蔻���`
	Rect m_colRect;
};