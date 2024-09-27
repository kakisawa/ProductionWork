#include "DxLib.h"
#include "Input.h"
#include "Sound.h"
#include "UIBattle.h"
#include "EffectManager.h"
#include "LoadData.h"
#include "Camera.h"
#include "Stage.h"
#include "EnemyBase.h"
#include "DebugDraw.h"
#include "Player.h"

// �萔
namespace
{
	/*�v���C���[���*/
	constexpr float kMaxGauge = 100.0f;				// �ő�Q�[�W��
	constexpr float kPunchGaugeCharge = 2.8f;		// �p���`���ɑ�����Q�[�W��
	constexpr float kKickGaugeCharge = 5.0f;		// �L�b�N���ɑ�����Q�[�W��
	constexpr float kDecreaseGauge = 0.0f;			// �U�����󂯂��ۂɌ���Q�[�W��
	constexpr float kAttackDist = 50.0f;			// �U���͈�
	constexpr float kAttackMove = 0.3f;				// �U�����̈ړ���
	constexpr float kHPRecoveryRate = 1.0f;			// �v���C���[��HP���񕜂��銄��
	constexpr float kAngleSpeed = 0.2f;				// �v���C���[�p�x�̕ω����x
	constexpr float kScale = 0.3f;					// �v���C���[���f���̊g�嗦
	constexpr float kAdj = 3.0f;					// �G�ɓ����������̈ʒu������
	const VECTOR kInitDir = VGet(0.0f, 0.0f, 0.0f);	// ��������
	constexpr float kEffectHeight = 30.0f;			// �G�t�F�N�g��\�����鍂��

	/*�A�j���[�V�������*/
	constexpr float kAnimBlendMax = 1.0f;			// �A�j���[�V�����u�����h�̍ő�l

