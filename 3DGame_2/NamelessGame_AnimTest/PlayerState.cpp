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
		// ステイとの種類が選ばれたステイとの種類と同じとき
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
	//ステイト遷移
	StateTransIdle();
	StateTransWalk();
	StateTransJump();
	StateTransAttack();
}

void PlayerState::ChangeState(StateKind state)
{
	//全部のステイトを調べる
	for (auto& changeState : m_pState)
	{
		//ステイトの種類が選ばれたステイトの種類と同じとき
		if (changeState.stateKind == state)
		{
			m_pNowState = changeState;	//ステイト変更
			m_pNowState.stateInit();	//ステイトの初期化処理
			return;
		}
	}
}
