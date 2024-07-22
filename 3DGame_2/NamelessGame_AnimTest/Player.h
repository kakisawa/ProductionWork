#pragma once
#include "DxLib.h"
#include <memory>

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

	// アニメーション情報
	struct AnimationData
	{
		int8_t kIdle = 0;		//待機モーション
		int8_t kWalk = 0;		//歩きモーション
		int8_t kJump = 0;		//ジャンプ時モーション
		int8_t kAttack1 = 0;	//攻撃モーション1
		int8_t kAttack2 = 0;	//攻撃モーション2
		int8_t kAttack3 = 0;	//攻撃モーション3
		int8_t kAttack4 = 0;	//攻撃モーション4
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

	struct AnimData			// アニメーションデータ
	{
		int animNo;			// アニメーション番号
		int attachNo;		// アタッチ番号
		float totalTime;	// アニメーションの総再生時間
		float elapsedTime;	// アニメーションの経過時間
		bool isLoop;		// アニメーションがループするか
	};

	// 〇新規作成_アニメーション
	// アニメーション状態の初期化
	void InitAnim(AnimData& anim);
	/// <summary>
	/// アニメーション変更
	/// </summary>
	/// <param name="animNo">変更先アニメーション番号</param>
	/// <param name="isLoop">ループ再生させるか</param>
	/// <param name="isForceChange">既に指定されたアニメーションが再生されている場合も再生するか</param>
	/// <param name="isChangeFrame">何フレームかけて切り替える</param>
	void ChangeAnim(int animNo, bool isLoop, bool isForceChange, bool isChangeFrame);
	// 現在のアニメーション切り替わり情報からアニメーションのブレンド率を設定する
	void UpdateAnimBlendRate();
	// アニメーションの更新
	void UpdateAnimation(AnimData anim,float dt=1.0f);

	// 各状態ごとの更新
	void IdleStateUpdate();
	void WalkStateUpdate();
	void JumpStateUpdate();
	void AttackStateUpdate();




	// アニメーション状態の更新
	void OldUpdateAnimState(State state);
	// アニメーション処理
	void OldUpdateAnim();
	// アニメーションを再生する
	void OldPlayAnim(AnimKind animIndex);

	// プレイヤーの移動値設定
	void OldMoveValue(const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec);
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
	//bool m_isForward;		// 攻撃時に少し前進するフラグ
	bool m_nextAttackFlag;	// 次の攻撃が実行されるかのフラグ
	bool m_isJump;			// ジャンプ中フラグ


	// アニメーション情報
	AnimData m_current;		// 変更後アニメーションデータ
	AnimData m_prev;		// 変更前アニメーションデータ
	float m_animBlendRate;		// 現在と過去のアニメーションの合成割合	(フレーム)
									// 0.0f:prevが再生される
									// 1.0f:currentが再生される

	float m_animSpeed;		// アニメーション変更速度

	// プレイヤーステイトポインタ
	std::shared_ptr<PlayerState> m_pState;

	// アニメーションデータ
	AnimationData m_animData;



	int m_multiAttack;		// 連続攻撃用変数
	bool m_isNextAttack;	// 次の攻撃を行うかのフラグ
	AttackKind m_attackKind;// 通常攻撃の段階

	State m_currentState;	// 現在のプレイヤーの状態

	VECTOR m_pos;			// プレイヤー位置
	VECTOR m_move;			// 移動量
	//VECTOR m_moveAttack;	// 攻撃時の加速値
	VECTOR m_targetDir;		// プレイヤーが向くべき方向のベクトル
};