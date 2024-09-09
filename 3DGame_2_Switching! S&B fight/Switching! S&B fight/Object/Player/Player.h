#pragma once
#include "../Model.h"
#include "DxLib.h"
#include "../../Util/Collision.h"
#include <memory>
#include <array>

class Camera;
class EnemyRight;
class EnemyLeft;
class PlayerState;
class GameMap;
class SoundManager;
class Effect;
class Player
{
public:
	Player();
	~Player();

	void Init(std::shared_ptr<GameMap> pMap);
	void Update(const Camera& camera,const EnemyRight& enemyR, const EnemyLeft& enemyL);
	void Draw();
	void End();

	void NearByEnemy(const EnemyRight& enemyR, const EnemyLeft& enemyL);

	// プレイヤーの移動値設定
	void OldMoveValue(const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec);
	void Move(const VECTOR& MoveVector, const EnemyRight& enemyR, const EnemyLeft& enemyL);	// プレイヤーの移動処理
	void Angle();							// プレイヤーの回転処理

	void Attack(const EnemyRight& enemyR, const EnemyLeft& enemyL);		// 攻撃処理
	void Jump();		// ジャンプ処理

	void Death();		// 死亡処理


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

	int GetAddDamage() const { return m_addDamage; }
	bool GetAttackRight()const { return m_isAttackRight; }
	bool GetAttackLeft()const { return m_isAttackLeft; }
	bool GetDeathFlag() const { return m_isDeath; }

	VECTOR GetPos()const { return m_pos; }	// 座標渡し
	Collision GetCol() const { return m_colSphere; }

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

	struct MapPlace
	{
		VECTOR leftBack;		// マップ左奥座標
		VECTOR rightFront;	// マップ右前座標
	}mp;

private:
	int m_hp;					// プレイヤーHP
	int m_addDamage;			// プレイヤーが敵に与えるダメージ量
	int m_multiAttack;		// 連続攻撃用変数
	std::array<int, 5> m_uiGraph{};	// UI用画像

	float m_angle;				// プレイヤー向き角度
	float m_jumpPower;			// Ｙ軸方向の速度
	float m_gravity;			// プレイヤーにかかる重力

	bool m_isWalk;			// 移動中フラグ
	bool m_isAttack;		// 攻撃中フラグ
	bool m_isNextAttackFlag;// 次の攻撃が実行されるかのフラグ
	bool m_isFirstAttack;	// 最初の攻撃かフラグ
	bool m_isSordAttackDamage;	// 剣攻撃を受けてHPが変動するかのフラグ
	bool m_isBowAttackDamage;	// 剣攻撃を受けてHPが変動するかのフラグ
	bool m_isJump;			// ジャンプ中フラグ
	bool m_isDeath;			// 死亡したかフラグ
	bool m_isAttackCancel;	// 攻撃をキャンセルしたかフラグ
	bool m_isAttackRight;	// 右の敵を攻撃しているフラグ
	bool m_isAttackLeft;	// 左の敵を攻撃しているフラグ
	bool m_isNextAttack;	// 次の攻撃を行うかのフラグ
	bool m_isLeftEnemyAttack;

	VECTOR m_pos;			// プレイヤー位置
	VECTOR m_move;			// 移動量
	VECTOR m_targetDir;		// プレイヤーが向くべき方向のベクトル
	VECTOR m_vecToEnemy;	// プレイヤー迄の距離
	VECTOR m_UpPos;			// カプセル上座標
	VECTOR m_attackRange;	// 攻撃範囲

	// プレイヤーアニメーションデータ
	AnimationData m_animData;
	// 攻撃種類データ
	AttackKind m_attackKind;
	// 当たり判定
	Collision m_colSphere;


	std::shared_ptr<PlayerState> m_pState;	// プレイヤーステイト

	std::shared_ptr<Effect> m_pEffect;		// エフェクト


	

	

	


	//モデルクラス
	std::shared_ptr<Model> m_pModel;

	SoundManager* m_pSound;

	bool isCol;	// 当たり判定仮
};