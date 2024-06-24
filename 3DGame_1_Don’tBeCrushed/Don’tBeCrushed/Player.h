#pragma once
#include "DxLib.h"
#include "Collision.h"
#include <memory>

class Stage;
class Player
{
public:
	Player();
	~Player();

	void Init(std::shared_ptr<Stage> pStage);
	void Update();
	void Draw();
	void End();

	Collision GetColRect() const { return m_colRect; }	// �v���C���[�����蔻��擾

private:
	int m_playerModel;		// �v���C���[���f��

	struct StagePlace
	{
		VECTOR leftUp;		// �X�e�[�W������W
		VECTOR rightDown;	// �X�e�[�W�E�����W
	}sp;
	
	float m_playerScele;	// �v���C���[�T�C�Y
	float m_speed;			// �ړ����x

	VECTOR m_pos;			// �v���C���[�ʒu
	VECTOR m_move;			// �ړ���

	Collision m_colRect;			// �����蔻���`
};