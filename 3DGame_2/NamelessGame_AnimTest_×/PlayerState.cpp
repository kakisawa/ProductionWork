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
	// ステイト追加用変数
	StateData state;
	state.stateUpdate = stateUpdate;
	state.stateInit = stateInit;
	state.stateKind = stateKind;

	// ステイト追加
	m_pState.push_back(state);
}

void PlayerState::SetState(StateKind state)
{
	for (auto& ChangeState : m_pState)
	{
		// ステイとの種類が選ばれたステイとの種類と同じとき
		if (ChangeState.stateKind == state)
		{
			m_pNowState = ChangeState;	// ステイト変更
			return;
		}
	}
}

void PlayerState::EndState()
{
	// アクション終了
	m_isActionState = false;
}

/// <summary>
/// アイドル状態
/// </summary>
void PlayerState::StateTransIdle()
{
	// アクション中なら処理しない
	if (m_isActionState)return;

	if (!Pad::IsPress(PAD_INPUT_RIGHT) || !Pad::IsPress(PAD_INPUT_LEFT) ||
		!Pad::IsPress(PAD_INPUT_UP) || !Pad::IsPress(PAD_INPUT_DOWN))
	{
		ChangeState(StateKind::kIdle);
	}
}

/// <summary>
/// 歩く状態
/// </summary>
void PlayerState::StateTransWalk()
{
	// アクション中なら処理しない
	if (m_isActionState)return;

	if (Pad::IsPress(PAD_INPUT_RIGHT) || Pad::IsPress(PAD_INPUT_LEFT) ||
		Pad::IsPress(PAD_INPUT_UP) || Pad::IsPress(PAD_INPUT_DOWN))
	{
		ChangeState(StateKind::kWalk);
	}
}

/// <summary>
/// ジャンプ状態
/// </summary>
void PlayerState::StateTransJump()
{
	// アクション中なら処理しない
	if (m_isActionState)return;

	if (Pad::IsPress(PAD_INPUT_A))
	{
		m_isActionState = true;
		ChangeState(StateKind::kJump);
	}
}

/// <summary>
/// 攻撃状態
/// </summary>
void PlayerState::StateTransAttack()
{
	// アクション中なら処理しない
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
