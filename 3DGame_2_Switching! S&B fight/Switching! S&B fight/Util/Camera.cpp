#include "DxLib.h"
#include "Camera.h"
#include "../Player/Player.h"
#include <algorithm>
#include <cmath>

namespace {

	// �J�������
	constexpr float kCameraHeight = 20.0f;	// �J�����̒����_
	constexpr float kCameraNear = 1.0f;		// �J������O�N���b�v����
	constexpr float kCameraFar = 10000.0f;	// �J�����ŉ��N���b�v����
	constexpr float kDist = 50.0f;			// �J��������v���C���[�܂ł̋���
	constexpr float kAngle = 0.05f;			// �J�����𓮂����p�x

	constexpr float kInitAngleH = -DX_PI_F / 2.0f;	// �J�����̏������s�p�x
	constexpr float kInitAngleV = -0.3f;	// �J�����̏��������p�x
	constexpr float kMinAngleV = DX_PI_F * 0.5f - 1.5f;		// �ŏ��̐����p�x
	constexpr float kMaxAngleV = -DX_PI_F * 0.5f + 0.6f;	// �ő�̐����p�x
	
	
	float cameraAddAngle=0.0f;
}

Camera::Camera():
	m_pos(VGet(0.0f, 0.0f, 0.0f)),
	m_targetPos(VGet(0.0f, 0.0f, 0.0f)),
	m_angleH(kInitAngleH),
	m_angleV(kInitAngleV)
{
	
}

void Camera::Init()
{
	SetCameraNearFar(kCameraNear, kCameraFar);
}

void Camera::Update(const Player& player)
{
	int Pad = GetJoypadInputState(DX_INPUT_PAD1);

	
	//���͏�ԏ�����
	input.Rx = 0;
	input.Ry = 0;


	// ���͏�Ԃ��擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	if (input.Rx < 0.0f)			// �E�X�e�B�b�N���E�ɓ|������E��]����
	{
		m_angleH -= kAngle;
	}
	if (input.Rx > 0.0f)			// �E�X�e�B�b�N�����ɓ|�����獶��]����
	{
		m_angleH += kAngle;
	}
	if(input.Ry>0.0f)				// �E�X�e�B�b�N�����ɓ|�����������ɉ��
	{
		m_angleV -= kAngle;
		// ������p�x�ȏ�ɂȂ�Ȃ��悤�ɂ���
		m_angleV = (std::max)(m_angleV, kMaxAngleV);

	}
	if (input.Ry < 0.0f)			// �E�X�e�B�b�N����ɓ|�����牺�����ɉ��
	{
		m_angleV += kAngle;
		// ������p�x�ȉ��ɂȂ�Ȃ��悤�ɂ���
		m_angleV = (std::min)(kMinAngleV, m_angleV);
	}

	// �J�����̒����_��ݒ�
	m_targetPos = VAdd(player.GetPos(), VGet(0.0f, kCameraHeight, 0.0f));
	// �J�����ʒu�␳
	FixCameraPos();

	// �J�����̏������C�u�����̃J�����ɔ��f������
	SetCameraPositionAndTarget_UpVecY(m_pos, m_targetPos);

	ChangeLightTypeDir(VGet(0.0f, -30.0f, 0.0f));

	DrawFormatString(0, 160, 0xffffff, "Camera:m_pos.x/y/z=%.2f/%.2f/%.2f", m_pos.x, m_pos.y, m_pos.z);

	DrawFormatString(0, 180, 0xffffff, "Player:m_pos.x/y/z=%.2f/%.2f/%.2f",
		player.GetPos().x, player.GetPos().y, player.GetPos().z);
}

void Camera::FixCameraPos()
{
	// ���������̉�]
	auto rotY = MGetRotY(m_angleH);
	// ���������̉�]
	auto rotZ = MGetRotZ(m_angleV);

	// X���ɃJ��������v���C���[�܂ł̋������L�т��x�N�g���𐂒������ɉ�]����(Z����])
	m_pos = VTransform(VGet(-kDist, 0.0f, 0.0f), rotZ);
	// ��������(Y����])�ɉ�]����
	m_pos = VTransform(m_pos, rotY);

	// �����_�̍��W�𑫂�
	m_pos = VAdd(m_pos, m_targetPos);
}
