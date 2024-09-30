#include "Player.h"
#include "Camera.h"
#include "Pad.h"
#include <cmath>

namespace {
	const VECTOR InitVec = VGet(0.0f, 0.0f, 0.0f);
	constexpr float kMove = 0.7f;
	constexpr float	kAngleSpeed =0.15f;			// �p�x�ω����x
}

Player::Player() :
	model(-1),
	m_angle(0.0f),
	m_pos(InitVec),
	m_move(InitVec),
	m_targetDir(InitVec)
{
	model = MV1LoadModel("Data/Model/PlayerModel.mv1");
}

Player::~Player()
{
	MV1DeleteModel(model);
}

void Player::Init()
{
	m_pos = VGet(0.0f,0.0f,0.0f);
	MV1SetPosition(model, m_pos);
	MV1SetScale(model, VGet(0.5f, 0.5f, 0.5f));
}

void Player::Update(const Camera& camera)
{	
	Move(camera);
	Angle();

	m_pos = VAdd(m_pos, m_move);
	MV1SetPosition(model, m_pos);
}

void Player::Draw()
{
	MV1DrawModel(model);

	DrawFormatString(0, 300, 0xffffff, "m_move.x/y/z=%.2f/%.2f/%.2f", m_move.x, m_move.y, m_move.z);
}

void Player::Move(const Camera& camera)
{
	VECTOR upMoveVec = VSub(camera.GetTarget(), camera.GetPosition());
	upMoveVec.y = 0.0f;

	VECTOR leftMoveVec= VCross(upMoveVec, VGet(0.0f, kMove, 0.0f));

	// �ړ��ʂ̏�����
	m_move = InitVec;


	if (Pad::IsPress(PAD_INPUT_RIGHT))						// �E����
	{
		m_move = VAdd(m_move, VScale(leftMoveVec, -1.0f));
	}
	if (Pad::IsPress(PAD_INPUT_LEFT))						// ������
	{
		m_move = VAdd(m_move, leftMoveVec);
	}
	if (Pad::IsPress(PAD_INPUT_UP))							// �O����
	{
		m_move = VAdd(m_move, upMoveVec);
	}
	if (Pad::IsPress(PAD_INPUT_DOWN))						// ������
	{
		m_move = VAdd(m_move, VScale(upMoveVec, -1.0f));
	}

	// ���K��
	if (VSize(m_move) > 0.0f){
		m_move = VNorm(m_move);
		m_targetDir = m_move;
		m_move = VScale(m_move, kMove);
	}
}

void Player::Angle()
{
	// �v���C���[�̈ړ������Ƀ��f���̕������߂Â���
	float targetAngle;		// �ڕW�p�x
	float difference;		// �ڕW�p�x�ƌ��݂̊p�x�̍�

	// �ڕW�̕����x�N�g������p�x�l���Z�o����
	targetAngle = static_cast<float>(atan2(m_targetDir.x, m_targetDir.z));

	// �ڕW�̊p�x�ƌ��݂̊p�x�Ƃ̍�������o��
	difference = targetAngle - m_angle;

	// ���̊p�x��180�x�ȏ�ɂȂ��Ă�����C������
	if (difference < -DX_PI_F)
	{
		difference += DX_TWO_PI_F;
	}
	else if (difference > DX_PI_F)
	{
		difference -= DX_TWO_PI_F;
	}

	// �p�x�̍���0�ɋ߂Â���
	if (difference > 0.0f)
	{
		// �����v���X�̏ꍇ�͈���
		difference -= kAngleSpeed;
		if (difference < 0.0f)
		{
			difference = 0.0f;
		}
	}
	else
	{
		// �����}�C�i�X�̏ꍇ�͑���
		difference += kAngleSpeed;
		if (difference > 0.0f)
		{
			difference = 0.0f;
		}
	}

	// ���f���̊p�x���X�V
	m_angle = targetAngle - difference;
	MV1SetRotationXYZ(model, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}
