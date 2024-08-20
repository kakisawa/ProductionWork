#pragma once
#include "Model.h"
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
		int8_t kIdle = 1;		//待機モーション
		int8_t kWalk = 3;		//歩きモーション
		int8_t kJump = 12;		//ジャンプ時モーション
		int8_t kAttack1 = 30;	//攻撃モーション1
		int8_t kAttack2 = 31;	//攻撃モーション2
		int8_t kAttack3 = 32;	//攻撃モーション3
		int8_t kAttack4 = 33;	//攻撃モーション4
	};

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

	// プレイヤーの移動値設定
	void OldMoveValue(const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec);
	void Move(const VECTOR& MoveVector);	// プレイヤーの移動処理
	void Angle();							// プレイヤーの回転処理

	void Attack();		// 攻撃処理
	void Jump();		// ジャンプ処理

private:
	float m_angle;				// プレイヤー向き角度
	float m_jumpPower;			// Ｙ軸方向の速度
	float m_gravity;			// プレイヤーにかかる重力

	bool m_isWalk;			// 移動中フラグ
	bool m_isAttack;		// 攻撃中フラグ
	bool m_nextAttackFlag;	// 次の攻撃が実行されるかのフラグ
	bool m_isFirstAttack;	// 最初の攻撃かフラグ
	bool m_isJump;			// ジャンプ中フラグ

	// プレイヤーステイトポインタ
	std::shared_ptr<PlayerState> m_pState;

	// プレイヤーアニメーションデータ
	AnimationData m_animData;

	int m_multiAttack;		// 連続攻撃用変数
	bool m_isNextAttack;	// 次の攻撃を行うかのフラグ

	VECTOR m_pos = m_pModel->GetPos();			// プレイヤー位置
	VECTOR m_move;			// 移動量
	VECTOR m_targetDir;		// プレイヤーが向くべき方向のベクトル

	//モデルクラス
	std::shared_ptr<Model> m_pModel;
};