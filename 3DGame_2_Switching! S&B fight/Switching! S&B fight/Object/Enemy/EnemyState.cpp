#include "EnemyState.h"
#include "DxLib.h"
#include "../../Util/Pad.h"

EnemyState::EnemyState():
	m_isActionState(false),
	m_isWalk(false)
{
}

void EnemyState::Update()
{
	StateTransition();

	m_pNowState.stateUpdate();
}

void EnemyState::AddState(std::function<void(void)> stateUpdate, std::function<void(void)> stateInit, State stateKind)
{
	// �X�e�C�g�ǉ��p�ϐ�
	StateData state;
	state.stateUpdate = stateUpdate;
	state.stateInit = stateInit;
	state.stateKind = stateKind;

	// �X�e�C�g�̒ǉ�
	m_pState.push_back(state);
}

void EnemyState::SetState(State state)
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

void EnemyState::EndState()
{
	// �A�N�V�����̏I��
	m_isActionState = false;
}

void EnemyState::StateTransitionIdle()
{
	// �A�N�V�������������珈����Ԃ�
	if (m_isActionState)return;

	if (!Pad::IsPress(PAD_INPUT_B))
	{
		ChangeState(State::kIdle);
	}
}

void EnemyState::StateTransitionWalk()
{
	// �A�N�V�������������珈����Ԃ�
	if (!m_isActionState) return;

	////�ړ��{�^����������Ă�����
	//if (Pad::IsPress(PAD_INPUT_B))
	{
		ChangeState(State::kWalk);
	}
}

void EnemyState::StateTransition()
{
	// �X�e�C�g�J��
	StateTransitionIdle();
	StateTransitionWalk();
}

void EnemyState::ChangeState(State state)
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
