#include "PlayerState.h"
#include "../../Util/Pad.h"
#include "DxLib.h"

PlayerState::PlayerState() :
	m_isActionState(false)
{
}

void PlayerState::Update()
{
	StateTransition();	// �e��Ԃ̌Ăяo��

	m_pNowState.stateUpdate();	// ���݂̃X�e�C�g�X�V
}

void PlayerState::AddState(std::function<void(void)> stateUpdate, std::function<void(void)> stateInit, State stateKind)
{
	// �X�e�C�g�ǉ��p�ϐ�
	StateData state;
	state.stateUpdate = stateUpdate;
	state.stateInit = stateInit;
	state.stateKind = stateKind;

	// �X�e�C�g�̒ǉ�
	m_pState.push_back(state);
}

void PlayerState::SetState(State state)
{
	for (auto& ChangeState : m_pState)
	{
		// �X�e�C�g�̎�ނ��I�΂ꂽ�X�e�C�g�̎�ނƓ�����
		if (ChangeState.stateKind == state)
		{
			m_pNowState = ChangeState;	// �X�e�C�g�̕ύX
			return;
		}
	}
}

void PlayerState::EndState()
{
	// �A�N�V�����̏I��
	m_isActionState = false;
}

void PlayerState::StateTransitionIdle()
{
	// �A�N�V�������������珈����Ԃ�
	if (m_isActionState)return;

	// �ړ��{�^����������Ă��Ȃ�������
	if (!Pad::IsPress(PAD_INPUT_RIGHT) && !Pad::IsPress(PAD_INPUT_LEFT) &&
		!Pad::IsPress(PAD_INPUT_UP) && !Pad::IsPress(PAD_INPUT_DOWN))
	{
		ChangeState(State::kIdle);
	}
}

void PlayerState::StateTransitionWalk()
{
	// �A�N�V�������������珈����Ԃ�
	if (m_isActionState) return;

	//�ړ��{�^����������Ă�����
	if (Pad::IsPress(PAD_INPUT_LEFT) || Pad::IsPress(PAD_INPUT_RIGHT) ||
		Pad::IsPress(PAD_INPUT_UP) || Pad::IsPress(PAD_INPUT_DOWN))
	{
		ChangeState(State::kWalk);
	}
}

void PlayerState::StateTransitionJump()
{
	// �A�N�V�������������珈����Ԃ�
	if (m_isActionState)return;

	// �W�����v�{�^����������Ă�����
	if (Pad::IsTrigger(PAD_INPUT_A))
	{
		m_isActionState = true;		// �A�N�V�������t���O��true�ɂ���
		ChangeState(State::kJump);
	}
}

/// <summary>
/// ���U��
/// </summary>
void PlayerState::StateTransitionAttackSord()
{
	// �A�N�V�������������珈����Ԃ�
	if (m_isActionState)return;

	if (Pad::IsTrigger(PAD_INPUT_X))
	{
		m_isActionState = true;
		ChangeState(State::kAttackSord);
	}	
}

/// <summary>
/// �|�U��
/// </summary>
void PlayerState::StateTransitionAttackBow()
{
	// �A�N�V�������������珈����Ԃ�
	//if (m_isActionState)return;

	if (Pad::IsPress(PAD_INPUT_B)&&Pad::IsPress(PAD_INPUT_Z))
	{
		m_isActionState = true;
		ChangeState(State::kAttackBow);
	}
}

void PlayerState::StateTransition()
{
	// �X�e�C�g�J��
	StateTransitionIdle();
	StateTransitionWalk();
	StateTransitionJump();
	StateTransitionAttackSord();
	StateTransitionAttackBow();
}

void PlayerState::ChangeState(State state)
{
	// �S�ẴX�e�C�g�𒲂ׂ�
	for (auto& ChangeState : m_pState)
	{
		// �X�e�C�g�̎�ނ��I�΂ꂽ�X�e�C�g�̎�ނƓ�����
		if (ChangeState.stateKind == state)
		{
			m_pNowState = ChangeState;
			m_pNowState.stateInit();
			return;
		}
	}
}