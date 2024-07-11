#include "Player.h"
#include "DxLib.h"
#include "Pad.h"
#include <cassert>

namespace {
	const char* const kModelPlayer = "data/model/RogueHooded.mv1";
	const char* const kModel = "data/model/artifact.mv1";
	

	constexpr float kInitAngle = -DX_PI_F / 2.0f;	// �v���C���[�̏����p�x
	constexpr float kModelSize = 5.0f;

	constexpr float kSpeed = 0.5f;

	int model;
}

Player::Player() :
	m_model(-1),
	m_pos(VGet(0.0f, 0.0f, 0.0f)),
	m_move(VGet(0.0f, 0.0f, 0.0f))
{
	m_model = MV1LoadModel(kModelPlayer);
	assert(m_model != -1);

	// ��
	model = MV1LoadModel(kModel);
	MV1SetScale(model, VGet(kModelSize, kModelSize, kModelSize));


	MV1SetScale(m_model, VGet(kModelSize, kModelSize, kModelSize));
	MV1SetRotationXYZ(m_model, VGet(0.0f,90* kInitAngle,0.0f));
	MV1SetPosition(m_model, m_pos);
}

Player::~Player()
{
}

void Player::Init()
{
}

void Player::Update()
{
	Pad::Update();

	// �ړ��l�������l�ɖ߂�
	m_move = VGet(0.0f, 0.0f, 0.0f);

	// �ړ�����
	if(Pad::IsPress(PAD_INPUT_RIGHT))						// �E����
	{
		m_move = VAdd(m_move, VGet(kSpeed, 0.0f, 0.0f));
	}
	if (Pad::IsPress(PAD_INPUT_LEFT))						// ������
	{
		m_move = VAdd(m_move, VGet(-kSpeed, 0.0f, 0.0f));
	}
	if (Pad::IsPress(PAD_INPUT_UP))							// �O����
	{
		m_move = VAdd(m_move, VGet(0.0f, 0.0f, kSpeed));
	}
	if (Pad::IsPress(PAD_INPUT_DOWN))						// ������
	{
		m_move = VAdd(m_move, VGet(0.0f, 0.0f, -kSpeed));
	}

	// ���K��
	if (VSize(m_move) > 0.0f)
	{
		m_move = VNorm(m_move);
		m_move = VScale(m_move, kSpeed);
	}

	// �v���C���[�̈ʒu�Ɉړ��ʂ𑫂�
	m_pos = VAdd(m_pos, m_move);

	// �v���C���[�̈ʒu�Z�b�g
	MV1SetPosition(m_model, m_pos);

	// ��
	VECTOR pos = VGet(0.0f, 0.0f, 0.0f);
	MV1SetPosition(model, pos);
}

void Player::Draw()
{
	MV1DrawModel(m_model);

	// ��
	MV1DrawModel(model);
}

void Player::End()
{
	MV1DeleteModel(m_model);
}
