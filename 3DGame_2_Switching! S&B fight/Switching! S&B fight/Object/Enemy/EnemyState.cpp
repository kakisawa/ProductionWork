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
	// ステイト追加用変数
	StateData state;
	state.stateUpdate = stateUpdate;
	state.stateInit = stateInit;
	state.stateKind = stateKind;

	// ステイトの追加
	m_pState.push_back(state);
}

void EnemyState::SetState(State state)
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

void EnemyState::EndState()
{
	// アクションの終了
	m_isActionState = false;
}

void EnemyState::StateTransitionIdle()
{
	// アクション中だったら処理を返す
	if (m_isActionState)return;

	if (!Pad::IsPress(PAD_INPUT_B))
	{
		ChangeState(State::kIdle);
	}
}

void EnemyState::StateTransitionWalk()
{
	// アクション中だったら処理を返す
	if (!m_isActionState) return;

	////移動ボタンが押されていたら
	//if (Pad::IsPress(PAD_INPUT_B))
	{
		ChangeState(State::kWalk);
	}
}

void EnemyState::StateTransition()
{
	// ステイト遷移
	StateTransitionIdle();
	StateTransitionWalk();
}

void EnemyState::ChangeState(State state)
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
