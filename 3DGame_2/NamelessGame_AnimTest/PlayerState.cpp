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
}

void PlayerState::SetState(StateKind state)
{
	for (auto& ChangeState : m_pState)
	{
		// �X�e�C�Ƃ̎�ނ��I�΂ꂽ�X�e�C�Ƃ̎�ނƓ����Ƃ�
		if (ChangeState.stateKind == state)
		{
			m_pNowState = ChangeState;
			return;
		}
	}
}

void PlayerState::EndState()
{
	m_isActionState = false;
}

void PlayerState::StateTransIdle()
{
	if (m_isActionState)return;

	if (!Pad::IsPress(PAD_INPUT_RIGHT) || !Pad::IsPress(PAD_INPUT_LEFT) ||
		!Pad::IsPress(PAD_INPUT_UP) || !Pad::IsPress(PAD_INPUT_DOWN))
	{
		ChangeState(StateKind::kIdle);
	}
}

void PlayerState::StateTransWalk()
{
	if (m_isActionState)return;

	if (Pad::IsPress(PAD_INPUT_RIGHT) || Pad::IsPress(PAD_INPUT_LEFT) ||
		Pad::IsPress(PAD_INPUT_UP) || Pad::IsPress(PAD_INPUT_DOWN))
	{
		ChangeState(StateKind::kWalk);
	}
}

void PlayerState::StateTransJump()
{
	if (m_isActionState)return;

	if (Pad::IsPress(PAD_INPUT_A))
	{
		m_isActionState = true;
		ChangeState(StateKind::kJump);
	}
}

void PlayerState::StateTransAttack()
{
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
