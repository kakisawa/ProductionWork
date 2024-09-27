#include "DxLib.h"
#include "Input.h"
#include "EffectManager.h"
#include "Sound.h"
#include "Player.h"
#include "Stage.h"
#include "SceneStageBase.h"
#include "EnemyBase.h"
#include "DebugDraw.h"

// �萔
namespace
{
	constexpr float kInitAngle = 180.0f;	// �J�n���̓G�̌�������
	constexpr float kEffectHeight = 30.0f;	// �G�t�F�N�g��\�����鍂��
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
EnemyBase::EnemyBase() :
	m_enemyType(0),
	m_stopTime(0),
	m_angleIntervalTime(0),
	m_intervalTime(0),
	m_guardTime(0),
	m_eToPDirVec(VGet(0.0f, 0.0f, 0.0f)),
	m_shadowMap(-1)
{
	m_currentState = CharacterBase::State::kFightIdle;
	m_angle = kInitAngle; // �^���ʂ������悤�ɂ���
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
EnemyBase::~EnemyBase()
{
}


/// <summary>
/// �X�V
/// </summary>
/// <param name="player"></param>
/// <param name="stage"></param>
/// <param name="sceneStage"></param>
void EnemyBase::Update(Player& player, Stage& stage, SceneStageBase& sceneStage)
{
	// �ړ��p�����[�^��ݒ肷��
	VECTOR	upMoveVec;		// ��{�^������͂������Ƃ��̃v���C���[�̈ړ������x�N�g��
	VECTOR	leftMoveVec;	// ���{�^������͂������Ƃ��̃v���C���[�̈ړ������x�N�g��
	VECTOR	moveVec;		// ���̃t���[���̈ړ��x�N�g��

	// �G�l�~�[�̏�Ԃ��X�V
	CharacterBase::State prevState = m_currentState;

	// �U�������̍X�V
	m_attackTime--;

	// �G�̈ʒu����v���C���[�ʒu�܂ł̃x�N�g�������߂�
	m_eToPDirVec = VSub(player.GetPos(), m_pos);

	// ��Ԃ��X�V����
	m_currentState = UpdateState(player, sceneStage, upMoveVec, leftMoveVec, moveVec);

	// �v���C���[�Ƃ̓����蔻����`�F�b�N����
	player.CheckHitEnemyCol(*this, VGet(m_pos.x, m_pos.y + m_colInfo.bodyHeight, m_pos.z), m_pos, m_colInfo.bodyRadius);

	UpdateAnimState(prevState);		// �A�j���[�V������Ԃ��X�V
	UpdateAngle();					// �p�x���X�V
	UpdateGuard();					// �K�[�h��Ԃ��X�V
	Move(moveVec, player, stage);	// �ړ��x�N�g�������ɃG�l�~�[���ړ�������
	UpdateAnim();					// �A�j���[�V���������̍X�V
	UpdateCol();					// �����蔻��ʒu�X�V
	UpdatePosLog();					// �ʒu���O���X�V
	m_pEffect->Update();			// �G�t�F�N�g�X�V
}


/// <summary>
/// �`��
/// </summary>
void EnemyBase::Draw()
{
	MV1DrawModel(m_modelHandle);
	m_pEffect->Draw();					// �G�t�F�N�g�`��

	// ��𒆂͎c����\������
	if (m_currentState == State::kAvoid)
	{
		DrawAfterImage();
	}

#ifdef _DEBUG
	DebugDraw debug;
	debug.DrawEnemyInfo(m_pos, m_hp, static_cast<int>(m_currentState), m_attackTime);
	// �����蔻��`��
	debug.DrawBodyCol(m_col.bodyTopPos, m_col.bodyBottomPos, m_colInfo.bodyRadius); // �S�g
	debug.DrawAimCol(m_col.armStartPos, m_col.armEndPos, m_colInfo.aimRadius);		// �r
	debug.DrawLegCol(m_col.legStartPos, m_col.legEndPos, m_colInfo.legRadius);		// �r
#endif
}


/// <summary>
/// �G�̏�Ԃ��X�V����
/// </summary>
/// <param name="player">�v���C���[�Q��</param>
/// <param name="sceneStage">�V�[�����Q��</param>
/// <param name="upMoveVec">������ւ̈ړ��x�N�g��</param>
/// <param name="leftMoveVec">�������ւ̈ړ��x�N�g��</param>
/// <param name="moveVec">�ړ��x�N�g��</param>
/// <returns>���݂̏��</returns>
EnemyBase::CharacterBase::State EnemyBase::UpdateState(Player& player, SceneStageBase& sceneStage, VECTOR& upMoveVec, VECTOR& leftMoveVec, VECTOR& moveVec)
{
	CharacterBase::State nextState = m_currentState;

	// ���̃t���[���ł̈ړ��x�N�g����������
	moveVec = VGet(0.0f, 0.0f, 0.0f);

	// �X�^�[�g���o���͑ҋ@��Ԃɂ���
	if (sceneStage.GetBattleStartTime() > 0)
	{
		nextState = CharacterBase::State::kFightIdle;
		return nextState;
	}

	// �v���C���[���K�E�Z�𔭓����K�[�h����������
	if (player.GetIsSpecialAttack())
	{
		OffGuard();
	}

	// �p���`�ł��Ȃ��ꍇ
	if (m_punchCoolTime > 0)
	{
		m_punchCoolTime--;
		return m_currentState;
	}

	m_punchComboTime--; // �R���{���͂̎�t���Ԃ̍X�V

	// �L�b�N�ł��Ȃ��ꍇ
	if (m_kickCoolTime > 0)
	{
		m_kickCoolTime--;
		return m_currentState;
	}

	// ����̏ꍇ�͏�Ԃ��X�V���Ȃ�
	bool isKeepState =
		   m_isGuard
		|| m_isAttack
		|| (m_currentState == CharacterBase::State::kReceive)
		|| (m_currentState == CharacterBase::State::kDown)
		|| player.GetIsSpecialAttack()
		|| player.GetHp() <= 0;
	if (isKeepState) return nextState;

	// �G��HP��0�ɂȂ����ꍇ
	if (m_hp <= 0)
	{
		Down(); // �_�E���������s��
		return m_currentState;
	}

	// �G�l�~�[�ƃv���C���[�̋������v�Z
	m_eToPDirVec = VSub(player.GetPos(), m_pos);
	float distance = VSize(m_eToPDirVec);
	if (m_intervalTime > 0)
	{
		m_intervalTime--;	// �N�[���_�E���^�C�}�[������������
		return nextState;
	}

	// �v���C���[����苗�����ꂽ�ꍇ
	if (distance > m_enemyInfo.approachRange)
	{
		// ���b��������v���C���[�ɋ߂Â�
		if (m_stopTime < 0)
		{
			m_eToPDirVec = VNorm(m_eToPDirVec);
			moveVec = VScale(m_eToPDirVec, m_moveSpeed);
			nextState = CharacterBase::State::kRun; // �ړ���Ԃɂ���
		}
		else
		{
			m_stopTime--;
		}
	}
	// �v���C���[���U���͈͂ɓ������ꍇ
	else if (distance <= m_enemyInfo.attackRange)
	{
		// �m���ōU�����s��
		int randNum = GetRand(m_enemyInfo.maxProb);

		// �p���`�U��
		if (randNum <= m_enemyInfo.punchProb)
		{
			nextState = Punch();
		}
		// �L�b�N�U��
		else if (randNum <= m_enemyInfo.kickProb + m_enemyInfo.punchProb)
		{
			nextState = kick();
		}
		// ���
		else if (randNum <= m_enemyInfo.avoidProb + m_enemyInfo.kickProb + m_enemyInfo.punchProb)
		{
			nextState = Avoid();
		}
		// �K�[�h
		else if (randNum <= m_enemyInfo.guardProb + m_enemyInfo.avoidProb + m_enemyInfo.kickProb + m_enemyInfo.punchProb)
		{
			nextState = Guard();
		}
		// �ҋ@���
		else
		{
			nextState = Fighting();
		}

		m_intervalTime = m_enemyInfo.stateIntervalTime;
	}
	else
	{
		m_stopTime = m_enemyInfo.minStopTime + GetRand(m_enemyInfo.maxStopTime);	// �v���C���[�ɋ߂Â��܂ł̎��Ԃ������_���Ōv�Z����
		nextState = CharacterBase::State::kFightIdle;								// �ҋ@��Ԃɂ���
	}

	return nextState;
}


/// <summary>
/// �ړ�����
/// </summary>
/// <param name="MoveVec">�ړ���</param>
/// <param name="stage">�X�e�[�W���Q��</param>
void EnemyBase::Move(const VECTOR& moveVec, Player& player, Stage& stage)
{
	if (fabs(moveVec.x) > 0.0f || fabs(moveVec.z) > 0.0f)
	{
		m_isMove = true;
	}
	else
	{
		m_isMove = false;
	}

	// �X�e�[�W�Ɠ����蔻����s���č��W��ۑ�����
	m_pos = stage.CheckEnemyCol(*this, moveVec);

	// �v���C���[�̍��W���X�V����
	MV1SetPosition(m_modelHandle, m_pos);
}


/// <summary>
/// �p���`�U��
/// </summary>
/// <returns>���݂̏��</returns>
CharacterBase::State EnemyBase::Punch()
{
	CharacterBase::State nextState = m_currentState;
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
		PlayAnim(AnimKind::kPunch1);
		nextState = CharacterBase::State::kPunch1;
		break;
	case 1:
		m_punchComboTime = m_status.punchReceptionTime;
		m_isAttack = true;
		m_isFighting = false;
		PlayAnim(AnimKind::kPunch2);
		nextState = CharacterBase::State::kPunch2;
		break;
	case 2:
		m_punchComboTime = m_status.punchReceptionTime;
		m_isAttack = true;
		m_isFighting = false;
		PlayAnim(AnimKind::kPunch3);
		nextState = CharacterBase::State::kPunch3;
		break;
	case 3:
		m_isAttack = false;
		m_punchCount = 0;
		m_punchCoolTime = m_status.punchCoolTime;	// �N�[���_�E���^�C����ݒ�
		nextState = CharacterBase::State::kFightIdle;
		break;
	}

	return nextState;
}


/// <summary>
/// �L�b�N�U��
/// </summary>
/// <returns>���݂̏��</returns>
CharacterBase::State EnemyBase::kick()
{
	// �L�b�N�U��
	m_isAttack = true;
	m_isFighting = false;
	m_kickCoolTime = m_status.kickCoolTime;	// �N�[���_�E���^�C����ݒ�
	PlayAnim(AnimKind::kKick);
	return CharacterBase::State::kKick;
}


/// <summary>
/// �������
/// </summary>
/// <param name="moveVec">�ړ��x�N�g��</param>
CharacterBase::State EnemyBase::Avoid()
{
	// ����ł��Ȃ��ꍇ
	if (m_avoidCoolTime > 0)
	{
		m_avoidCoolTime--;
		return m_currentState;
	}

	PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kAvoid)], DX_PLAYTYPE_BACK); // SE�Đ�
	m_isFighting = false;
	m_avoidCount++;

	// ��𐔂��ő�ɂȂ����ꍇ
	if (m_avoidCount > m_status.maxAvoidCount)
	{
		m_avoidCount = 0;
		m_avoidCoolTime = m_status.avoidCoolTime;	// �N�[���_�E���^�C����ݒ�
	}
	else
	{
		// �����Ă�������Ƌt�����Ɉړ�����
		VECTOR backMoveVec = VScale(VNorm(m_eToPDirVec), -1.0f * m_status.avoidDist);
		m_pos = VAdd(m_pos,backMoveVec);
		return CharacterBase::State::kAvoid;
	}

	return m_currentState;
}


