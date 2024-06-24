#include "Player.h"
#include "DxLib.h"
#include "Stage.h"
#include <vector>
#include <memory>

namespace {
	const char* const kModelPlayer = "data/model/Player.mv1";
	constexpr float kColSize = 3.5f;	// �����蔻��̕�(�����`)	�x�N�g���Ȃ�4
}

Player::Player() :
	m_speed(0.5f),
	m_playerScele(3.0f),
	m_pos(VGet(0.0f, 1.0f, 0.0f)),
	m_move(VGet(0.0f, 0.0f, 0.0f)),
	m_playerModel(MV1LoadModel(kModelPlayer))// �v���C���[�̃��[�h
{
	MV1SetScale(m_playerModel, VGet(m_playerScele, m_playerScele, m_playerScele));
	MV1SetPosition(m_playerModel, m_pos);
}

Player::~Player()
{
}

void Player::Init(std::shared_ptr<Stage> pStage)
{
	sp.leftUp = pStage->GetStageUpperLeft();
	sp.rightDown = pStage->GetStageLowerRight();
}

void Player::Update()
{
	// �ړ��ʂ������l�ɖ߂�
	m_move = VGet(0, 0, 0);

	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// �ړ�����
	if (pad & PAD_INPUT_RIGHT)
	{
		m_move = VAdd(m_move, VGet(m_speed, 0.0f, 0.0f));
	}
	if (pad & PAD_INPUT_LEFT)
	{
		m_move = VAdd(m_move, VGet(-m_speed, 0.0f, 0.0f));
	}
	if (pad & PAD_INPUT_UP)
	{
		m_move = VAdd(m_move, VGet(0.0f, 0.0f, m_speed));
	}
	if (pad & PAD_INPUT_DOWN)
	{
		m_move = VAdd(m_move, VGet(0.0f, 0.0f, -m_speed));
	}

	// ���K��
	if (VSize(m_move) > 0.0f)
	{
		m_move = VNorm(m_move);
		m_move = VScale(m_move, m_speed);
	}

	// �v���C���[�̈ʒu�Ɉړ��ʂ𑫂�
	m_pos = VAdd(m_pos, m_move);


	// �v���C���[����ʊO�ɏo�Ȃ��悤���鏈��
	if (m_pos.x - 2 < sp.leftUp.x)
	{
		m_pos.x -=m_move.x ;		// ��
	}
	if (m_pos.x + 2 > sp.rightDown.x)
	{
		m_pos.x -= m_move.x;		// �E
	}
	if (m_pos.z + 2 > sp.rightDown.z)
	{
		m_pos.z -= m_move.z;		// ��
	}
	if (m_pos.z - 2 < sp.leftUp.z)
	{
		m_pos.z -= m_move.z;		// ��
	}
	

	// �����蔻��̍X�V
	m_colRect.SetLB(m_pos, kColSize, kColSize);
	// �v���C���[�̈ʒu�Z�b�g
	MV1SetPosition(m_playerModel, m_pos);	
}

void Player::Draw()
{
	// �����蔻��`��
	m_colRect.PlayerDraw(0x000000, true);
	// �v���C���[�`��
	MV1DrawModel(m_playerModel);

	DrawFormatString(0, 430, 0xffffff, "m_pos.x=%.0f:.z=%.0f", m_pos.x, m_pos.z);
}

void Player::End()
{
	// �v���C���[�̃A�����[�h
	MV1DeleteModel(m_playerModel);	
	m_playerModel = -1;
}
