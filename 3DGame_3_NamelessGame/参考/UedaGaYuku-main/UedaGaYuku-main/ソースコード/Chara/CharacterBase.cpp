#include "DxLib.h"
#include "LoadData.h"
#include "UIBattle.h"
#include "EffectManager.h"
#include "CharacterBase.h"

// �萔
namespace
{
	// �A�j���[�V�������
	constexpr float kAnimBlendMax = 1.0f;	 // �A�j���[�V�����u�����h�̍ő�l
	constexpr float kAnimBlendSpeed = 0.2f;	 // �A�j���[�V�����u�����h�̕ω����x
	constexpr int kPosLogNum = 8;			 // �o���Ă����ߋ��̈ʒu���̐�
	constexpr float kAlphaAdj = 0.3f;		 // �c���̓����x�𒲐�
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
CharacterBase::CharacterBase():
	m_animSpeed{0.0f},
	m_status{0.0f},
	m_colInfo{0.0f},
	m_hp(0.0f),
	m_pos(VGet(0.0f, 0.0f, 0.0f)),
	m_moveSpeed(0.0f),
	m_angle(0.0f),
	m_punchCount(0),
	m_punchComboTime(0),
	m_punchCoolTime(0),
	m_kickCoolTime(0),
	m_attackTime(0),
	m_avoidCoolTime(0),
	m_avoidCount(0),
	m_isAttack(false),
	m_isGuard(false),
	m_isReceive(false),
	m_isSpecialAttack(false),
	m_isStartProduction(false),
	m_isClearProduction(false),
	m_isGameoverProduction(false),
	m_modelHandle(-1),
	m_currentPlayAnim(-1),
	m_currentAnimCount(0.0f),
	m_prevPlayAnim(-1),
	m_prevAnimCount(0.0f),
	m_animBlendRate(0.0f)
{
	m_posLog.resize(kPosLogNum);
}


/// <summary>
/// �_���[�W���󂯂��ۂ̏���
/// </summary>
/// <param name="damage">�_���[�W��</param>
void CharacterBase::OnDamage(float damage)
{
	m_hp -= damage;
	m_hp = std::max(0.0f, m_hp);

	// HP�o�[���X�V
	if (damage > 0.0f)
	{
		m_pUIBattle->OnDamage(damage);
	}
}


/// <summary>
/// �����蔻��ʒu�X�V
/// </summary>
void CharacterBase::UpdateCol()
{
	// �v���C���[�̌��������Ƃɓ����蔻��̈ʒu�𒲐�����
	MATRIX rotationMatrix = MGetRotY(m_angle);

	// �v���C���[�S�̂̓����蔻��ʒu���X�V
	m_col.bodyTopPos = VAdd(m_pos, (VTransform(VGet(0.0f, m_colInfo.bodyHeight, 0.0f), rotationMatrix)));
	m_col.bodyBottomPos = m_pos;

	// �r�̓����蔻��ʒu���X�V
	m_col.armStartPos = VAdd(m_pos, (VTransform(m_colInfo.armStartPos, rotationMatrix)));
	m_col.armEndPos = VAdd(m_col.armStartPos, (VTransform(m_colInfo.armEndPos, rotationMatrix)));

	// �r�̓����蔻��ʒu���X�V
	m_col.legStartPos = VAdd(m_pos, (VTransform(m_colInfo.legStartPos, rotationMatrix)));
	m_col.legEndPos = VAdd(m_col.legStartPos, (VTransform(m_colInfo.legEndPos, rotationMatrix)));
}


/// <summary>
/// �A�j���[�V�����X�e�[�g���X�V
/// </summary>
/// <param name="prevState">1�O�̃A�j���[�V����</param>
void CharacterBase::UpdateAnimState(CharacterBase::State prevState)
{
	// �ҋ@��Ԃ���
	if (prevState == CharacterBase::State::kFightIdle)
	{
		// �ړ��A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kRun) PlayAnim(AnimKind::kRun);
		// �p���`�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kPunch1)PlayAnim(AnimKind::kPunch1);
		// �L�b�N�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kKick)	PlayAnim(AnimKind::kKick);
		// ����A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kAvoid) PlayAnim(AnimKind::kAvoid);
		// �\���A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kFightWalk) PlayAnim(AnimKind::kFightWalk);
		// �K�[�h�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kGuard);
		// �_���[�W�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kReceive);
	}
	// �ړ���Ԃ���
	else if (prevState == CharacterBase::State::kRun)
	{
		// �ҋ@�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kFightIdle) PlayAnim(AnimKind::kFightIdle);
		// �p���`�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kPunch1) PlayAnim(AnimKind::kPunch1);
		// �L�b�N�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kKick) PlayAnim(AnimKind::kKick);
		// ����A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kAvoid) PlayAnim(AnimKind::kAvoid);
		// �\���A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kFightWalk) PlayAnim(AnimKind::kFightWalk);
		// �K�[�h�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kGuard);
		// �_���[�W�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kReceive);
	}
	// �p���`���(1�R���{��)����
	else if (prevState == CharacterBase::State::kPunch1)
	{
		// �ҋ@�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kFightIdle) PlayAnim(AnimKind::kFightIdle);
		// �ړ��A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kRun) PlayAnim(AnimKind::kRun);
		// 2�R���{�ڃA�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kPunch2) PlayAnim(AnimKind::kPunch2);
		// �L�b�N�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kKick) PlayAnim(AnimKind::kKick);
		// ����A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kAvoid) PlayAnim(AnimKind::kAvoid);
		// �\���A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kFightWalk) PlayAnim(AnimKind::kFightWalk);
		// �K�[�h�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kGuard);
		// �_���[�W�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kReceive);
	}
	// �p���`���(2�R���{��)����
	else if (prevState == CharacterBase::State::kPunch2)
	{
		// �ҋ@�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kFightIdle) PlayAnim(AnimKind::kFightIdle);
		// �ړ��A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kRun) PlayAnim(AnimKind::kRun);
		// 3�R���{�ڃA�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kPunch3) PlayAnim(AnimKind::kPunch3);
		// �L�b�N�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kKick) PlayAnim(AnimKind::kKick);
		// ����A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kAvoid) PlayAnim(AnimKind::kAvoid);
		// �\���A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kFightWalk) PlayAnim(AnimKind::kFightWalk);
		// �K�[�h�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kGuard);
		// �_���[�W�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kReceive);
	}
	// �p���`���(3�R���{��)����
	else if (prevState == CharacterBase::State::kPunch3)
	{
		// �ҋ@�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kFightIdle) PlayAnim(AnimKind::kFightIdle);
		// �ړ��A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kRun) PlayAnim(AnimKind::kRun);
		// 1�R���{�ڃA�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kPunch1) PlayAnim(AnimKind::kPunch1);
		// �L�b�N�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kKick) PlayAnim(AnimKind::kKick);
		// ����A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kAvoid) PlayAnim(AnimKind::kAvoid);
		// �\���A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kFightWalk) PlayAnim(AnimKind::kFightWalk);
		// �K�[�h�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kGuard);
		// �_���[�W�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kReceive);
	}
	// �L�b�N��Ԃ���
	else if (prevState == CharacterBase::State::kKick)
	{
		// �ҋ@�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kFightIdle) PlayAnim(AnimKind::kFightIdle);
		// �ړ��A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kRun) PlayAnim(AnimKind::kRun);
		// �p���`�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kPunch1) PlayAnim(AnimKind::kPunch1);
		// ����A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kAvoid) PlayAnim(AnimKind::kAvoid);
		// �\���A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kFightWalk) PlayAnim(AnimKind::kFightWalk);
		// �K�[�h�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kGuard);
		// �_���[�W�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kReceive);
	}
	// �����Ԃ���
	else if (prevState == CharacterBase::State::kAvoid)
	{
		// �ҋ@�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kFightIdle) PlayAnim(AnimKind::kFightIdle);
		// �ړ��A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kRun) PlayAnim(AnimKind::kRun);
		// �p���`�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kPunch1) PlayAnim(AnimKind::kPunch1);
		// �L�b�N�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kKick) PlayAnim(AnimKind::kKick);
		// �\���A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kFightWalk) PlayAnim(AnimKind::kFightWalk);
		// �K�[�h�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kGuard);
		// �_���[�W�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kReceive);
	}
	// �\����Ԃ���
	else if (prevState == CharacterBase::State::kFightWalk)
	{
		// �ҋ@�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kFightIdle) PlayAnim(AnimKind::kFightIdle);
		// �ړ��A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kRun) PlayAnim(AnimKind::kRun);
		// �p���`�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kPunch1) PlayAnim(AnimKind::kPunch1);
		// �L�b�N�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kKick) PlayAnim(AnimKind::kKick);
		// ����A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kAvoid) PlayAnim(AnimKind::kAvoid);
		// �K�[�h�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kGuard);
		// �_���[�W�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kReceive);
	}
	// �K�[�h��Ԃ���
	else if (prevState == CharacterBase::State::kGuard)
	{
		// �ҋ@�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kFightIdle) PlayAnim(AnimKind::kFightIdle);
		// �ړ��A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kRun) PlayAnim(AnimKind::kRun);
		// �p���`�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kPunch1) PlayAnim(AnimKind::kPunch1);
		// �L�b�N�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kKick) PlayAnim(AnimKind::kKick);
		// ����A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kAvoid) PlayAnim(AnimKind::kAvoid);
		// �\���A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kFightWalk) PlayAnim(AnimKind::kFightWalk);
	}
	// �U�����󂯂���Ԃ���
	else if (prevState == CharacterBase::State::kReceive)
	{
		// �ҋ@�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kFightIdle) PlayAnim(AnimKind::kFightIdle);
		// �ړ��A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kRun) PlayAnim(AnimKind::kRun);
		// �p���`�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kPunch1) PlayAnim(AnimKind::kPunch1);
		// �L�b�N�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kKick) PlayAnim(AnimKind::kKick);
		// ����A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kAvoid) PlayAnim(AnimKind::kAvoid);
		// �\���A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kFightWalk) PlayAnim(AnimKind::kFightWalk);
		// �K�[�h�A�j���[�V�������Đ�
		if (m_currentState == CharacterBase::State::kGuard) PlayAnim(AnimKind::kGuard);
	}
}


/// <summary>
/// �A�j���[�V�������Đ�����
/// </summary>
/// <param name="PlayAnimIndex">�Đ�����A�j���[�V�������</param>
void CharacterBase::PlayAnim(AnimKind playAnimIndex)
{
	// 1�O�̃A�j���[�V�������A�^�b�`����Ă���ꍇ�폜����
	if (m_prevPlayAnim != -1)
	{
		MV1DetachAnim(m_modelHandle, m_prevPlayAnim);
		m_prevPlayAnim = -1;
	}

	// ���ݍĐ����̃A�j���[�V������1�O�Ɉړ�����
	m_prevPlayAnim = m_currentPlayAnim;
	m_prevAnimCount = m_currentAnimCount;

	// �V���ɃA�j���[�V�������A�^�b�`����
	m_currentPlayAnim = MV1AttachAnim(m_modelHandle, static_cast<int>(playAnimIndex), -1, false);
	m_currentAnimCount = 0.0f;

	// �u�����h����Prev���L���łȂ��ꍇ�A1.0�ɂ���
	if (m_prevPlayAnim == -1)
	{
		m_animBlendRate = kAnimBlendMax;
	}
	else
	{
		m_animBlendRate = 0.0f;
	}
}


/// <summary>
/// �A�j���[�V�����̏������s��
/// </summary>
void CharacterBase::UpdateAnim()
{
	float animTotalTime; // �Đ����̃A�j���[�V��������

	// �u�����h����1�ȉ��̏ꍇ
	if (m_animBlendRate < kAnimBlendMax)
	{
		m_animBlendRate += kAnimBlendSpeed;
		m_animBlendRate = std::min(m_animBlendRate, kAnimBlendMax);
	}

	// ���ݍĐ����̃A�j���[�V�����̏���
	if (m_currentPlayAnim != -1)
	{
		// �A�j���[�V�����̑����Ԃ��擾����
		animTotalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_currentPlayAnim);

		// �A�j���[�V�����ɂ���čĐ��X�s�[�h��ς���
		if (m_currentState == CharacterBase::State::kPunch1)
		{
			m_currentAnimCount += m_animSpeed.punch1;
		}
		else if (m_currentState == CharacterBase::State::kPunch2)
		{
			m_currentAnimCount += m_animSpeed.punch2;
		}
		else if (m_currentState == CharacterBase::State::kPunch3)
		{
			m_currentAnimCount += m_animSpeed.punch3;
		}
		else if (m_currentState == CharacterBase::State::kKick)
		{
			m_currentAnimCount += m_animSpeed.kick;
		}
		else if (m_currentState == CharacterBase::State::kSpecialAttack)
		{
			m_currentAnimCount += m_animSpeed.kick;
		}
		else if (m_currentState == CharacterBase::State::kAvoid)
		{
			m_currentAnimCount += m_animSpeed.specialAttack;
		}
		else if (m_currentState == CharacterBase::State::kFightWalk)
		{
			// �ړ����̂݃A�j���[�V�������Đ�
			if (m_isMove)
			{
				m_currentAnimCount += m_animSpeed.fightWalk;
			}
			else
			{
				m_currentAnimCount = 0.0f;
			}
		}
		else if (m_currentState == CharacterBase::State::kGuard)
		{
			m_currentAnimCount += m_animSpeed.guard;
			if (m_isGuard)
			{
				m_currentAnimCount = std::min(m_currentAnimCount, m_status.guardAnimTime);
			}
		}
		else if (m_currentState == CharacterBase::State::kReceive)
		{
			m_currentAnimCount += m_animSpeed.receive;
		}
		else if (m_currentState == CharacterBase::State::kDown)
		{
			m_currentAnimCount += m_animSpeed.down;
			// �A�j���[�V�������ꎞ��~����
			if (m_currentAnimCount > animTotalTime)
			{
				m_currentAnimCount = animTotalTime;
				MV1SetAttachAnimTime(m_modelHandle, m_currentPlayAnim, m_currentAnimCount);
				MV1SetAttachAnimBlendRate(m_modelHandle, m_currentPlayAnim, m_animBlendRate);
				return;
			}
		}
		else
		{
			m_currentAnimCount += m_animSpeed.fightIdle;
		}

		// �A�j���[�V�������I������ꍇ
		if (m_currentAnimCount > animTotalTime)
		{
			// ����̃A�j���[�V�������I�������ҋ@��ԂɈڍs
			bool isAnimChange = m_currentState == CharacterBase::State::kAvoid || m_currentState == CharacterBase::State::kGuard || m_currentState == CharacterBase::State::kReceive;

			// �U���A�j���[�V�������I��������ҋ@��ԂɈڍs
			if (m_isAttack || m_isSpecialAttack)
			{
				m_isAttack = false;
				m_isSpecialAttack = false;
				m_isReceive = false;
				m_currentState = CharacterBase::State::kFightIdle;
				PlayAnim(AnimKind::kFightIdle);
			}
			else if (isAnimChange)
			{
				m_isReceive = false;
				m_currentState = CharacterBase::State::kFightIdle;
				PlayAnim(AnimKind::kFightIdle);
			}
			else
			{
				// �A�j���[�V�����̍Đ����Ԃ����[�v
				m_currentAnimCount = static_cast<float>(fmod(m_currentAnimCount, animTotalTime));
			}
		}

		// �Đ����Ԃ��X�V
		MV1SetAttachAnimTime(m_modelHandle, m_currentPlayAnim, m_currentAnimCount);
		// �A�j���[�V�����̃u�����h����ݒ肷��
		MV1SetAttachAnimBlendRate(m_modelHandle, m_currentPlayAnim, m_animBlendRate);
	}

