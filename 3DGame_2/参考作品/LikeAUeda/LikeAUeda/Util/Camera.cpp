#include "DxLib.h"
#include "Camera.h"
#include "Player.h"
#include "Input.h"
#include <cmath>

// �萔
namespace
{
	constexpr float kNear = 1.0f;							// �J�����̎�O�N���b�v����
	constexpr float kFar = 5000.0f;							// �J�����̉��N���b�v����
	constexpr float kDist = 100.0f;							// �J��������v���C���[�܂ł̋���
	constexpr float kHeight = 60.0f;						// �J�����̒����_
	constexpr float kAngle = 0.03f;							// �J�����𓮂����p�x
	constexpr float kInitAngleH = -DX_PI_F / 2.0f;			// �J�����̏������s�p�x
	constexpr float kInitAngleV = -0.3f;					// �J�����̏��������p�x
	constexpr float kMinAngleV = DX_PI_F * 0.5f - 1.0f;		// �ŏ��̐����p�x
	constexpr float kMaxAngleV = -DX_PI_F * 0.5f + 0.6f;	// �ő�̐����p�x
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Camera::Camera():
	m_pos(VGet(0.0f, kHeight, 0.0f)),
	m_target(VGet(0.0f, 0.0f, 0.0f)),
	m_angleH(kInitAngleH),
	m_angleV(kInitAngleV)
{
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
Camera::~Camera()
{
	// �����Ȃ�
}


/// <summary>
/// ������
/// </summary>
void Camera::Init()
{
	SetCameraNearFar(kNear, kFar);
}


/// <summary>
/// �X�V
/// </summary>
void Camera::Update(Input& input, const Player& player)
{
	// ������
	if (input.IsPressing("rotateL"))
	{
		m_angleH -= kAngle;
	}
	// �E����
	if (input.IsPressing("rotateR"))
	{
		m_angleH += kAngle;
	}
	// �����
	if (input.IsPressing("rotateU"))
	{
		m_angleV -= kAngle;
		// ������p�x�ȏ�ɂȂ�Ȃ��悤�ɂ���
		m_angleV = (std::max)(m_angleV, kMaxAngleV);
	}
	// ������
	if (input.IsPressing("rotateD"))
	{
		m_angleV += kAngle;
		// ������p�x�ȉ��ɂȂ�Ȃ��悤�ɂ���
		m_angleV = (std::min)(kMinAngleV, m_angleV);
	}

	// �J�����̒����_��ݒ肷��
	m_target = VAdd(player.GetPos(), VGet(0.0f, kHeight, 0.0f));

	// �J�����ʒu�␳
	FixCameraPos();
	SetCameraPositionAndTarget_UpVecY(m_pos, m_target);
}

/// <summary>
/// �J�����ʒu��␳����
/// </summary>
void Camera::FixCameraPos()
{
	// ���������̉�]
	auto rotY = MGetRotY(m_angleH);
	// ���������̉�]
	auto rotZ = MGetRotZ(m_angleV);

	// �J�����̍��W�����߂�
	// X���ɃJ��������v���C���[�܂ł̋������L�т��x�N�g���𐂒������ɉ�]����(Z����])
	m_pos = VTransform(VGet(-kDist, 0.0f, 0.0f), rotZ);
	// ��������(Y����])�ɉ�]����
	m_pos = VTransform(m_pos, rotY);
	// �����_�̍��W�𑫂�
	m_pos = VAdd(m_pos, m_target);
}
