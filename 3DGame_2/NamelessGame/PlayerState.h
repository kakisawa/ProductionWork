#pragma once
#include <vector>
#include <memory>
#include <functional>

class PlayerState
{
public:
	// プレイヤーの状態
	enum class State
	{
		kIdle = 1,	// 待機
		kWalk = 2,	// 移動
		kJump = 3,	// ジャンプ
		kAttackSord = 4,// 剣攻撃
		kAttackBow=5,	// 弓攻撃
	};

public:
	PlayerState();		// コンストラクタ
	~PlayerState() {};	// デストラクタ

	void Update();	//更新

	/// <summary>
	/// ステイトの追加
	/// </summary>
	/// <param name="stateUpdate">更新関数</param>
	/// <param name="stateInit">初期化関数</param>
	/// <param name="stateKind">ステイトの種類</param>
	void AddState(std::function<void(void)> stateUpdate, std::function<void(void)> stateInit, State stateKind);

	/// <summary>
	/// ステイトのセット
	/// </summary>
	/// <param name="state"></param>
	void SetState(State state);

	/// <summary>
	/// ステイトの終了
	/// </summary>
	void EndState();

	/// <summary>
	/// 現在のステイト取得
	/// </summary>
	/// <returns></returns>
	State GetState() const { return m_pNowState.stateKind; }

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
	void StateTransitionJump();		// ジャンプ状態に条件が合えば変更
	void StateTransitionAttackSord();	// 剣攻撃状態に条件が合えば変更
	void StateTransitionAttackBow();	// 弓攻撃状態に条件が合えば変更

	// 各状態遷移呼び出し
	void StateTransition();

	// ステイトの変更
	void ChangeState(State state);

	// 全てのステイトを格納
	std::vector<StateData>	m_pState;

	// 現在選ばれているステイト
	StateData m_pNowState;

private:
	bool m_isActionState;	// アクション行動をとっているか
};

