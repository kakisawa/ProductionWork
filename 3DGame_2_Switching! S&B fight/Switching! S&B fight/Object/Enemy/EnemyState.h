#pragma once
#include <vector>
#include <memory>
#include <functional>

class EnemyState
{
public:
	// プレイヤーの状態
	enum class State
	{
		kIdle = 1,	// 待機
		kWalk = 2,	// 移動
		//kJump = 3,	// ジャンプ
		//kAttack = 4,// 攻撃
	};

public:
	EnemyState();		// PlayerStateのコンストラクタ
	~EnemyState() {};

	void Update();	//更新処理 

	/// <summary>
	/// ステイトの追加
	/// </summary>
	/// <param name="stateUpdate">更新関数</param>
	/// <param name="stateInit">初期化関数</param>
	/// <param name="stateKind">ステイトの種類</param>
	void AddState(std::function<void(void)> stateUpdate, std::function<void(void)> stateInit, State stateKind);

	// ステイトのセット
	void SetState(State state);

	// ステイト終了処理
	void EndState();

	// 現在のステイト取得
	State GetState() const { return m_pNowState.stateKind; }
	bool GetIsWalk() const { return m_isWalk; }

private:
	// ステイトデータ
	struct StateData {
		std::function<void(void)>stateUpdate{};	// ステイト更新処理
		std::function<void(void)>stateInit{};	// ステイト初期化処理
		State stateKind{};						// ステイトの種類
	};

private:
	// ステイトの変更
	void StateTransitionIdle();		// 待機状態に条件が合えば変更
	void StateTransitionWalk();		// 歩き状態に条件が合えば変更
	//void StateTransitionJump();		// ジャンプ状態に条件が合えば変更
	//void StateTransitionAttack();	// 攻撃状態に条件が合えば変更

	// 各状態遷移呼び出し
	void StateTransition();

	// ステイトの変更
	void ChangeState(State state);

	// 全てのステイトを格納
	std::vector<StateData>	m_pState;

	// 現在選ばれているステイト
	StateData m_pNowState;

	// アクション行動をとっているか
	bool m_isActionState;

	bool m_isWalk;
};