	// 1�O�ɍĐ����Ă����A�j���[�V�����̏���
	if (m_prevPlayAnim != -1)
	{
		// �A�j���[�V�����̑����Ԃ��擾����
		animTotalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_prevPlayAnim);

		// �A�j���[�V�����ɂ���čĐ����x��ς���
		if (m_prevPlayAnim == static_cast<int>(CharacterBase::AnimKind::kPunch1))
		{
			m_prevAnimCount += m_animSpeed.punch1;
		}
		else if (m_prevPlayAnim == static_cast<int>(CharacterBase::AnimKind::kPunch2))
		{
			m_prevAnimCount += m_animSpeed.punch2;
		}
		else if (m_prevPlayAnim == static_cast<int>(CharacterBase::AnimKind::kPunch3))
		{
			m_prevAnimCount += m_animSpeed.punch3;
		}
		else if (m_prevPlayAnim == static_cast<int>(CharacterBase::AnimKind::kKick))
		{
			m_prevAnimCount += m_animSpeed.kick;
		}
		else if (m_currentState == CharacterBase::State::kSpecialAttack)
		{
			m_prevAnimCount += m_animSpeed.specialAttack;
		}
		else if (m_prevPlayAnim == static_cast<int>(CharacterBase::AnimKind::kAvoid))
		{
			m_prevAnimCount += m_animSpeed.avoid;
		}
		else if (m_currentState == CharacterBase::State::kFightWalk)
		{
			// �ړ����̂݃A�j���[�V�������Đ�
			if (m_isMove)
			{
				m_prevAnimCount += m_animSpeed.fightWalk;
			}
			else
			{
				m_prevAnimCount = 0.0f;
			}
		}
		else if (m_currentState == CharacterBase::State::kGuard)
		{
			m_prevAnimCount += m_animSpeed.guard;
			if (m_isGuard)
			{
				m_prevAnimCount = std::min(m_currentAnimCount, m_status.guardAnimTime);
			}
		}
		else if (m_currentState == CharacterBase::State::kReceive)
		{
			m_prevAnimCount += m_animSpeed.receive;
		}
		else if (m_currentState == CharacterBase::State::kDown)
		{
			m_prevAnimCount += m_animSpeed.down;
		}
		else
		{
			m_prevAnimCount += m_animSpeed.fightIdle;
		}

