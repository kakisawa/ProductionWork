#include "Player.h"
#include "DxLib.h"
#include "Pad.h"
#include <cassert>
#include <cmath>

namespace {
	const char* const kModelPlayer = "data/model/RogueHooded.mv1";	// ���f���̃t�@�C����

	constexpr float kInitAngle = -DX_PI_F / 2.0f * 90.0f;	// �v���C���[�̏����p�x*90(�����𔽑΂ɂ���)
	constexpr float kModelSize = 5.0f;						// ���f���̃T�C�Y
	constexpr float kSpeed = 0.5f;							// �v���C���[�ړ����x

	// �A�j���[�V�����֌W
	constexpr float kAnimChangeFrame = 8.0f;				// �A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;	// �A�j���[�V�����؂�ւ��ɂ����鑬�x
	constexpr float kAnimBlendAdd = 0.5f;					// �A�j���[�V�����u�����h�̑����l
	constexpr float kAnimBlendMax = 1.0f;					// �A�j���[�V�����u�����h�̍ő�l
	
	// �������p�l
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// �x�N�g���̏�����
	constexpr float kInitFloat = 0.0f;				// float�l�̏�����
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player() :
	m_model		(-1),
	m_angle		(kInitFloat),
	m_isAttack	(false),
	m_isWalk	(false),
	m_currentAnimNo(-1),
	m_currentAnimCount(kInitFloat),
	m_prevAnimNo(-1),
	m_prevAnimCount(kInitFloat),
	m_animBlendRate(kAnimBlendMax),
	m_currentState(State::kIdle),
	m_pos(kInitVec),
	m_move(kInitVec)
{
	// ���f���ǂݍ���
	m_model = MV1LoadModel(kModelPlayer);
	assert(m_model != -1);

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
}

/// <summary>
/// ������
/// </summary>
void Player::Init()
{
	// �v���C���[�����ݒ�
	MV1SetScale(m_model, VGet(kModelSize, kModelSize, kModelSize));	// �v���C���[�̏����T�C�Y
	MV1SetRotationXYZ(m_model, VGet(0.0f, kInitAngle, 0.0f));		// �v���C���[�̏����p�x
	MV1SetPosition(m_model, m_pos);									// �v���C���[�̏����ʒu	
	PlayAnim(AnimKind::kIdle);										// �v���C���[�̏����A�j���[�V����
}

/// <summary>
/// �X�V
/// </summary>
void Player::Update()
{
	Pad::Update();

	// �v���C���[�̏�ԍX�V
	State prevState = m_currentState;
	m_currentState = MoveValue();		// �ړ�
	//m_currentState = Attack();		// �U��

	//�A�j���[�V������Ԃ̍X�V
	UpdateAnimState(prevState);
	// �A�j���[�V���������̍X�V
	UpdateAnim();

	// �v���C���[�̍��W�X�V
	Move(m_move);
}

/// <summary>
/// �`��
/// </summary>
void Player::Draw()
{
	// ���f���̕`��
	MV1DrawModel(m_model);
}

/// <summary>
/// �I��
/// </summary>
void Player::End()
{
	// ���f���̍폜
	MV1DeleteModel(m_model);
}

/// <summary>
/// �A�j���[�V������Ԃ̍X�V
/// </summary>
/// <param name="state">���݂̏��</param>
void Player::UpdateAnimState(State state)
{
	// �ҋ@���ړ�
	if (state == State::kIdle && m_currentState == State::kWalk)
	{
		PlayAnim(AnimKind::kWalk);
	}
	//// �ҋ@���U��
	//if (state == State::kIdle && m_currentState == State::kAttack)
	//{
	//	PlayAnim(AnimKind::kAttack);
	//}
	// �ړ����ҋ@
	if (state == State::kWalk&& m_currentState == State::kIdle)
	{
		PlayAnim(AnimKind::kIdle);
	}
	//// �ړ����U��
	//if (state == State::kWalk&& m_currentState == State::kAttack)
	//{
	//	PlayAnim(AnimKind::kAttack);
	//}
	//// �U�����ҋ@
	//if (state == State::kAttack&& m_currentState == State::kIdle)
	//{
	//	PlayAnim(AnimKind::kIdle);
	//}
	//// �U�����ړ�
	//if (state == State::kAttack && m_currentState == State::kWalk)
	//{
	//	PlayAnim(AnimKind::kWalk);
	//}
}

/// <summary>
/// �A�j���[�V��������
/// </summary>
void Player::UpdateAnim()
{
	float total;		// �Đ����A�j���[�V�����̍ő�l

	// test 8�t���[���Ő؂�ւ�
	if (m_animBlendRate < kAnimBlendMax)
	{
		m_animBlendRate += kAnimChangeRateSpeed;
		if (m_animBlendRate >= kAnimBlendMax)
		{
			m_animBlendRate = kAnimBlendMax;
		}
	}

	// ���ݍĐ����̃A�j���[�V�����̏���
	if (m_currentAnimNo != -1)
	{
		// ���ݍĐ����̃A�j���[�V�����̑��J�E���g���擾����
		total = MV1GetAttachAnimTotalTime(m_model, m_currentAnimNo);
		// �A�j���[�V������i�s������
		m_currentAnimCount += kAnimBlendAdd;	// �A�j���[�V������i�߂�

		if(m_currentAnimCount > total)
		{
			// �U���A�j���[�V�������I��������ҋ@�Ɉڍs
			if (m_isAttack)
			{
				m_isAttack = false;
				m_currentState = State::kIdle;
				PlayAnim(AnimKind::kIdle);
			}

			m_currentAnimCount = static_cast<float>(fmod(m_currentAnimCount, total));
		}

		// �i�߂����Ԃɐݒ�
		MV1SetAttachAnimTime(m_model, m_currentAnimNo, m_currentAnimCount);
		// �A�j���[�V�����̃u�����h����ݒ肷��
		MV1SetAttachAnimBlendRate(m_model, m_currentAnimNo, m_animBlendRate);
	}

	// ��O�ɍĐ����Ă����A�j���[�V�����̏���
	if (m_prevAnimNo != -1)
	{
		// �A�j���[�V�����̑����Ԋl��
		total=MV1GetAttachAnimTotalTime(m_model, m_prevAnimNo);
		
		// �A�j���[�V������i�s������
		m_prevAnimCount += kAnimBlendAdd;

		// �A�j���[�V�����̍Đ����Ԃ����[�v
		if (m_prevAnimNo > total)
		{
			m_prevAnimCount = static_cast<float>(fmod(m_prevAnimCount, total));
		}

		// �i�߂����Ԃɐݒ�
		MV1SetAttachAnimTime(m_model, m_prevAnimNo, m_prevAnimCount);
		// �A�j���[�V�����̃u�����h����ݒ肷��
		MV1SetAttachAnimBlendRate(m_model, m_prevAnimNo, kAnimBlendMax - m_animBlendRate);
		// �ύX��̃A�j���[�V����0%
		//MV1SetAttachAnimBlendRate(m_model, m_prevAnimNo, m_animBlendRate);
	}
}

/// <summary>
/// �A�j���[�V�����̍Đ�
/// </summary>
/// <param name="animIndex">�Đ�����A�j���[�V�����̏��</param>
void Player::PlayAnim(AnimKind animIndex)
{
	// �X�ɌÂ��A�j���[�V�������A�^�b�`����Ă���ꍇ�͂��̎��_�ō폜���Ă���
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_model, m_prevAnimNo);
		m_prevAnimNo = -1;
	}

	// ���ݍĐ����̑ҋ@�A�j���[�V�����͕ύX�O�̃A�j���[�V����������
	m_prevAnimNo = m_currentAnimNo;
	m_prevAnimCount = m_currentAnimCount;

	// �ύX��̃A�j���[�V�����Ƃ��čU���A�j���[�V���������߂Đݒ肷��
	m_currentAnimNo = MV1AttachAnim(m_model, static_cast<int>(animIndex), -1, false);
	m_currentAnimCount = 0.0f;

	// �u�����h����Prev���L���łȂ��ꍇ�A1.0f�ɂ���
	if (m_prevAnimNo == 1)
	{
		m_animBlendRate = kAnimBlendMax;
	}
	else {
		// �؂�ւ��̏u�Ԃ͕ύX�O�̃A�j���[�V�������Đ�������Ԃɂ���
		m_animBlendRate = 0.0f;
	}
	
}