/// <summary>
/// �\������
/// </summary>
CharacterBase::State  EnemyBase::Fighting()
{
	m_isFighting = true;
	PlayAnim(AnimKind::kFightWalk);
	return CharacterBase::State::kFightWalk;
}


/// <summary>
/// �K�[�h����
/// </summary>
CharacterBase::State EnemyBase::Guard()
{
	if (m_isGuard)
	{
		return CharacterBase::State::kGuard;
	}

	m_isGuard = true;
	// �K�[�h�̎��Ԃ�ݒ�
	m_guardTime = m_enemyInfo.guardTime;
	return CharacterBase::State::kGuard;
}


/// <summary>
/// �K�[�h��Ԃ��X�V
/// </summary>
void EnemyBase::UpdateGuard()
{
	if (!m_isGuard) return;

	if (m_guardTime > 0)
	{
		m_guardTime--;
	}
	else
	{
		OffGuard(); // �K�[�h��Ԃ���������
	}
}


/// <summary>
/// �K�[�h��Ԃ�����
/// </summary>
CharacterBase::State EnemyBase::OffGuard()
{
	m_isGuard = false;
	m_guardTime = 0;
	return CharacterBase::State::kFightIdle;
}


/// <summary>
/// �U�����󂯂Ă���Œ��̏���
/// </summary>
/// <returns></returns>
void EnemyBase::Receive()
{ 
	m_isAttack = false; // �U����Ԃ�����
	m_currentState = CharacterBase::State::kReceive;

	if (m_attackTime > 0) return;
	PlayAnim(CharacterBase::AnimKind::kReceive);
	m_pEffect->PlayDamageEffect(VGet(m_pos.x, m_pos.y + kEffectHeight, m_pos.z));					// �U���G�t�F�N�g�Đ�
	PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kAttack)], DX_PLAYTYPE_BACK); 	// �U��SE�Đ�
}