		// �A�j���[�V�����̍Đ����Ԃ����[�v
		if (m_prevPlayAnim > animTotalTime)
		{
			m_prevAnimCount = static_cast<float>(fmod(m_prevAnimCount, animTotalTime));
		}

		// �Đ����Ԃ��X�V
		MV1SetAttachAnimTime(m_modelHandle, m_prevPlayAnim, m_prevAnimCount);
		// �A�j���[�V�����̃u�����h����ݒ肷��
		MV1SetAttachAnimBlendRate(m_modelHandle, m_prevPlayAnim, kAnimBlendMax - m_animBlendRate);
	}
}



/// <summary>
/// �ʒu���O���X�V����
/// </summary>
void CharacterBase::UpdatePosLog()
{
	// �ʒu���O�����炷
	for (int i = static_cast<int>(m_posLog.size()) - 1; i >= 1; i--)
	{
		m_posLog[i] = m_posLog[i - 1];
	}
	// 1�t���[���O�̈ʒu��������
	m_posLog[0] = m_pos;
}


/// <summary>
/// ����̍ۂɎc����\������
/// </summary>
void CharacterBase::DrawAfterImage()
{
	for (int i = static_cast<int>(m_posLog.size()) - 1; i >= 0; i--)
	{
		// ���O���Â��Ȃ�قǓ����ɂȂ�悤�ɂ���
		float alpha = (1.0f - (static_cast<float>(i) / m_posLog.size())) * kAlphaAdj;
		MV1SetOpacityRate(m_modelHandle, alpha);
		MV1SetPosition(m_modelHandle, m_posLog[i]);
		MV1DrawModel(m_modelHandle);
	}
	MV1SetOpacityRate(m_modelHandle, 1.0f); // �����x��߂�
}