	/*�R���g���[���[*/
	constexpr int kVibrationPower = 1000;				// �U���̋���
	constexpr int kVibrationTime = 150;					// �U�������鎞��
	constexpr int kSpecialAttackVibrationPower = 150;	// �K�E�Z���̐U���̋���
	constexpr int kSpecialAttackVibrationTime = 10;		// �K�E�Z���̐U�������鎞��
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player():
	m_gauge(0.0f),
	m_pToEVec(VGet(0.0f, 0.0f, 0.0f)),
	m_targetMoveDir(kInitDir),
	m_isAccumulateGaugeSe(false)
{
	// �L�����N�^�[����ǂݍ���
	m_pLoadData = std::make_shared<LoadData>(*this, static_cast<int>(CharaType::kPlayer));
	m_pUIBattle = std::make_shared<UIBattle>(m_status.maxHp, static_cast<int>(CharaType::kPlayer));

	m_hp = m_status.maxHp;
	m_moveSpeed = 0.0f;
	m_angle = 0.0f;
	m_punchCount = 0;
	m_punchComboTime = 0;
	m_punchCoolTime = 0;
	m_kickCoolTime = 0;
	m_attackTime = 0;
	m_avoidCoolTime = 0;
	m_avoidCount = 0;
	m_isMove = false;
	m_isFighting = false;
	m_modelHandle = MV1LoadModel("data/Model/Chara/Player.mv1");

	MV1SetScale(m_modelHandle, VGet(kScale, kScale, kScale));
	m_animBlendRate = kAnimBlendMax;
	PlayAnim(AnimKind::kFightIdle);
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
	MV1DeleteModel(m_modelHandle);
}


/// <summary>
/// ������
/// </summary>
void Player::Init(std::shared_ptr<EffectManager> pEffect, VECTOR pos)
{
	m_pos = pos;
	m_pEffect = pEffect;
	m_isStartProduction = true;
	m_isClearProduction = false;
	m_isGameoverProduction = false;
	m_isAttack = false;
	m_isSpecialAttack = false;
	m_targetMoveDir = kInitDir;
	MV1SetPosition(m_modelHandle, m_pos);
	m_currentState = CharacterBase::State::kFightIdle;
	PlayAnim(AnimKind::kFightIdle);

	m_pEffect->Init();	// �G�t�F�N�g�̏�����

	// ���f���S�̂̃R���W�������̃Z�b�g�A�b�v
	MV1SetupCollInfo(m_modelHandle, -1);
}


/// <summary>
/// �X�V
/// </summary>
/// <param name="input">���̓R�}���h</param>
/// <param name="stage">�X�e�[�W���Q��</param>
void Player::Update(const Input& input, const Camera& camera, EnemyBase& enemy, Stage& stage)
{
	// �p�b�h���͂ɂ���Ĉړ��p�����[�^��ݒ肷��
	VECTOR	upMoveVec;		// ��{�^������͂������Ƃ��̃v���C���[�̈ړ������x�N�g��
	VECTOR	leftMoveVec;	// ���{�^������͂������Ƃ��̃v���C���[�̈ړ������x�N�g��
	VECTOR	moveVec;		// ���̃t���[���̈ړ��x�N�g��

	// �v���C���[�̏�Ԃ��X�V
	CharacterBase::State prevState = m_currentState;

	// �U�������̍X�V
	m_attackTime--;

	// ����̏�Ԓ��͂ق��̏������ł��Ȃ��悤�ɂ���
	const bool isUpdate = (m_currentState == CharacterBase::State::kDown) || !m_isSpecialAttack && !m_isStartProduction && !m_isClearProduction && !m_isGameoverProduction;
	if(isUpdate)
	{	
		Punch(input);						// �p���`����
		Kick(input);						// �L�b�N����
		Avoid(input, stage, moveVec);		// �������
		Guard(input);						// �K�[�h����
		Fighting(input);					// �\������
	}
	else if (m_isClearProduction)
	{
		DestroyEnemy();						// �G���j������

	}
	else if (m_isGameoverProduction)
	{
		Gameover();							// �Q�[���I�[�o�[����
	}

	if (!m_isClearProduction && !m_isGameoverProduction)
	{
		SpecialAttack(input, enemy);		// �K�E�Z����
	}

	m_currentState = UpdateMoveParameter(input, camera, upMoveVec, leftMoveVec, moveVec); // �ړ�����

	// �G�l�~�[�Ƃ̓����蔻����`�F�b�N����
	enemy.CheckHitPlayerCol(*this, VGet(m_pos.x, m_pos.y + m_colInfo.bodyHeight, m_pos.z), m_pos, m_colInfo.bodyRadius);
	// �v���C���[����G�̃x�N�g�����v�Z����
	m_pToEVec = VSub(enemy.GetPos(), m_pos);

	UpdateAnimState(prevState);	// �A�j���[�V������Ԃ��X�V
	UpdateAngle(enemy);			// �v���C���[�̈ړ�������ݒ�
	Move(moveVec, stage);		// �ړ��x�N�g�������Ƀv���C���[���ړ�������
	UpdateAnim();				// �A�j���[�V���������̍X�V
	UpdateCol();				// �����蔻��̈ʒu�X�V
	UpdatePosLog();				// �ʒu���O���X�V
	m_pUIBattle->UpdateHpBar();	// HP�o�[�̍X�V
	m_pEffect->Update();		// �G�t�F�N�g�X�V
}


/// <summary>
/// �`��
/// </summary>
void Player::Draw()
{
	MV1DrawModel(m_modelHandle);					  // �v���C���[�`��
	m_pEffect->Draw();								  // �G�t�F�N�g�`��
	m_pUIBattle->DrawPlayerSilhouette();			  // �V���G�b�g�`��
	m_pUIBattle->DrawPlayerName();					  // �v���C���[���\��
	m_pUIBattle->DrawPlayerHP(m_hp);				  // HP�Q�[�W��\��
	m_pUIBattle->DrawPlayerGauge(m_gauge, kMaxGauge); // �Q�[�W�\��

	// ��𒆂͎c����\������
	if (m_currentState == State::kAvoid)
	{
		DrawAfterImage();
	}

	// �K�E�Z�̕�����\������
	m_pUIBattle->DrawSpecialAttack(m_gauge, kMaxGauge);


#ifdef _DEBUG	// �f�o�b�O�\��
	DebugDraw debug;
	debug.DrawPlayerInfo(m_pos, m_hp, static_cast<int>(m_currentState), m_attackTime);
	// �����蔻��`��
	debug.DrawBodyCol(m_col.bodyTopPos, m_col.bodyBottomPos, m_colInfo.bodyRadius); // �S�g
	debug.DrawAimCol(m_col.armStartPos, m_col.armEndPos, m_colInfo.aimRadius);		// �r
	debug.DrawLegCol(m_col.legStartPos, m_col.legEndPos, m_colInfo.legRadius);		// �r
#endif
}


/// <summary>
/// �_���[�W���󂯂��ۂ̏���
/// </summary>
/// <param name="damage">�_���[�W��</param>
void Player::OnDamage(float damage)
{
	CharacterBase::OnDamage(damage);

	// �K�[�h��Ԃ̏ꍇ
	if (m_currentState == CharacterBase::State::kGuard)
	{
		m_pEffect->PlayGuardEffect(VGet(m_pos.x, m_pos.y + kEffectHeight, m_pos.z)); // �G�t�F�N�g�\��
		if (!CheckSoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kGuardAttack)]))
		{
			PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kGuardAttack)], DX_PLAYTYPE_BACK);	// �K�[�h���U��SE�Đ�
		}

		// �������Ɉړ�����
		VECTOR backMoveVec = VScale(m_targetMoveDir, -1.0f);
		m_pos = VAdd(m_pos, VScale(backMoveVec, m_status.backMove));
	}
	else
	{
		m_isAttack = false;
		Receive();
		OffGuard();
	}
}


