#include "Player.h"
#include "DxLib.h"
#include "Model.h"
#include "Pad.h"
#include "Camera.h"
#include "PlayerState.h"
#include "GameMap.h"
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


	// �������p�l
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// �x�N�g���̏�����
	constexpr float kInitFloat = 0.0f;				// float�l�̏�����
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player() :
	m_angle(kInitFloat),
	m_gravity(kGravity),
	m_isAttack(false),
	m_nextAttackFlag(false),
	m_isFirstAttack(false),
	m_isWalk(false),
	m_isJump(false),
	m_jumpPower(0.0f),
	m_multiAttack(0),
	m_pos(kInitVec),
	m_move(kInitVec),
	m_targetDir(VGet(0.0f, 0.0f, 0.0f))
{
	//���f���C���X�^���X�쐬
	m_pModel = std::make_shared<Model>(kModelPlayer);
	// �A�C�h����Ԃ̃A�j���[�V�������Đ�������
	m_pModel->SetAnim(m_animData.kIdle, false, true);

	//�X�e�C�g�N���X�̃C���X�^���X����
	m_pState = std::make_shared<PlayerState>();

	m_pState->AddState([=] { IdleStateUpdate(); }, [=] { IdleStateInit(); }, PlayerState::State::kIdle);
	m_pState->AddState([=] { WalkStateUpdate(); }, [=] { WalkStateInit(); }, PlayerState::State::kWalk);
	m_pState->AddState([=] { JumpStateUpdate(); }, [=] { JumpStateInit(); }, PlayerState::State::kJump);
	m_pState->AddState([=] { AttackStateUpdate(); }, [=] { AttackStateInit(); }, PlayerState::State::kAttack);

	//�����X�e�C�g�Z�b�g
	m_pState->SetState(PlayerState::State::kIdle);	//�X�e�C�g�Z�b�g(�ŏ���Idle���)
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
void Player::Init(std::shared_ptr<GameMap> pMap)
{
	// �v���C���[�����ݒ�
	m_pModel->SetSize(VGet(kModelSize, kModelSize, kModelSize));
	m_pModel->SetRota(VGet(0.0f, kInitAngle, 0.0f));
	m_pModel->SetPos(m_pos);

	mp.leftBack = pMap->GetMapLeftBack();
	mp.rightFront = pMap->GetMapRightFront();
}


/// <summary>
/// �X�V
/// </summary>
void Player::Update(const Camera& camera)
{
	// �p�b�h���͂ɂ���Ĉړ��p�����[�^��ݒ肷��
	VECTOR	upMoveVec;		// �����{�^���u���v����͂������Ƃ��̃v���C���[�̈ړ������x�N�g��
	VECTOR	leftMoveVec;	// �����{�^���u���v����͂������Ƃ��̃v���C���[�̈ړ������x�N�g��

	// �X�e�C�g�̍X�V
	m_pState->Update();

	// �v���C���[�̏�ԍX�V
	// �U������
	Attack();
	// �ړ�����
	OldMoveValue(camera, upMoveVec, leftMoveVec);

	// ���f���̃A�b�v�f�[�g
	m_pModel->Update();

	// �v���C���[�̈ړ������Ƀ��f���̕������߂Â���
	Angle();

	// �v���C���[�̍��W�X�V
	Move(m_move);
}

/// <summary>
/// �`��
/// </summary>
void Player::Draw()
{
	m_pModel->Draw();

	DrawFormatString(0, 200, 0xffffff, "State=%d", m_pState->GetState());
	DrawFormatString(0, 220, 0xffffff, "m_isWalk=%d", m_isWalk);
	DrawFormatString(0, 240, 0xffffff, "m_pos.x,y,z=%.2f,=%.2f,=%.2f", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(0, 260, 0xffffff, "m_move.x,y,z=%.2f,=%.2f,=%.2f", m_move.x, m_move.y, m_move.z);

	DrawFormatString(0, 280, 0xffffff, "m_angle=%.2f", m_angle);
}

/// <summary>
/// �I��
/// </summary>
void Player::End()
{
}


void Player::JumpStateInit()
{
	m_isJump = true;
	m_jumpPower = kJumpPower;
}

void Player::AttackStateInit()
{
	m_isAttack = true;
	m_multiAttack = 0;
	m_nextAttackFlag = false;
	m_isFirstAttack = true;
}

void Player::IdleStateUpdate()
{
	// �A�j���[�V������ҋ@���[�V�����ɕύX
	m_pModel->ChangeAnim(m_animData.kIdle, true, false, 0.5f);
}

void Player::WalkStateUpdate()
{
	// �A�j���[�V������������[�V�����ɕύX
	m_pModel->ChangeAnim(m_animData.kWalk, true, false, 0.5f);
}

void Player::JumpStateUpdate()
{
	// �A�j���[�V�������W�����v���[�V�����ɕύX
	m_pModel->ChangeAnim(m_animData.kJump, false, false, 1.0f);
}

void Player::AttackStateUpdate()
{
	// �A�j���[�V�����ύX
	switch (m_multiAttack)
	{
	case 0:
		m_pModel->ChangeAnim(m_animData.kAttack1, false, false, 1.0f);
		break;
	case 1:
		m_pModel->ChangeAnim(m_animData.kAttack2, false, false, 1.0f);
		break;
	case 2:
		m_pModel->ChangeAnim(m_animData.kAttack3, false, false, 1.0f);
		break;
	case 3:
		m_pModel->ChangeAnim(m_animData.kAttack4, false, false, 1.0f);
		break;
	default:
		break;
	}

	
	if (Pad::IsTrigger(PAD_INPUT_X) && !m_nextAttackFlag)
	{	
		if (!m_isFirstAttack)
		{
			m_nextAttackFlag = true;
		}
		m_isFirstAttack = false;
	}

	// �A�j���[�V�������I������i�K�Ŏ��̍U���t���O�������Ă��Ȃ�������
	if (m_pModel->IsAnimEnd() && !m_nextAttackFlag)
	{
		m_isAttack = false;
		m_multiAttack = 0;
		m_pState->EndState();
	}

	// �A�j���[�V�������I������i�K�Ŏ��̍U���t���O�������Ă�����
	if (m_pModel->IsAnimEnd() && m_nextAttackFlag)
	{
		// �d�����Ԃ�����Ȃ炱��
		{
			m_nextAttackFlag = false;
			m_multiAttack++;
		}
	}
}

/// <summary>
/// �ړ��p�����[�^�̐ݒ�
/// </summary>
void Player::OldMoveValue(const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec)
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
			m_targetDir = m_move;
			m_move = VScale(m_move, kSpeed);
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

	// �v���C���[�̈ʒu�Ɉړ��ʂ𑫂�
	m_pos = VAdd(m_pos, m_move);


	// �v���C���[����ʊO�ɏo�Ȃ��悤���鏈��
	if (m_pos.x < mp.leftBack.x)
	{
		m_pos.x -= m_move.x;		// ��
	}
	if (m_pos.x > mp.rightFront.x)
	{
		m_pos.x -= m_move.x;		// �E
	}
	if (m_pos.z < mp.rightFront.z)
	{
		m_pos.z -= m_move.z;		// �O
	}
	if (m_pos.z > mp.leftBack.z)
	{
		m_pos.z -= m_move.z;		// ��
	}

	// �v���C���[�̈ʒu�Z�b�g
	MV1SetPosition(m_pModel->GetModel(), m_pos);
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
	MV1SetRotationXYZ(m_pModel->GetModel(), VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}


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
		if (m_isJump)
		{
			// �x�������̑��x���d�͕����Z����
			m_jumpPower -= m_gravity;
			m_gravity += 0.005f;
		}

		// �ړ��x�N�g���̂x�������x�������̑��x�ɂ���
		m_move.y = m_jumpPower;
	}
	else {
		m_isJump = false;
		m_pos.y = 0.0f;
		m_gravity = kGravity;
		m_pState->EndState();
	}
}