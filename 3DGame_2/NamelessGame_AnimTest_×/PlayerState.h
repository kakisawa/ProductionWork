#pragma once
#include <vector>
#include <functional>

class PlayerState
{
public:
	// プレイヤーの状態
	enum class StateKind
	{
		kIdle = 1,	// 待機
		kWalk = 2,	// 移動
		kJump = 3,	// ジャンプ
		kAttack = 4,	// 攻撃
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerState();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerState();

	void Update();

	/// <summary>
	/// ステイト追加
	/// </summary>
	/// <param name="stateUpdate"></param>
	/// <param name="stateInit"></param>
	/// <param name="stateKind"></param>
	void AddState(std::function<void(void)> stateUpdate, std::function<void(void)> stateInit, StateKind stateKind);

	//ステイトセット
	void SetState(StateKind state);

	//ステイト終了処理
	void EndState();

	//現在のステイト取得
	StateKind GetState() const { return m_pNowState.stateKind; }

private:
	struct StateData
	{
		std::function<void(void)> stateUpdate{};	//ステイト更新処理
		std::function<void(void)> stateInit{};	//ステイト初期化処理
		StateKind stateKind{};					//ステイトの種類
	};

private:
	void StateTransIdle();			//待機状態に条件が合えば変更
	void StateTransWalk();			//歩き状態に条件が合えば変更
	void StateTransJump();			//ジャンプ状態に条件が合えば変更
	void StateTransAttack();		//攻撃状態に条件が合えば変更

	void StateTransition();				//各状態遷移呼び出し

	void ChangeState(StateKind state);	//ステイト変更

	std::vector<StateData> m_pState;	//全部のステイトを格納

	StateData m_pNowState;				//現在選ばれているステイト

	bool m_isActionState = false;		//アクション行動を行っているか
};