/// <summary>
/// �_�E������
/// </summary>
void EnemyBase::Down()
{
	m_isAttack = false;
	m_isFighting = false;
	m_isGuard = false;
	m_currentState = CharacterBase::State::kDown;
	PlayAnim(CharacterBase::AnimKind::kDown);
}


/// <summary>
/// �G�̊p�x���X�V
/// </summary>
void EnemyBase::UpdateAngle()
{
	m_angleIntervalTime++;

	// �ړ����͏�Ƀv���C���[�̕����������悤�ɂ���
	if(m_currentState == CharacterBase::State::kRun || m_currentState == CharacterBase::State::kAvoid)
	{
		m_angle = atan2f(m_eToPDirVec.x, m_eToPDirVec.z);
	}
	// ��莞�Ԍo�߂�����
	else if (m_angleIntervalTime >= m_enemyInfo.changeAngleFrame)
	{
		// �����_���Ńv���C���[�̕���������
		int randNum = GetRand(m_enemyInfo.maxProb);
		if (randNum <= m_enemyInfo.changeAngleProb)
		{
			m_angle = atan2f(m_eToPDirVec.x, m_eToPDirVec.z);
		}
		m_angleIntervalTime = 0;
	}

	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}


/// <summary>
/// �U�����󂯂��ۂ̏���
/// </summary>
/// <param name="damage">�_���[�W��</param>
void EnemyBase::OnDamage(float damage)
{
	CharacterBase::OnDamage(damage);

	// �K�[�h��Ԃ̏ꍇ
	if (m_currentState == CharacterBase::State::kGuard)
	{
		// �������Ɉړ�����
		VECTOR backMoveVec = VScale(m_eToPDirVec, -1.0f);
		m_pos = VAdd(m_pos, VScale(backMoveVec, m_status.backMove));
		m_pEffect->PlayGuardEffect(VGet(m_pos.x, m_pos.y + kEffectHeight, m_pos.z)); // �G�t�F�N�g�\��
		if (!CheckSoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kGuardAttack)]))
		{
			PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kGuardAttack)], DX_PLAYTYPE_BACK);	// �K�[�h���U��SE�Đ�
		}
	}
	else
	{
		Receive(); // �U�����󂯂���Ԃɂ���
	}
}