/// <summary>
/// �񕜏���
/// </summary>
void Player::Recovery()
{
	// �������̑O��HP��S�񕜂���
	m_hp = std::min(m_hp + m_status.maxHp * kHPRecoveryRate, m_status.maxHp);
}


/// <summary>
/// �G�Ƃ̓����蔻����`�F�b�N����
/// </summary>
/// <param name="enemy">�G�̎Q��</param>
/// <param name="eCapPosTop">�G�̃J�v�Z���̏�ʒu</param>
/// <param name="eCapPosBottom">�G�̃J�v�Z���̉��ʒu</param>
/// <param name="CapRadius">�G�̓����蔻��̔��a</param>
void Player::CheckHitEnemyCol(EnemyBase& enemy, VECTOR eCapPosTop, VECTOR eCapPosBottom, float eCapRadius)
{
	// �v���C���[�ƓG�̓����蔻����s��
	bool isHit = HitCheck_Capsule_Capsule(m_col.bodyTopPos, m_col.bodyBottomPos, m_colInfo.bodyRadius, eCapPosTop, eCapPosBottom, eCapRadius);
	// �p���`
	bool isHitPunch = HitCheck_Capsule_Capsule(m_col.armStartPos, m_col.armEndPos, m_colInfo.aimRadius, eCapPosTop, eCapPosBottom, eCapRadius);
	// �L�b�N
	bool isHitKick = HitCheck_Capsule_Capsule(m_col.legStartPos, m_col.legEndPos, m_colInfo.legRadius, eCapPosTop, eCapPosBottom, eCapRadius);

	// �w�ォ��U���������ǂ���
	bool isBackAttack = m_pToEVec.x < 0.0f;

	// �p���`��Ԃ��ǂ���
	bool isStatePunch = m_currentState == CharacterBase::State::kPunch1 || m_currentState == CharacterBase::State::kPunch2 || m_currentState == CharacterBase::State::kPunch3;

	// �p���`�����������ꍇ
	if (isHitPunch && isStatePunch)
	{
		if (m_attackTime > 0) return;
		AdjAttackPos(enemy); // �G�ɋ߂Â�

		// �G���K�[�h���Ă��Ȃ����A�w�ォ��U�������ꍇ
		if (!enemy.GetIsGuard() || isBackAttack)
		{
			VibrationPad(); // �p�b�h��U��������

			// 1�R���{��
			if (m_currentState == CharacterBase::State::kPunch1)
			{
				enemy.OnDamage(m_status.punchPower);
				m_attackTime = m_status.punchTime;
				m_gauge += kPunchGaugeCharge;	// �K�E�Z�Q�[�W�𑝂₷
			}
			// 2�R���{��
			if (m_currentState == CharacterBase::State::kPunch2)
			{
				enemy.OnDamage(m_status.secondPunchPower);
				m_attackTime = m_status.punchTime;
				m_gauge += kPunchGaugeCharge;
			}
			// 3�R���{��
			if (m_currentState == CharacterBase::State::kPunch3)
			{
				enemy.OnDamage(m_status.thirdPunchPower);
				m_attackTime = m_status.punchTime;
				m_gauge += kPunchGaugeCharge;
			}
		}
		else
		{
			enemy.OnDamage(0.0f);
		}
	}
	// �L�b�N�����������ꍇ
	else if (isHitKick && m_currentState == CharacterBase::State::kKick)
	{
		if (m_attackTime > 0) return;
		AdjAttackPos(enemy); // �G�ɋ߂Â�

		if (!enemy.GetIsGuard() || isBackAttack)
		{
			enemy.OnDamage(m_status.kickPower);
			VibrationPad();	 // �p�b�h��U��������
			m_attackTime = m_status.kickTime;
			m_gauge += kKickGaugeCharge;
		}
		else
		{
			enemy.OnDamage(0.0f);
		}
	}
	// �U�����łȂ��A�G�ɓ��������ꍇ
	else if(isHit)
	{
		// �v���C���[�̈ʒu��␳����
		VECTOR collisionNormal = VNorm(VSub(m_pos, enemy.GetPos()));
		m_pos = VAdd(m_pos, VScale(collisionNormal, kAdj));
	}

	// �݂͂����܂�Ȃ������ꍇ
	if(!isHit && m_currentState == CharacterBase::State::kGrab)
	{
		// �͂ݎ��s�̃A�j���[�V�������Đ�����
		PlayAnim(CharacterBase::AnimKind::kStumble);
	}

	// �Q�[�W�����܂�����SE���Đ�����
	if (m_gauge >= kMaxGauge && !m_isAccumulateGaugeSe)
	{
		if (!CheckSoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kAccumulateGauge)]))
		{
			PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kAccumulateGauge)], DX_PLAYTYPE_BACK);
			m_isAccumulateGaugeSe = true;
		}
	}

	if (m_gauge < kMaxGauge)
	{
		m_isAccumulateGaugeSe = false;
	}

	// �Q�[�W�ʂ̒���
	m_gauge = std::min(m_gauge, kMaxGauge);
	m_gauge = std::max(0.0f, m_gauge);
}


