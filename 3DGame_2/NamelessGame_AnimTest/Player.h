#pragma once
#include "Animation.h"
#include "DxLib.h"
#include <memory>

class Camera;
class PlayerState;
class Player:public Animation
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

	// アニメーション情報
	struct AnimationData
	{
		int8_t kIdle = 1;		//待機モーション
		int8_t kWalk = 2;		//歩きモーション
		int8_t kJump = 12;		//ジャンプ時モーション
		int8_t kAttack1 = 30;	//攻撃モーション1
		int8_t kAttack2 = 31;	//攻撃モーション2
		int8_t kAttack3 = 32;	//攻撃モーション3
		int8_t kAttack4 = 33;	//攻撃モーション4
	};


	// アニメーション種類
	//enum class AnimKind
	//{
	//	kNone = -1,	// 無し
	//	kUnKnown=0,	// 不明
	//	kIdle=1,	// 待機
	//	kWalk=2,	// 移動
	//	kJump=12,	// ジャンプ
	//	kAttack1 = 30,	// 通常剣攻撃1
	//	kAttack2 = 31,	// 通常剣攻撃2
	//	kAttack3 = 32,	// 通常剣攻撃3
	//	kAttack4 = 33,	// 通常剣攻撃4

	//	kAttack5 = 58,	// 通常銃攻撃(連射のみ)
	//
	//};

	// 通常剣攻撃種類
	//enum class AttackKind
	//{
	//	kNone=-1,			// 無し
	//	kNormalAttack1 = 1,	// 通常剣攻撃1
	//	kNormalAttack2 = 2,	// 通常剣攻撃2
	//	kNormalAttack3 = 3,	// 通常剣攻撃3
	//	kNormalAttack4 = 4,	// 通常剣攻撃4

	//	kNormalShot = 5,	// 通常銃攻撃

	//};

	// 各状態ごとの初期化
	void IdleStateInit() {};			//待機状態の初期化
	void WalkStateInit() {};			//歩き状態の初期化

	void JumpStateInit();			//ジャンプ状態の初期化
	void AttackStateInit();			//攻撃状態の初期化

	// 各状態ごとの更新
	void IdleStateUpdate();
	void WalkStateUpdate();
	void JumpStateUpdate();
	void AttackStateUpdate();




	//// アニメーション状態の更新
	//void OldUpdateAnimState(State state);
	//// アニメーション処理
	//void OldUpdateAnim();
	//// アニメーションを再生する
	//void OldPlayAnim(AnimKind animIndex);

	// プレイヤーの移動値設定
	void OldMoveValue(const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec);
	void Move(const VECTOR& MoveVector);	// プレイヤーの移動処理
	void Angle();							// プレイヤーの回転処理
	
	void Attack();		// 攻撃処理
	void Jump();		// ジャンプ処理

private:
	int m_model;				// プレイヤーモデル
	float m_angle;				// プレイヤー向き角度
	float m_jumpPower;			// Ｙ軸方向の速度
	float m_gravity;			// プレイヤーにかかる重力
	
	bool m_isWalk;			// 移動中フラグ
	bool m_isAttack;		// 攻撃中フラグ
	bool m_nextAttackFlag;	// 次の攻撃が実行されるかのフラグ
	bool m_isJump;			// ジャンプ中フラグ

	// プレイヤーステイトポインタ
	std::shared_ptr<PlayerState> m_pState;

	// プレイヤーアニメーションデータ
	AnimationData m_animData;

	int m_multiAttack;		// 連続攻撃用変数
	bool m_isNextAttack;	// 次の攻撃を行うかのフラグ

	VECTOR m_pos;			// プレイヤー位置
	VECTOR m_move;			// 移動量
	VECTOR m_targetDir;		// プレイヤーが向くべき方向のベクトル
};