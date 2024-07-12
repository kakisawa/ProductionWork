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
		kWalk=2,	// 移動
		kAttack=30,	// 攻撃
	};
	// アニメーション種類
	enum class AnimKind
	{
		kNone = -1,	// 無し
		kUnKnown=0,	// 不明
		kIdle=1,	// 待機
		kWalk=2,	// 移動
		kAttack1 = 30,	// 通常剣攻撃1
		kAttack2 = 31,	// 通常剣攻撃2
		kAttack3 = 32,	// 通常剣攻撃3
		kAttack4 = 33,	// 通常剣攻撃4

		kAttack5 = 58,	// 通常銃攻撃(連射のみ)
	
	};

	// 通常剣攻撃種類
	enum class AttackKind
	{
		kNormalAttack1 = 1,
		kNormalAttack2 = 2,
		kNormalAttack3 = 3,
		kNormalAttack4 = 4,

		kNormalShot = 5,

	};

	// アニメーション状態の更新
	void UpdateAnimState(State state);
	// アニメーション処理
	void UpdateAnim();
	// アニメーションを再生する
	void PlayAnim(AnimKind animIndex);

	State MoveValue();						// プレイヤーの移動値設定
	void Move(const VECTOR& MoveVector);	// プレイヤーの移動処理
	State Attack();							// プレイヤーの攻撃処理


private:
	int m_model;			// プレイヤーモデル
	float m_angle;			// プレイヤー向き角度
	
	bool m_isWalk;			// 移動中フラグ
	bool m_isAttack;		// 攻撃中フラグ

	// アニメーション情報
	int m_currentAnimNo;		// 再生中のアニメーションのアタッチ番号
	float m_currentAnimCount;	// 再生中のアニメーションの再生時間
	float m_prevAnimNo;			// 前の再生アニメーションのアタッチ番号
	float m_prevAnimCount;		// 前の再生アニメーションの再生時間
	float m_animBlendRate;		// 現在と過去のアニメーションの合成割合
									// 0.0f:prevが再生される
									// 1.0f:currentが再生される

	State m_currentState;	// 現在のプレイヤーの状態

	VECTOR m_pos;			// プレイヤー位置
	VECTOR m_move;			// 移動量
};