#include "PlayerState.h"
#include "../../Util/Pad.h"
#include "DxLib.h"

PlayerState::PlayerState() :
	m_isActionState(false)
{
}

void PlayerState::Update()
{
	StateTransition();	// 各状態の呼び出し

	m_pNowState.stateUpdate();	// 現在のステイト更新
}

void PlayerState::AddState(std::function<void(void)> stateUpdate, std::function<void(void)> stateInit, State stateKind)
{
	// ステイト追加用変数
	StateData state;
	state.stateUpdate = stateUpdate;
	state.stateInit = stateInit;
	state.stateKind = stateKind;

	// ステイトの追加
	m_pState.push_back(state);
}

void PlayerState::SetState(State state)
{
	for (auto& ChangeState : m_pState)
	{
		// ステイトの種類が選ばれたステイトの種類と同じ時
		if (ChangeState.stateKind == state)
		{
			m_pNowState = ChangeState;	// ステイトの変更
			return;
		}
	}
}

void PlayerState::EndState()
{
	// アクションの終了
	m_isActionState = false;
}

void PlayerState::StateTransitionIdle()
{
	// アクション中だったら処理を返す
	if (m_isActionState)return;

	// 移動ボタンが押されていなかったら
	if (!Pad::IsPress(PAD_INPUT_RIGHT) && !Pad::IsPress(PAD_INPUT_LEFT) &&
		!Pad::IsPress(PAD_INPUT_UP) && !Pad::IsPress(PAD_INPUT_DOWN))
	{
		ChangeState(State::kIdle);
	}
}

void PlayerState::StateTransitionWalk()
{
	// アクション中だったら処理を返す
	if (m_isActionState) return;

	//移動ボタンが押されていたら
	if (Pad::IsPress(PAD_INPUT_LEFT) || Pad::IsPress(PAD_INPUT_RIGHT) ||
		Pad::IsPress(PAD_INPUT_UP) || Pad::IsPress(PAD_INPUT_DOWN))
	{
		ChangeState(State::kWalk);
	}
}

void PlayerState::StateTransitionJump()
{
	// アクション中だったら処理を返す
	if (m_isActionState)return;

	// ジャンプボタンが押されていたら
	if (Pad::IsTrigger(PAD_INPUT_A))
	{
		m_isActionState = true;		// アクション中フラグをtrueにする
		ChangeState(State::kJump);
	}
}

/// <summary>
/// 剣攻撃
/// </summary>
void PlayerState::StateTransitionAttackSord()
{
	// アクション中だったら処理を返す
	if (m_isActionState)return;

	if (Pad::IsTrigger(PAD_INPUT_X))
	{
		m_isActionState = true;
		ChangeState(State::kAttackSord);
	}	
}

/// <summary>
/// 弓攻撃
/// </summary>
void PlayerState::StateTransitionAttackBow()
{
	// アクション中だったら処理を返す
	//if (m_isActionState)return;

	if (Pad::IsPress(PAD_INPUT_B)&&Pad::IsPress(PAD_INPUT_Z))
	{
		m_isActionState = true;
		ChangeState(State::kAttackBow);
	}
}

void PlayerState::StateTransition()
{
	// ステイト遷移
	StateTransitionIdle();
	StateTransitionWalk();
	StateTransitionJump();
	StateTransitionAttackSord();
	StateTransitionAttackBow();
}

void PlayerState::ChangeState(State state)
{
	// 全てのステイトを調べる
	for (auto& ChangeState : m_pState)
	{
		// ステイトの種類が選ばれたステイトの種類と同じ時
		if (ChangeState.stateKind == state)
		{
			m_pNowState = ChangeState;
			m_pNowState.stateInit();
			return;
		}
	}
}
