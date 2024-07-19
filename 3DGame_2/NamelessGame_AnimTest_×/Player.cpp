#include "Player.h"
#include "DxLib.h"
#include "PlayerState.h"
#include "Pad.h"
#include "Camera.h"
#include "Time.h"
#include <cassert>
#include <cmath>

// 07/16:�A���U��������

namespace {
	const char* const kModelPlayer = "data/model/RogueHooded.mv1";	// ���f���̃t�@�C����

	constexpr float kInitAngle = -DX_PI_F / 2.0f * 90.0f;	// �v���C���[�̏����p�x*90(�����𔽑΂ɂ���)
	constexpr float kModelSize = 5.0f;			// ���f���̃T�C�Y
	constexpr float kSpeed = 0.7f;				// �v���C���[�ړ����x
	constexpr float kAttackSpeed = 0.5f;		// �v���C���[�U�����̉����x
	constexpr float	kAngleSpeed = 0.2f;			// �p�x�ω����x
	constexpr float	kJumpPower = 1.8f;			// �W�����v��
	constexpr float	kGravity = 0.05f;			// �d��

	// �A�j���[�V�����֌W
	constexpr float kAnimChangeFrame = 8.0f;		// �A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float kAnimChangeRateSpeed = 0.2f;	// �A�j���[�V�����؂�ւ��ɂ����鑬�x
	constexpr float kAnimBlendAdd = 0.5f;			// �A�j���[�V�����u�����h�̑����l
	constexpr float kAnimBlendMax = 1.0f;			// �A�j���[�V�����u�����h�̍ő�l