/// <summary>
/// �ړ��p�����[�^�̐ݒ�
/// </summary>
Player::State Player::MoveValue()
{
	State nextState = m_currentState;

	// �ړ��l�������l�ɖ߂�
	m_move = VGet(0.0f, 0.0f, 0.0f);

	// �ړ�������(true:�ړ�����)
	bool isPressMove = false;

	if (!m_isAttack)
	{

		if (Pad::IsPress(PAD_INPUT_RIGHT))						// �E����
		{
			m_move = VAdd(m_move, VGet(kSpeed, 0.0f, 0.0f));
			isPressMove = true;
		}
		if (Pad::IsPress(PAD_INPUT_LEFT))						// ������
		{
			m_move = VAdd(m_move, VGet(-kSpeed, 0.0f, 0.0f));
			isPressMove = true;
		}
		if (Pad::IsPress(PAD_INPUT_UP))							// �O����
		{
			m_move = VAdd(m_move, VGet(0.0f, 0.0f, kSpeed));
			isPressMove = true;
		}
		if (Pad::IsPress(PAD_INPUT_DOWN))						// ������
		{
			m_move = VAdd(m_move, VGet(0.0f, 0.0f, -kSpeed));
			isPressMove = true;
		}

		// ���K��
		if (VSize(m_move) > 0.0f)
		{
			m_move = VNorm(m_move);
			m_move = VScale(m_move, kSpeed);
		}

		if (isPressMove)
		{
			// �ҋ@��Ԃ̏ꍇ
			if (m_currentState == State::kIdle)
			{
				nextState = State::kWalk;
			}

			
		}
		else	// �ړ����Ȃ��ꍇ
		{
			if (m_currentState == State::kWalk)
			{
				// �ҋ@��Ԃɂ���
				nextState = State::kIdle;
			}
		}
	}

	return nextState;
}

/// <summary>
/// �v���C���[�̈ړ�����
/// </summary>
/// <param name="MoveVector">�ړ���</param>
void Player::Move(const VECTOR& MoveVector)
{
	if (fabs(MoveVector.x) > 0.0f || fabs(MoveVector.z) > 0.0f)
	{
		m_isWalk = true;
	}
	else
	{
		m_isWalk = false;
	}

	// �v���C���[�̈ʒu�Ɉړ��ʂ𑫂�
	m_pos = VAdd(m_pos, m_move);

	// �v���C���[�̈ʒu�Z�b�g
	MV1SetPosition(m_model, m_pos);
}

/// <summary>
/// �v���C���[�̍U������
/// </summary>
Player::State Player::Attack()
{
	return State();
}