/// <summary>
/// �v���C���[�̈ړ�����
/// </summary>
/// <param name="MoveVec">�ړ��x�N�g��</param>
/// <param name="stage">�X�e�[�W�Q��</param>
void Player::Move(const VECTOR& moveVec, Stage& stage)
{
	if (fabs(moveVec.x) > 0.0f || fabs(moveVec.z) > 0.0f)
	{
		m_isMove = true;
	}
	else
	{
		m_isMove = false;
	}

	// �����蔻����s���č��W��ۑ�����
	m_pos = stage.CheckPlayerCol(*this, moveVec);

	// �v���C���[�̍��W���X�V����
	MV1SetPosition(m_modelHandle, m_pos);
}


/// <summary>
/// �p���`�U������
/// </summary>
/// <param name="input">���͏���</param>
/// <returns>���݂̏��</returns>
void Player::Punch(const Input& input)
{
	// ����̏�Ԓ��̓X�L�b�v
	const bool isSkip = m_isAttack || m_isReceive || m_isGuard || m_isClearProduction || m_hp <= 0;
	if (isSkip) return;

	// �p���`�ł��Ȃ��ꍇ
	if (m_punchCoolTime > 0)
	{
		m_punchCoolTime--;
		return;
	}

	// �R���{���͂̎�t���Ԃ̍X�V
	m_punchComboTime--;

	if (input.IsTriggered("punch"))
	{
		m_isGuard = false;

		UpdateAttackAngle(); // �U���p�x���X�V

		// �R���{���͎�t���ԓ��Ƀ{�^���������ꂽ�ꍇ
		if (m_punchComboTime > 0)
		{
			m_punchCount++;
		}
		else
		{
			m_punchCount = 0;
		}

		// �R���{���ɂ���ăp���`��ς���
		switch (m_punchCount)
		{
		case 0:
			m_punchComboTime = m_status.punchReceptionTime; // �R���{���͂̎�t���Ԃ����Z�b�g
			m_isAttack = true;
			m_isFighting = false;
			m_currentState = CharacterBase::State::kPunch1;
			PlayAnim(AnimKind::kPunch1);
			break;
		case 1:
			m_punchComboTime = m_status.punchReceptionTime;
			m_isAttack = true;
			m_isFighting = false;
			m_currentState = CharacterBase::State::kPunch2;
			PlayAnim(AnimKind::kPunch2);
			break;
		case 2:
			m_punchComboTime = m_status.punchReceptionTime;
			m_isAttack = true;
			m_isFighting = false;
			m_currentState = CharacterBase::State::kPunch3;
			PlayAnim(AnimKind::kPunch3);
			break;
		case 3:
			m_punchCount = -1;
			m_punchCoolTime = m_status.punchCoolTime;	// �N�[���_�E���^�C����ݒ�
			break;
		default:
			break;
		}
	}
}


