#pragma once
#include "DxLib.h"

class Player
{
public:
	Player();
	~Player();

	void Init();
	void Update();
	void Draw();
	void End();

	VECTOR GetPos()const { return m_pos; }	// 座標渡し

private:

	// プレイヤーの状態
	enum class State
	{
		kIdle=1,	// 待機
		kWalk=3,	// 移動
		kAttack=30,	// 攻撃
	};
	// アニメーション種類
	enum class AnimKind
	{
		kNone = -1,	// 無し
		kIdle=0,	// 待機
		kWalk=1,	// 移動
		kAttack,	// 攻撃
	};

	// アニメーションステートの更新
	void UpdateAnimState(State state);
	// アニメーション処理
	void UpdateAnim();
	// アニメーションを再生する
	void PlayAnim(AnimKind animIndex);

	void Move();	// プレイヤーの移動処理
	bool UpdateAnim(int attachNo);	// アニメーションの進行
	// ループしたかどうかを返す
	void ChangeAnim(int animIndex);	// アニメーションの変更

private:
	int m_model;			// プレイヤーモデル

	int m_currentAnimNo;	// 現在のアニメーション
	int m_prevAnimNo;		// 変更前のアニメーション
	float m_animBlemdRate;	// アニメーション合成割合
								// 0.0f:prevが再生される
								// 1.0f:currentが再生される

	float m_angle;			// プレイヤー向き角度

	bool m_isWalk;
	bool m_isAttack;			// 攻撃中フラグ

	VECTOR m_pos;			// プレイヤー位置
	VECTOR m_move;			// 移動量
};