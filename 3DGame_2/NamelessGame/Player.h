#pragma once
#include "Model.h"
#include "DxLib.h"
#include <memory>

class Camera;
class PlayerState;
class GameMap;
class Player
{
public:
	Player();
	~Player();

	void Init(std::shared_ptr<GameMap> pMap);
	void Update(const Camera& camera);
	void Draw();
	void End();


	int GetAddDamage() const { return m_addDamage; }
	bool GetDeathFlag() const { return m_isDeath; }
	VECTOR GetPos()const { return m_pos; }	// 座標渡し

private:

	// 攻撃の種類
	enum class AttackKind
	{
		kNone = -1,
		kAttackSord = 0,
		kAttackBow = 1
	};

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
		int8_t kAttackBow = 54;	//攻撃モーション(銃)
	};

	// 各状態ごとの初期化
	void IdleStateInit() {};		//待機状態の初期化
	void WalkStateInit() {};		//歩き状態の初期化

	void JumpStateInit();			//ジャンプ状態の初期化
	void AttackSordStateInit();		//剣攻撃状態の初期化
	void AttackBowStateInit();		// 弓攻撃状態の初期化

	// 各状態ごとの更新
	void IdleStateUpdate();
	void WalkStateUpdate();
	void JumpStateUpdate();
	void AttackSordStateUpdate();
	void AttackBowStateUpdate();

	// プレイヤーの移動値設定
	void OldMoveValue(const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec);
	void Move(const VECTOR& MoveVector);	// プレイヤーの移動処理
	void Angle();							// プレイヤーの回転処理

	void Attack();		// 攻撃処理
	void Jump();		// ジャンプ処理

	void Death();		// 死亡処理

private:
	int m_hp;					// プレイヤーHP
	int m_addDamage;			// プレイヤーが敵に与えるダメージ量
	int m_uiGraph;				// プレイヤー用UI画像

	float m_angle;				// プレイヤー向き角度
	float m_jumpPower;			// Ｙ軸方向の速度
	float m_gravity;			// プレイヤーにかかる重力

	bool m_isWalk;			// 移動中フラグ
	bool m_isAttack;		// 攻撃中フラグ
	bool m_isNextAttackFlag;	// 次の攻撃が実行されるかのフラグ
	bool m_isFirstAttack;	// 最初の攻撃かフラグ
	bool m_isAttackDamage;	// ダメージを受けてHPが変動するかのフラグ
	bool m_isJump;			// ジャンプ中フラグ
	bool m_isDeath;			// 死亡したかフラグ

	struct MapPlace
	{
		VECTOR leftBack;		// マップ左奥座標
		VECTOR rightFront;	// マップ右前座標
	}mp;

	// プレイヤーステイトポインタ
	std::shared_ptr<PlayerState> m_pState;

	// プレイヤーアニメーションデータ
	AnimationData m_animData;

	// 攻撃種類データ
	AttackKind m_attackKind;

	int m_multiAttack;		// 連続攻撃用変数
	bool m_isNextAttack;	// 次の攻撃を行うかのフラグ

	VECTOR m_pos = m_pModel->GetPos();			// プレイヤー位置
	VECTOR m_move;			// 移動量
	VECTOR m_targetDir;		// プレイヤーが向くべき方向のベクトル
	VECTOR m_UpPos;			// カプセル上座標

	//モデルクラス
	std::shared_ptr<Model> m_pModel;
};