#include "PlayerState.h"
#include "Pad.h"
#include "DxLib.h"

PlayerState::PlayerState()
{
}

PlayerState::~PlayerState()
{
}

void PlayerState::Update()
{
	StateTransition();

	m_pNowState.stateUpdate();
}

void PlayerState::AddState(std::function<void(void)> stateUpdate, std::function<void(void)> stateInit, StateKind stateKind)
{
	// �X�e�C�g�ǉ��p�ϐ�
	StateData state;
	state.stateUpdate = stateUpdate;
	state.stateInit = stateInit;
	state.stateKind = stateKind;

	// �X�e�C�g�ǉ�
	m_pState.push_back(state);
}

void PlayerState::SetState(StateKind state)
{
	for (auto& ChangeState : m_pState)
	{
		// �X�e�C�Ƃ̎�ނ��I�΂ꂽ�X�e�C�Ƃ̎�ނƓ����Ƃ�
		if (ChangeState.stateKind == state)
		{
			m_pNowState = ChangeState;	// �X�e�C�g�ύX
			return;
		}
	}
}

void PlayerState::EndState()
{
	// �A�N�V�����I��
	m_isActionState = false;
}

/// <summary>
/// �A�C�h�����
/// </summary>
void PlayerState::StateTransIdle()
{
	// �A�N�V�������Ȃ珈�����Ȃ�
	if (m_isActionState)return;

	if (!Pad::IsPress(PAD_INPUT_RIGHT) || !Pad::IsPress(PAD_INPUT_LEFT) ||
		!Pad::IsPress(PAD_INPUT_UP) || !Pad::IsPress(PAD_INPUT_DOWN))
	{
		ChangeState(StateKind::kIdle);
	}
}

/// <summary>
/// �������
/// </summary>
void PlayerState::StateTransWalk()
{
	// �A�N�V�������Ȃ珈�����Ȃ�
	if (m_isActionState)return;

	if (Pad::IsPress(PAD_INPUT_RIGHT) || Pad::IsPress(PAD_INPUT_LEFT) ||
		Pad::IsPress(PAD_INPUT_UP) || Pad::IsPress(PAD_INPUT_DOWN))
	{
		ChangeState(StateKind::kWalk);
	}
}

/// <summary>
/// �W�����v���
/// </summary>
void PlayerState::StateTransJump()
{
	// �A�N�V�������Ȃ珈�����Ȃ�
	if (m_isActionState)return;

	if (Pad::IsPress(PAD_INPUT_A))
	{
		m_isActionState = true;
		ChangeState(StateKind::kJump);
	}
}

/// <summary>
/// �U�����
/// </summary>
void PlayerState::StateTransAttack()
{
	// �A�N�V�������Ȃ珈�����Ȃ�
	if (m_isActionState)return;

	if (Pad::IsPress(PAD_INPUT_X))
	{
		m_isActionState = true;
		ChangeState(StateKind::kAttack);
	}
}

void PlayerState::StateTransition()
{
	//�X�e�C�g�J��
	StateTransIdle();
	StateTransWalk();
	StateTransJump();
	StateTransAttack();
}

void PlayerState::ChangeState(StateKind state)
{
	//�S���̃X�e�C�g�𒲂ׂ�
	for (auto& changeState : m_pState)
	{
		//�X�e�C�g�̎�ނ��I�΂ꂽ�X�e�C�g�̎�ނƓ����Ƃ�
		if (changeState.stateKind == state)
		{
			m_pNowState = changeState;	//�X�e�C�g�ύX
			m_pNowState.stateInit();	//�X�e�C�g�̏���������
			return;
		}
	}
}