	// �������p�l
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// �x�N�g���̏�����
	constexpr float kInitFloat = 0.0f;				// float�l�̏�����
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player() :
	m_model(-1),
	m_angle(kInitFloat),
	m_gravity(kGravity),
	m_isAttack(false),
	m_nextAttackFlag(false),
	m_isWalk(false),
	m_isJump(false),
	//m_isForward(false),
	m_animBlendRate(kAnimBlendMax),
	m_currentJumpPower(0.0f),
	m_multiAttack(0),
	m_currentState(State::kIdle),
	m_pos(kInitVec),
	m_move(kInitVec),
	m_targetDir(VGet(0.0f, 0.0f, 0.0f)),
	m_animSpeed(0),
	m_animChangeFrameTotal(0),
	m_pAttackStanTime(std::make_shared<Time>(6))
{
	// ���f���ǂݍ���
	m_model = MV1LoadModel(kModelPlayer);
	assert(m_model != -1);


	// �A�j���[�V������Ԃ̏�����
	InitAnim(m_current);
	InitAnim(m_prev);
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
	//PlayAnim(AnimKind::kIdle);									// �v���C���[�̏����A�j���[�V����
	SetAnim(m_animData.kIdle, false, true);
}

/// <summary>
/// �X�V
/// </summary>
void Player::Update(const Camera& camera)
{
	Pad::Update();

	// �p�b�h���͂ɂ���Ĉړ��p�����[�^��ݒ肷��
	VECTOR	upMoveVec;		// �����{�^���u���v����͂������Ƃ��̃v���C���[�̈ړ������x�N�g��
	VECTOR	leftMoveVec;	// �����{�^���u���v����͂������Ƃ��̃v���C���[�̈ړ������x�N�g��

	// �v���C���[�̏�ԍX�V
	State prevState = m_currentState;
	m_currentState = JumpState();
	m_currentState = AttackState();		// �U��
	
	// �{�����f���N���X���ɓ����Ă�
	{
		m_animSpeed++;
		if (m_animSpeed >= m_animChangeFrameTotal)
		{
			// �A�j���[�V�����֌W(������)
			UpdateAnimation(m_prev, kAnimBlendAdd);
			UpdateAnimation(m_current, kAnimBlendAdd);
			m_animSpeed = 0;
		}

		m_animChangeFrame++;
		if (m_animChangeFrame > m_animChangeFrameTotal)
		{
			m_animChangeFrame = m_animChangeFrameTotal;
		}
		// �A�j���[�V�����̃u�����h����ݒ�
		UpdateAnimBlendRate();
	}


	

	// �U������
	Attack();

	m_currentState = MoveValue(camera, upMoveVec, leftMoveVec);		// �ړ�

	//�A�j���[�V������Ԃ̍X�V
	//UpdateAnimState(prevState);

	// �v���C���[�̈ړ������Ƀ��f���̕������߂Â���
	Angle();

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
/// (�ꉞ)�����ŏ��������
//void Player::UpdateAnimState(State state)
{
	// �ҋ@���ړ�
	if (state == State::kIdle && m_currentState == State::kWalk)
	{
		PlayAnim(AnimKind::kWalk);
	}
	// �ҋ@���U��
	if (state == State::kIdle && m_currentState == State::kAttack)
	{
		PlayAnim(AnimKind::kAttack1);
	}
	// �ҋ@���W�����v
	if (state == State::kIdle && m_currentState == State::kJump)
	{
		PlayAnim(AnimKind::kJump);
	}

	// �ړ����ҋ@
	if (state == State::kWalk && m_currentState == State::kIdle)
	{
		PlayAnim(AnimKind::kIdle);
	}
	// �ړ����U��
	if (state == State::kWalk && m_currentState == State::kAttack)
	{
		PlayAnim(AnimKind::kAttack1);
	}
	// �ړ����W�����v
	if (state == State::kWalk && m_currentState == State::kJump)
	{
		PlayAnim(AnimKind::kJump);
	}

	// �U�����ҋ@
	if (state == State::kAttack && m_currentState == State::kIdle)
	{
		PlayAnim(AnimKind::kIdle);
	}
	// �U�����ړ�
	if (state == State::kAttack && m_currentState == State::kWalk)
	{
		PlayAnim(AnimKind::kWalk);
	}
	// �U�����W�����v
	if (state == State::kAttack && m_currentState == State::kJump)
	{
		PlayAnim(AnimKind::kJump);
	}

	// �W�����v���ړ�
	if (state == State::kJump && m_currentState == State::kWalk)
	{
		PlayAnim(AnimKind::kWalk);
	}
}

/// <summary>
/// �A�j���[�V��������
/// </summary>
/// (�ꉞ)�����ŏ��������
void Player::UpdateAnim()
{
	//float total;		// �Đ����A�j���[�V�����̍ő�l

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
	if (m_current.animNo != -1)
	{
		// ���ݍĐ����̃A�j���[�V�����̑��J�E���g���擾����
		m_current.totalTime = MV1GetAttachAnimTotalTime(m_model, m_current.animNo);
		// �A�j���[�V������i�s������
		m_current.elapsedTime += kAnimBlendAdd;	// �A�j���[�V������i�߂�

		if (m_current.elapsedTime > m_current.totalTime)
		{
			// �U���A�j���[�V�������I��������ҋ@�Ɉڍs
			if (m_isAttack)
			{
				m_isAttack = false;
				m_currentState = State::kIdle;
				PlayAnim(AnimKind::kIdle);
			}

			// �W�����v�A�j���[�V�������I��������ҋ@�Ɉڍs
			if (!m_isJump)
			{
				m_currentState = State::kIdle;
				PlayAnim(AnimKind::kIdle);
			}

			m_current.elapsedTime = static_cast<float>(fmod(m_current.elapsedTime, m_current.totalTime));
		}

		// �i�߂����Ԃɐݒ�
		MV1SetAttachAnimTime(m_model, m_current.animNo, m_current.elapsedTime);
		// �A�j���[�V�����̃u�����h����ݒ肷��
		MV1SetAttachAnimBlendRate(m_model, m_current.animNo, m_animBlendRate);
	}

	// ��O�ɍĐ����Ă����A�j���[�V�����̏���
	if (m_prev.animNo != -1)
	{
		// �A�j���[�V�����̑����Ԋl��
		m_prev.totalTime = MV1GetAttachAnimTotalTime(m_model, m_prev.animNo);

		// �A�j���[�V������i�s������
		m_prev.elapsedTime += kAnimBlendAdd;

		// �A�j���[�V�����̍Đ����Ԃ����[�v
		if (m_prev.animNo > m_prev.totalTime)
		{
			m_prev.elapsedTime = static_cast<float>(fmod(m_prev.elapsedTime, m_prev.totalTime));
		}

		// �i�߂����Ԃɐݒ�
		MV1SetAttachAnimTime(m_model, m_prev.animNo, m_prev.elapsedTime);
		// �A�j���[�V�����̃u�����h����ݒ肷��
		MV1SetAttachAnimBlendRate(m_model, m_prev.animNo, kAnimBlendMax - m_animBlendRate);
	}
}

/// <summary>
/// �A�j���[�V�����̍Đ�
/// </summary>
/// <param name="animIndex">�Đ�����A�j���[�V�����̏��</param>
/// (�ꉞ)�����ŏ��������
void Player::PlayAnim(AnimKind animIndex)
{
	// �X�ɌÂ��A�j���[�V�������A�^�b�`����Ă���ꍇ�͂��̎��_�ō폜���Ă���
	if (m_prev.animNo != -1)
	{
		MV1DetachAnim(m_model, m_prev.animNo);
		m_prev.animNo = -1;
	}

	// ���ݍĐ����̑ҋ@�A�j���[�V�����͕ύX�O�̃A�j���[�V����������
	m_prev.animNo = m_current.animNo;
	m_prev.elapsedTime = m_current.elapsedTime;

	// �ύX��̃A�j���[�V�����Ƃ��čU���A�j���[�V���������߂Đݒ肷��
	m_current.animNo = MV1AttachAnim(m_model, static_cast<int>(animIndex), -1, false);
	m_current.elapsedTime = 0.0f;

	// �u�����h����Prev���L���łȂ��ꍇ�A1.0f�ɂ���
	if (m_prev.animNo == -1)
	{
		m_animBlendRate = kAnimBlendMax;
	}
	else {
		// �؂�ւ��̏u�Ԃ͕ύX�O�̃A�j���[�V�������Đ�������Ԃɂ���
		m_animBlendRate = 0.0f;
	}

}

void Player::EndJumpState()
{
	m_isJump = false;

	m_pos.y = 0.0f;
	m_gravity = kGravity;

	//m_upPower = 0;
}

void Player::GravityUpdate()
{
	m_gravity += 0.005f;
}

void Player::InitAnim(AnimData& anim)
{
	anim.animNo = -1;
	anim.attachNo = -1;
	anim.totalTime = 0.0f;
	anim.elapsedTime = 0.0f;
	anim.isLoop = false;
}

void Player::SetAnim(int animNo, bool loop, bool isForceChange)
{
	if (!isForceChange)
	{
		// �Đ����̃A�j���[�V�����͍Đ����Ȃ�
		if (m_current.animNo == animNo) return;
	}

	// �ȑO�̃A�j���[�V�������c���Ă�����I��
	if (m_prev.animNo != -1)
	{
		MV1DetachAnim(m_model, m_prev.animNo);
		InitAnim(m_prev);
	}
	if (m_current.animNo != -1)
	{
		MV1DetachAnim(m_model, m_prev.animNo);
		InitAnim(m_current);
	}

	
	// �V�����A�j���[�V�����̐ݒ�
	m_current.animNo = animNo;
	m_current.attachNo = MV1AttachAnim(m_model, m_current.animNo, -1, false);
	m_current.totalTime = MV1GetAttachAnimTotalTime(m_model, m_current.animNo);
	m_current.isLoop = loop;

	// �ύX�Ɋ|����t���[�������o���Ă���
	m_animChangeFrameTotal = 1;
	m_animChangeFrame = 1;
}

void Player::ChangeAnimation(int animNo, bool Loop, bool isForceChange, int changeFrame)
{
	// �ύX�Ɋ|����t���[�����̋L��
	if (!isForceChange)
	{
		// ���ɍĐ����̃A�j���[�V�����͍Đ����Ȃ�
		if (m_current.animNo == animNo)	return;
	}
	
	// �ȑO�̃A�j���[�V�������c���Ă�����I��
	if (m_prev.animNo != -1)
	{
		MV1DetachAnim(m_model, m_prev.animNo);
		InitAnim(m_prev);
	}

	// ���ݍĐ����̃A�j���[�V�����͕ύX�O�̃A�j���[�V����������
	m_prev = m_current;

	// �V�����A�j���[�V������ݒ肷��
	// �V�����A�j���[�V�����̐ݒ�
	m_current.animNo = animNo;
	m_current.attachNo = MV1AttachAnim(m_model, m_current.animNo, -1, false);
	m_current.totalTime = MV1GetAttachAnimTotalTime(m_model, m_current.animNo);
	m_current.isLoop = Loop;


	// �ύX�Ɋ|����t���[�������o���Ă���
	m_animChangeFrameTotal = changeFrame;
	m_animChangeFrame = 0;

	// �A�j���[�V�����̃u�����h����ݒ肷��
	UpdateAnimBlendRate();
}

bool Player::IsAnimEnd()
{
	// Loop�A�j���̏ꍇ��false��Ԃ�
	if (m_current.isLoop)return false;

	float time = MV1GetAttachAnimTime(m_model, m_current.attachNo);
	if (time >= m_current.totalTime)
	{
		return true;
	}

	return false;
}

void Player::UpdateAnimation(AnimData anim, float dt)
{
	if (anim.attachNo == -1)	return;

	// �A�j���[�V�����̍X�V
	anim.elapsedTime= MV1GetAttachAnimTime(m_model, anim.attachNo);
	anim.elapsedTime += dt;

	if (anim.elapsedTime >= anim.totalTime)
	{
		if (anim.isLoop)
		{
			// �A�j���[�V�����̃��[�v
			anim.elapsedTime -= anim.totalTime;
		}
		else
		{
			anim.elapsedTime = anim.totalTime;
		}

		// �i�߂����Ԃɐݒ�
		MV1SetAttachAnimTime(m_model, anim.attachNo, anim.elapsedTime);
	}
}

void Player::UpdateAnimBlendRate()
{
	// �u�����h����Prev���L���łȂ��ꍇ�A1.0f�ɂ���
	if (m_prev.animNo == -1)
	{
		m_animBlendRate = kAnimBlendMax;
	}
	else {
		// �؂�ւ��̏u�Ԃ͕ύX�O�̃A�j���[�V�������Đ�������Ԃɂ���
		m_animBlendRate = 0.0f;
	}
}


void Player::JumpStateInit()
{
	m_isJump = true;
}

void Player::AttackStateInit()
{
	m_isAttack = true;
	m_multiAttack = 0;
	m_isNextAttack = false;
}

void Player::IdleStateUpdate()
{
	ChangeAnimation(m_animData.kIdle, true, false, kStateAnimSpeed::Idle);
}

void Player::WalkStateUpdate()
{
	ChangeAnimation(m_animData.kWalk, true, false, kStateAnimSpeed::Walk);
}

void Player::JumpStateUpdate()
{
	ChangeAnimation(m_animData.kJump, true, false, kStateAnimSpeed::Jump);
}

void Player::AttackStateUpdate()
{
	switch (m_multiAttack)
	{
	case0:
		ChangeAnimation(m_animData.kAttack1, true, false, kStateAnimSpeed::Attack);
		break;
	case1:
		ChangeAnimation(m_animData.kAttack2, true, false, kStateAnimSpeed::Attack);
		break;
	case2:
		ChangeAnimation(m_animData.kAttack3, true, false, kStateAnimSpeed::Attack);
		break;
	case3:
		ChangeAnimation(m_animData.kAttack4, true, false, kStateAnimSpeed::Attack);
		break;
	default:
		break;
	}

	// �U���{�^���������ꂽ��
	if (Pad::IsPress(PAD_INPUT_X) && !m_nextAttackFlag)
	{
		m_nextAttackFlag = true;
	}
	
	// �A�j���[�V�������I������i�K�Ŏ��̍U���t���O���o���Ă��Ȃ�������
	if (IsAnimEnd() && !m_nextAttackFlag)
	{
		m_isAttack = false;
		m_pAttackStanTime->Reset();
		m_multiAttack = 0;
		m_pState->EndState();
	}


	// �A�j���[�V�������I������i�K�Ŏ��̍U���t���O�������Ă�����
	if (IsAnimEnd() && !m_nextAttackFlag)
	{
		// �U���d�����Ԃ�0��菬�����Ȃ�����
		if (m_pAttackStanTime->Update())
		{
			
			m_pAttackStanTime->Reset();
			m_nextAttackFlag = false;
			m_multiAttack++;
		}
	}
}

/// <summary>
/// �ړ��p�����[�^�̐ݒ�
/// </summary>
VECTOR Player::MoveValue(const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec)
{
	// �v���C���[�̈ړ������̃x�N�g�����Z�o
	// �����{�^���u���v���������Ƃ��̃v���C���[�̈ړ��x�N�g���̓J�����̎�����������x�����𔲂�������
	upMoveVec = VSub(camera.GetTarget(), camera.GetPosition());
	upMoveVec.y = 0.0f;

	// �����{�^���u���v���������Ƃ��̃v���C���[�̈ړ��x�N�g���͏���������Ƃ��̕����x�N�g���Ƃx���̃v���X�����̃x�N�g���ɐ����ȕ���
	leftMoveVec = VCross(upMoveVec, VGet(0.0f, kSpeed, 0.0f));

	// �ړ��l�������l�ɖ߂�
	m_move = VGet(0.0f, 0.0f, 0.0f);

	// �ړ�������(true:�ړ�����)
	bool isPressMove = false;

	// �ړ�����
	if (!m_isAttack)
	{
		if (Pad::IsPress(PAD_INPUT_RIGHT))						// �E����
		{
			m_move = VAdd(m_move, VScale(leftMoveVec, -1.0f));
			isPressMove = true;
		}
		if (Pad::IsPress(PAD_INPUT_LEFT))						// ������
		{
			m_move = VAdd(m_move, leftMoveVec);
			isPressMove = true;
		}
		if (Pad::IsPress(PAD_INPUT_UP))							// �O����
		{
			m_move = VAdd(m_move, upMoveVec);
			isPressMove = true;
		}
		if (Pad::IsPress(PAD_INPUT_DOWN))						// ������
		{
			m_move = VAdd(m_move, VScale(upMoveVec, -1.0f));
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

			m_targetDir = VNorm(m_move);
			m_move = VScale(m_targetDir, kSpeed);

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

	//�W�����v����
	Jump();
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

	//// �U�����̑O�i���x�𑫂�
	//m_move = VAdd(m_move, m_moveAttack);

	// �v���C���[�̈ʒu�Ɉړ��ʂ𑫂�
	m_pos = VAdd(m_pos, m_move);

	// �v���C���[�̈ʒu�Z�b�g
	MV1SetPosition(m_model, m_pos);
}

/// <summary>
/// �v���C���[�̊p�x����
/// </summary>
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
	MV1SetRotationXYZ(m_model, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}

///// <summary>
///// �v���C���[�̍U������
///// </summary>
//Player::State Player::AttackState()
//{
//	State nextState = m_currentState;
//
//
//	// ���݁A�A���U���̏���������Ă�
//
//
//	/**/
//
//	
//
//
//	return nextState;
//}
//
//Player::State Player::JumpState()
//{
//	State nextState = m_currentState;
//
//	// �v���C���[�̏�Ԃ��u�W�����v�v�ł͂Ȃ��A���{�^���P��������Ă�����W�����v����
//	if (nextState != State::kJump && (Pad::IsPress(PAD_INPUT_A)))
//	{
//		m_isJump = true;
//
//		// �x�������̑��x���Z�b�g
//		m_currentJumpPower = kJumpPower;
//
//		nextState = State::kJump;
//	}
//
//	return nextState;
//}

void Player::Attack()
{
	if (!m_isAttack)	return;

	/*if (m_isForward)
	{
		m_moveAttack = VAdd(m_moveAttack, VGet(0.0f, 0.0f, kAttackSpeed));
		m_isForward = false;
	}*/

	

}

void Player::Jump()
{
	if (!m_isJump)	return;

	if (m_pos.y >= 0.0f)
	{
		// �W�����v��ԂȂ�d�͓K�p
		if (m_currentState == State::kJump)
		{
			// �x�������̑��x���d�͕����Z����
			m_currentJumpPower -= m_gravity;
			GravityUpdate();
		}

		// �ړ��x�N�g���̂x�������x�������̑��x�ɂ���
		m_move.y = m_currentJumpPower;
	}
	else {
		EndJumpState();
	}
}