/// <summary>
/// �L�b�N�U������
/// </summary>
/// <param name="input">���͏���</param>
/// <returns>���݂̏��</returns>
void Player::Kick(const Input& input)
{
	// ����̏�Ԓ��̓X�L�b�v
	const bool isSkip = m_isAttack || m_isReceive || m_isClearProduction || m_isGuard || m_hp <= 0;;
	if (isSkip) return;

	UpdateAttackAngle(); // �U���p�x���X�V

	// �L�b�N�ł��Ȃ��ꍇ
	if (m_kickCoolTime > 0)
	{
		m_kickCoolTime--;
		return;
	}
	// �L�b�N�U��
	else if (input.IsTriggered("kick"))
	{
		m_isAttack = true;
		m_isFighting = false;
		m_isGuard = false;
		m_kickCoolTime = m_status.kickCoolTime;	// �N�[���_�E���^�C����ݒ�
		m_currentState = CharacterBase::State::kKick;
		PlayAnim(AnimKind::kKick);
	}
}


/// <summary>
/// �������
/// </summary>
/// <param name="input">���͏���</param>
/// <returns>���݂̏��</returns>
void Player::Avoid(const Input& input, Stage& stage, VECTOR& moveVec)
{
	// ����̏�Ԓ��̓X�L�b�v
	const bool isSkip = m_isGuard || m_isClearProduction || m_hp <= 0;
	if (isSkip) return;

	// ����ł��Ȃ��ꍇ
	if (m_avoidCoolTime > 0)
	{
		m_avoidCoolTime--;
		return;
	}

	// �{�^�����������ꍇ
	if (input.IsTriggered("avoidance"))
	{
		m_isFighting = false;
		m_isGuard = false;
		m_avoidCount++;
		// ��𐔂��ő�ɂȂ����ꍇ
		if (m_avoidCount > m_status.maxAvoidCount)
		{
			m_avoidCount = 0;
			m_avoidCoolTime = m_status.avoidCoolTime;	// �N�[���_�E���^�C����ݒ�
		}
		else
		{
			PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kAvoid)], DX_PLAYTYPE_BACK); // SE�Đ�
			m_currentState = CharacterBase::State::kAvoid;

			// �ړ��x�N�g����ݒ肷��
			// �����L�[����͒��͓��͕����ֈړ�����
			if (input.IsPressing("up") || input.IsPressing("down") || input.IsPressing("left") || input.IsPressing("right"))
			{
				m_pos = VAdd(m_pos, VScale(m_targetMoveDir, m_status.avoidDist));
			}
			// �����L�[�����͂���Ă��Ȃ��ꍇ�͌��ֈړ�����
			else
			{
				VECTOR backMoveVec = VScale(m_targetMoveDir, -1.0f);
				m_pos = VAdd(m_pos, VScale(backMoveVec, m_status.avoidDist));
			}
		}
	}
}


/// <summary>
/// �\��������
/// </summary>
/// <param name="input">���͏���</param>
/// <param name="enemy">�G���Q��</param>
/// <returns>���݂̏��</returns>
void Player::Fighting(const Input& input)
{
	// ����̏�Ԓ��̓X�L�b�v
	const bool isSkip = m_isAttack || m_isReceive || m_isGuard || m_isClearProduction || m_hp <= 0;
	if (isSkip) return;

	if (input.IsTriggered("fighting"))
	{
		m_isFighting = true;
		m_isAttack = false;
		m_isGuard = false;
		m_currentState = CharacterBase::State::kFightWalk;
		PlayAnim(AnimKind::kFightWalk);
	}
	else if(input.IsReleased("fighting"))
	{
		m_isFighting = false;
		m_currentState = CharacterBase::State::kFightIdle;
		PlayAnim(AnimKind::kFightIdle);
	}
}


/// <summary>
/// �K�[�h����
/// </summary>
/// <param name="input">���͏���</param>
/// <returns>���݂̏��</returns>
void Player::Guard(const Input& input)
{
	if (m_isClearProduction || m_hp <= 0) return; // ���o���͑���ł��Ȃ��悤�ɂ���

	UpdateAttackAngle(); // �U���p�x���X�V
	
	if (input.IsTriggered("guard"))
	{
		m_isAttack = false;
		m_isFighting = false;
		m_isGuard = true;
		m_attackTime = 0;
		m_currentState = CharacterBase::State::kGuard;
		PlayAnim(AnimKind::kGuard);
	}
	else if (input.IsReleased("guard"))
	{
		OffGuard();
	}
}


