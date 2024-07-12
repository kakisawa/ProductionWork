#include "Player.h"
#include "DxLib.h"
#include "Pad.h"
#include <cassert>

namespace {
	const char* const kModelPlayer = "data/model/RogueHooded.mv1";	// ���f���̃t�@�C����

	constexpr float kInitAngle = -DX_PI_F / 2.0f * 90.0f;	// �v���C���[�̏����p�x*90(�����𔽑΂ɂ���)
	constexpr float kModelSize = 5.0f;						// ���f���̃T�C�Y
	constexpr float kSpeed = 0.5f;							// �ړ����x

	// �A�j���[�V�����ԍ�
	constexpr int kIdleAnimIndex = 1;		// �ҋ@
	constexpr int kWalkAnimIndex = 3;		// ����
	constexpr int kAttackAnimIndex = 30;	// �U��

	// �A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float kAnimChangeFrame = 8.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;
	
	const VECTOR kInit = VGet(0.0f, 0.0f, 0.0f);	// �x�N�g���̏�����
}

Player::Player() :
	m_model(-1),
	m_prevAnimNo(-1),
	m_currentAnimNo(-1),
	m_animBlemdRate(0.0f),
	m_pos(kInit),
	m_move(kInit),
	m_isAttack(false),
	m_isWalk(false)
{
	// ���f���ǂݍ���
	m_model = MV1LoadModel(kModelPlayer);
	assert(m_model != -1);

}

Player::~Player()
{
}

void Player::Init()
{
	// �ҋ@�A�j���[�V������ݒ�
	m_currentAnimNo = MV1AttachAnim(m_model, kIdleAnimIndex, -1, false);
	m_prevAnimNo = -1;
	m_animBlemdRate = 1.0f;

	// �v���C���[�����ݒ�
	MV1SetScale(m_model, VGet(kModelSize, kModelSize, kModelSize));	// �v���C���[�̏����T�C�Y
	MV1SetRotationXYZ(m_model, VGet(0.0f, kInitAngle, 0.0f));		// �v���C���[�̏����p�x
	MV1SetPosition(m_model, m_pos);									// �v���C���[�̏����ʒu
}

void Player::Update()
{
	Pad::Update();

	// �A�j���[�V�����̐؂�ւ�
	if (m_prevAnimNo != -1)
	{
		// test 8�t���[���Ő؂�ւ�
		m_animBlemdRate += kAnimChangeRateSpeed;
		if (m_animBlemdRate >= 1.0f) m_animBlemdRate = 1.0f;
		// �ύX��̃A�j���[�V����������ݒ肷��
		MV1SetAttachAnimBlendRate(m_model, m_prevAnimNo, 1.0f - m_animBlemdRate);
		MV1SetAttachAnimBlendRate(m_model, m_currentAnimNo, m_animBlemdRate);
	}

	// �A�j���[�V������i�߂�
	bool isLoop = UpdateAnim(m_currentAnimNo);
	UpdateAnim(m_prevAnimNo);

	// �U���A�j���[�V��������
	if(!m_isAttack)
	{
		if (Pad::IsPress(PAD_INPUT_X))
		{
			m_isAttack = true;
			ChangeAnim(kAttackAnimIndex);
		}
		else {
			// �ړ�����
			Move();
			/*if(m_isWalk)
			{
				ChangeAnim(kWalkAnimIndex);
			}
			if(isLoop){
				m_isWalk = false;
				ChangeAnim(kIdleAnimIndex);
			}*/
		}
	}
	else
	{
		// �U���A�j���[�V�������I��������ҋ@�A�j���[�V�������Đ�����
		if (isLoop)
		{
			m_isAttack = false;
			ChangeAnim(kIdleAnimIndex);
		}
	}
	
	// �v���C���[�̈ʒu�Z�b�g
	MV1SetPosition(m_model, m_pos);
}

void Player::Draw()
{
	// ���f���̕`��
	MV1DrawModel(m_model);

	DrawFormatString(0, 40, 0xffffff, "m_isWalk=%d", m_isWalk);

}

void Player::End()
{
	// ���f���̍폜
	MV1DeleteModel(m_model);
}

void Player::UpdateAnimState(State state)
{
}

void Player::UpdateAnim()
{
}

void Player::PlayAnim(AnimKind animIndex)
{
}

void Player::Move()
{
	// �ړ��l�������l�ɖ߂�
	m_move = VGet(0.0f, 0.0f, 0.0f);

	if (Pad::IsPress(PAD_INPUT_RIGHT))						// �E����
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

	if (m_move.x != kInit.x || m_move.z != kInit.z)
	{
		m_isWalk = true;
	}

	// �v���C���[�̈ʒu�Ɉړ��ʂ𑫂�
	m_pos = VAdd(m_pos, m_move);
}

bool Player::UpdateAnim(int attachNo)
{
	// �A�j���[�V�������ݒ肳��Ă��Ȃ��̂ŏI��
	if (attachNo == -1) return false;

	// �A�j���[�V������i�s������
	float now = MV1GetAttachAnimTime(m_model, attachNo);	// ���݂̍Đ��J�E���g���擾
	now += 0.5f;	// �A�j���[�V������i�߂�

	// ���ݍĐ����̃A�j���[�V�����̑��J�E���g���擾����
	float total = MV1GetAttachAnimTotalTime(m_model, attachNo);
	bool isLoop = false;
	while (now >= total)
	{
		now -= total;
		isLoop = true;
	}
	// �i�߂����Ԃɐݒ�
	MV1SetAttachAnimTime(m_model, attachNo, now);

	return isLoop;
}

void Player::ChangeAnim(int animIndex)
{
	// �X�ɌÂ��A�j���[�V�������A�^�b�`����Ă���ꍇ�͂��̎��_�ō폜���Ă���
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_model, m_prevAnimNo);
	}

	// ���ݍĐ����̑ҋ@�A�j���[�V�����͕ύX�O�̃A�j���[�V����������
	m_prevAnimNo = m_currentAnimNo;

	// �ύX��̃A�j���[�V�����Ƃ��čU���A�j���[�V���������߂Đݒ肷��
	m_currentAnimNo = MV1AttachAnim(m_model, animIndex, -1, false);

	// �؂�ւ��̏u�Ԃ͕ύX�O�̃A�j���[�V�������Đ�������Ԃɂ���
	m_animBlemdRate = 0.0f;

	// �ύX�O�̃A�j���[�V����100%
	MV1SetAttachAnimBlendRate(m_model, m_prevAnimNo, 1.0f - m_animBlemdRate);
	// �ύX��̃A�j���[�V����0%
	MV1SetAttachAnimBlendRate(m_model, m_currentAnimNo, m_animBlemdRate);
}
