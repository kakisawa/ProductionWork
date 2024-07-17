#pragma once
#include "DxLib.h"
#include<memory>
#include <stdint.h>

class Camera;
class PlayerState;
class Player
{
public:
	Player();
	~Player();

	void Init();
	void Update(const Camera& camera);
	void Draw();
	void End();

	VECTOR GetPos()const { return m_pos; }	// 座標渡し

private:

	
	// アニメーション種類
	enum class AnimKind
	{
		kNone = -1,	// 無し
		kUnKnown=0,	// 不明
		kIdle=1,	// 待機
		kWalk=2,	// 移動
		kJump=12,	// ジャンプ
		kFall=14,	// 落下
		kAttack1 = 30,	// 通常剣攻撃1
		kAttack2 = 31,	// 通常剣攻撃2
		kAttack3 = 32,	// 通常剣攻撃3
		kAttack4 = 33,	// 通常剣攻撃4

		kAttack5 = 58,	// 通常銃攻撃(連射のみ)
	
	};

	// アニメーション種類
	struct AnimKindStruct
	{
		int8_t kNone = 0;	// 無し
		int8_t kUnKnown = 0;	// 不明
		int8_t kIdle = 0;	// 待機
		int8_t kWalk = 0;	// 移動
		int8_t kJump = 0;	// ジャンプ
		int8_t kFall = 0;	// 落下
		int8_t kAttack1 = 0;	// 通常剣攻撃1
		int8_t kAttack2 = 0;	// 通常剣攻撃2
		int8_t kAttack3 = 0;	// 通常剣攻撃3
		int8_t kAttack4 = 0;	// 通常剣攻撃4

		int8_t kAttack5 = 0;	// 通常銃攻撃(連射のみ)

	};

	//アニメーション速度
	enum kStateAnimSpeed
	{
		Idle = 2,				//待機
		Walk = 2,				//歩き
		Jump = 1,				//ジャンプ
		Attack = 1,				//攻撃
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

	struct AnimData
	{
		int animNo;			// アニメーション番号
		int attachNo;		// アタッチ番号
		float totalTime;	// アニメーションの総再生時間
		float elapsedTime;	// アニメーションの経過時間
		bool isLoop;		// アニメーションがループするか
	};


	// アニメーション状態の更新
	void UpdateAnimState(State state);
	// アニメーション処理
	void UpdateAnim();
	// アニメーションを再生する
	void PlayAnim(AnimKind animIndex);


	// 書き直しアニメーション処理
	void InitAnim(AnimData& anim);
	// アニメーションの設定する
	void SetAnim(int animNo, bool loop, bool isForceChange);
	// アニメーションを変化させる
	void ChangeAnimation(int animNo, bool Loop, bool isForceChange, int changeFrame);
	// 現在のアニメーションが終了しているかどうか
	bool IsAnimEnd();
	// アニメーションの更新(改)
	void UpdateAnimation(AnimData anim, float dt);
	// アニメーションのブレンド管理
	void UpdateAnimBlendRate();

private:
	void IdleStateInit(){}
	void WalkStateInit(){}
	void JumpStateInit();
	void AttackStateInit();

	// 各状態ごとの更新
	void IdleStateUpdate();			//待機時の更新
	void WalkStateUpdate();			//歩き時の更新
	void JumpStateUpdate();			//ジャンプ時の更新
	void AttackStateUpdate();		//通常攻撃時の更新
	

	State MoveValue(const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec);						// プレイヤーの移動値設定
	void Move(const VECTOR& MoveVector);	// プレイヤーの移動処理
	void Angle();							// プレイヤーの回転処理
	State AttackState();					// プレイヤーの攻撃処理
	State JumpState();						// プレイヤーのジャンプ処理

	void Attack();		// 攻撃処理
	void Jump();		// ジャンプ処理

private:
	int m_model;				// プレイヤーモデル
	float m_angle;				// プレイヤー向き角度

	float m_currentJumpPower;	// Ｙ軸方向の速度
	float m_gravity;			// プレイヤーにかかる重力
	
	bool m_isWalk;			// 移動中フラグ
	bool m_isAttack;		// 攻撃中フラグ
	bool m_isNextAttack;
	//bool m_isForward;		// 攻撃時に少し前進するフラグ
	bool m_nextAttackFlag;	// 次の攻撃が実行されるかのフラグ
	bool m_isJump;			// ジャンプ中フラグ

	// アニメーション情報

	AnimData m_current;		// 変更後アニメーションデータ
	AnimData m_prev;		// 変更前アニメーションデータ

	int m_animChangeFrameTotal = 1;	// 切り替えにかける総フレーム数
	int m_animChangeFrame = 1;		// 現在の切り替えフレーム数

	AnimKindStruct m_animData;		

	int m_animSpeed;		


	//int m_currentAnimNo;		// 再生中のアニメーションのアタッチ番号
	//float m_currentAnimCount;	// 再生中のアニメーションの再生時間
	//int m_prevAnimNo;			// 前の再生アニメーションのアタッチ番号
	//float m_prevAnimCount;		// 前の再生アニメーションの再生時間
	float m_animBlendRate;		// 現在と過去のアニメーションの合成割合
									// 0.0f:prevが再生される
									// 1.0f:currentが再生される

	int m_multiAttack;		// 連続攻撃用変数

	State m_currentState;	// 現在のプレイヤーの状態

	VECTOR m_pos;			// プレイヤー位置
	VECTOR m_move;			// 移動量
	//VECTOR m_moveAttack;	// 攻撃時の加速値
	VECTOR m_targetDir;		// プレイヤーが向くべき方向のベクトル

	//プレイヤーステイトポインタ
	std::shared_ptr<PlayerState> m_pState;
};