/// <summary>
/// �K�[�h��Ԃ���������
/// </summary>
void Player::OffGuard()
{
	m_isGuard = false;
	m_currentState = CharacterBase::State::kFightIdle;
	PlayAnim(AnimKind::kFightIdle);
}


/// <summary>
/// �U�����󂯂Ă���Œ��̏���
/// </summary>
void Player::Receive()
{
	m_currentState = CharacterBase::State::kReceive;
	PlayAnim(CharacterBase::AnimKind::kReceive);
	m_pEffect->PlayDamageEffect(VGet(m_pos.x, m_pos.y + kEffectHeight, m_pos.z));				 // �U���G�t�F�N�g�\��
	PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kAttack)], DX_PLAYTYPE_BACK); // �U��SE�Đ�
	// �Q�[�W�����炷
	m_gauge -= kDecreaseGauge;
	m_pUIBattle->ResetSpecialAttack();
}


/// <summary>
/// �K�E�Z�̏���
/// </summary>
/// <param name="input">���͏���</param>
void Player::SpecialAttack(const Input& input, EnemyBase& enemy)
{
	if (m_isSpecialAttack)
	{
		VibrationPad(); // �p�b�h��U��������
		enemy.OnDamage(m_status.specialAttackPower);
	}

	// �v���C���[�ƓG�̋��������߂�
	float pToEDist = VSize(m_pToEVec);

	// �Q�[�W�����܂��Ă��Ȃ��A�܂��͓G���߂��ɂ��Ȃ��Ƃ��͕K�E�Z���o���Ȃ��悤�ɂ���
	if (m_gauge < kMaxGauge || pToEDist >= kAttackDist) return;

	// �{�^������������K�E�Z�𔭓�����
	if (input.IsTriggered("special"))
	{
		m_isSpecialAttack = true;
		m_gauge = 0.0f;	// �Q�[�W�����炷

		m_currentState = State::kSpecialAttack;
		PlayAnim(AnimKind::kSpecialAttack);

		if (!CheckSoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kSpecialAttack)]))
		{
			PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kSpecialAttack)], DX_PLAYTYPE_BACK); // SE��炷
		}
	}
}


/// <summary>
/// �U�����Ɉʒu���ʒu�𒲐�����
/// </summary>
void Player::AdjAttackPos(EnemyBase& enemy)
{
	if (VSize(m_pToEVec) > kAttackDist)
	{
		m_pos = VAdd(m_pos, VScale(m_pToEVec, kAttackMove));
	}
}


/// <summary>
/// �G��|�������̏���
/// </summary>
void Player::DestroyEnemy()
{
	// �ҋ@��Ԃɂ���
	m_currentState = State::kFightIdle;
	PlayAnim(AnimKind::kFightIdle);
}


/// <summary>
/// �Q�[���I�[�o�[���̏���
/// </summary>
void Player::Gameover()
{
	m_isAttack = false;
	m_isFighting = false;
	m_isGuard = false;
	// �_�E����Ԃɂ���
	m_currentState = CharacterBase::State::kDown;
	PlayAnim(CharacterBase::AnimKind::kDown);
}