/// <summary>
/// �v���C���[�Ƃ̓����蔻����`�F�b�N����
/// </summary>
/// <param name="player">�v���C���[�Q��</param>
/// <param name="eCapPosTop">�G�̓����蔻��J�v�Z���̒��_�ʒu</param>
/// <param name="eCapPosBottom">�G�̓����蔻��J�v�Z���̒��_�ʒu</param>
/// <param name="eCapRadius">�G�̓����蔻��J�v�Z���̔��a</param>
void EnemyBase::CheckHitPlayerCol(Player& player, VECTOR eCapPosTop, VECTOR eCapPosBottom, float eCapRadius)
{
	// �v���C���[�ƓG�̓����蔻����s��
	bool isHit = HitCheck_Capsule_Capsule(m_col.bodyTopPos, m_col.bodyBottomPos, m_colInfo.bodyRadius, eCapPosTop, eCapPosBottom, eCapRadius);
	// �p���`
	bool isHitPunch = HitCheck_Capsule_Capsule(m_col.armStartPos, m_col.armEndPos, m_colInfo.aimRadius, eCapPosTop, eCapPosBottom, eCapRadius);
	// �L�b�N
	bool isHitKick = HitCheck_Capsule_Capsule(m_col.legStartPos, m_col.legEndPos, m_colInfo.legRadius, eCapPosTop, eCapPosBottom, eCapRadius);

	// �w�ォ��U���������ǂ���
	m_eToPDirVec = VNorm(m_eToPDirVec);
	bool isBackAttack = VDot(player.GetDir(), m_eToPDirVec) < 0.0f;

	// �p���`��Ԃ��ǂ���
	bool isStatePunch = m_currentState == CharacterBase::State::kPunch1 || m_currentState == CharacterBase::State::kPunch2 || m_currentState == CharacterBase::State::kPunch3;
	// �p���`�����������ꍇ
	if (isHitPunch && isStatePunch)
	{
		if (m_attackTime > 0) return;

		// �v���C���[���K�[�h���Ă��Ȃ����A�w�ォ��U�������ꍇ
		if (!player.GetIsGuard())
		{
			// 1�R���{��
			if (m_currentState == CharacterBase::State::kPunch1)
			{
				player.OnDamage(m_status.punchPower);
				m_attackTime = m_status.punchTime;
			}
			// 2�R���{��
			if (m_currentState == CharacterBase::State::kPunch2)
			{
				player.OnDamage(m_status.secondPunchPower);
				m_attackTime = m_status.punchTime;
			}
			// 3�R���{��
			if (m_currentState == CharacterBase::State::kPunch3)
			{
				player.OnDamage(m_status.thirdPunchPower);
				m_attackTime = m_status.punchTime;
			}
		}
		else
		{
			player.OnDamage(0.0f);
		}
	}
	// �L�b�N�����������ꍇ
	else if (isHitKick && m_currentState == CharacterBase::State::kKick)
	{
		if (m_attackTime > 0) return;

		// �L�b�N�����������ꍇ
		if (!player.GetIsGuard())
		{
			player.OnDamage(m_status.kickPower);
			m_attackTime = m_status.kickTime;
		}
		else
		{
			player.OnDamage(0.0f);
		}
	}

	// �݂͂����܂�Ȃ������ꍇ
	if (!isHit && m_currentState == CharacterBase::State::kGrab)
	{
		// �͂ݎ��s�̃A�j���[�V�������Đ�����
		PlayAnim(CharacterBase::AnimKind::kStumble);
	}
}


#ifdef _DEBUG
/// <summary>
/// �f�o�b�O�p
/// </summary>
void EnemyBase::DebugDamage(Input& input)
{
	if (input.IsTriggered("debug_damage"))
	{
		m_hp = 0;
	}
}
#endif