/// <summary>
/// �ړ��p�����[�^��ݒ肷��
/// </summary>
/// <param name="input">���͏���</param>
/// <param name="camera">�J����</param>
/// <param name="upMoveVec">������ւ̈ړ��x�N�g��</param>
/// <param name="leftMoveVec">�������ւ̈ړ��x�N�g��</param>
/// <param name="moveVec">�ړ��x�N�g��</param>
/// <returns>���݂̏��</returns>
Player::CharacterBase::State Player::UpdateMoveParameter(const Input& input, const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec, VECTOR& moveVec)
{
	CharacterBase::State nextState = m_currentState;

	/*�v���C���[�̈ړ������x�N�g�������߂�*/
	// ��{�^�����������Ƃ�
	upMoveVec = VSub(camera.GetAngle(), camera.GetPos());
	upMoveVec.y = 0.0f;

	// ���{�^�����������Ƃ�
	leftMoveVec = VCross(upMoveVec, VGet(0.0f, 1.0f, 0.0f));

	// �x�N�g���̐��K��
	upMoveVec = VNorm(upMoveVec);
	leftMoveVec = VNorm(leftMoveVec);

	// ���̃t���[���ł̈ړ��x�N�g����������
	moveVec = VGet(0.0f, 0.0f, 0.0f);

	// �ړ�������(true:�ړ�����)
	bool isPressMove = false;

	// ����̏�Ԓ��͓����Ȃ��悤�ɂ���
	const bool isMove = !m_isAttack && !m_isSpecialAttack && !m_isGuard && !m_isStartProduction && !m_isClearProduction && !m_isGameoverProduction;
	if (isMove)
	{
		// �{�^������������ړ�
		if (input.IsPressing("up"))
		{
			moveVec = VAdd(moveVec, upMoveVec);
			isPressMove = true;
		}
		if (input.IsPressing("down"))
		{
			moveVec = VAdd(moveVec, VScale(upMoveVec, -1.0f));
			isPressMove = true;
		}
		if (input.IsPressing("left"))
		{
			moveVec = VAdd(moveVec, leftMoveVec);
			isPressMove = true;
		}
		if (input.IsPressing("right"))
		{
			moveVec = VAdd(moveVec, VScale(leftMoveVec, -1.0f));
			isPressMove = true;
		}

		// �ړ��{�^����������Ă���ꍇ
		if (isPressMove)
		{
			// �ҋ@��Ԃ̏ꍇ
			if (m_currentState == CharacterBase::State::kFightIdle)
			{
				nextState = CharacterBase::State::kRun; // �ړ���Ԃɂ���
			}

			// �v���C���[������������ݒ肷��
			m_targetMoveDir = VNorm(moveVec);

			// �v���C���[�̉����x��ݒ肷��
			if (m_moveSpeed < m_status.maxMoveSpeed)
			{
				m_moveSpeed += m_status.acceleration;
				m_moveSpeed = (std::min)(m_moveSpeed, m_status.maxMoveSpeed);
			}
			// �v���C���[�̈ړ��x�N�g����ݒ肷��
			// �\�����͈ړ����x��x������
			if (m_isFighting)
			{
				moveVec = VScale(m_targetMoveDir, m_status.fightWalkSpeed);
			}
			else
			{
				moveVec = VScale(m_targetMoveDir, m_moveSpeed);
			}
		}
		// �ړ����Ȃ��ꍇ
		else
		{
			// �ړ���Ԃ̏ꍇ
			if (m_currentState == CharacterBase::State::kRun)
			{
				// �ҋ@��Ԃɂ���
				nextState = CharacterBase::State::kFightIdle;
				m_moveSpeed = 0.0f;
			}

			// �v���C���[������������
			if (m_moveSpeed > 0.0f)
			{
				m_moveSpeed -= m_status.deceleration;
				m_moveSpeed = (std::max)(0.0f, m_moveSpeed);
			}
			moveVec = VScale(m_targetMoveDir, m_moveSpeed);
		}
	}

	return nextState;
}


/// <summary>
/// �v���C���[�̉�]�𐧌䂷��
/// </summary>
void Player::UpdateAngle(EnemyBase& enemy)
{
	// �v���C���[�̊p�x���X�V
	if (m_isFighting || m_isSpecialAttack)
	{
		// �G�̕�����������
		VECTOR dir = VSub(enemy.GetPos(), m_pos);
		m_angle = atan2f(dir.x, dir.z);
	}
	else
	{
		m_angle = atan2f(m_targetMoveDir.x, m_targetMoveDir.z);
	}
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}


/// <summary>
/// �U�����̊p�x���X�V����
/// </summary>
void Player::UpdateAttackAngle()
{
	// �G������̊p�x���������Ă���ꍇ�A�G�̕����������悤�ɂ���
	// ���ς��g���Ċp�x�����߂�
	float dot = VDot(VNorm(m_pToEVec), VNorm(m_targetMoveDir));
	float angle = acos(dot) * (180.0f / DX_PI_F);
	if (angle <= 90.0f)
	{
		m_angle = atan2f(m_targetMoveDir.x, m_targetMoveDir.z);
	}
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}


/// <summary>
/// �p�b�h��U��������
/// </summary>
void Player::VibrationPad()
{
	if (m_isSpecialAttack)
	{
		StartJoypadVibration(DX_INPUT_PAD1, kSpecialAttackVibrationPower, kSpecialAttackVibrationTime, -1);
	}
	else
	{
		StartJoypadVibration(DX_INPUT_PAD1, kVibrationPower, kVibrationTime, -1);
	}
}
