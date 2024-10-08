#pragma once
#include "DxLib.h"
#include "../../Util/Collision.h"
#include <memory>
#include <array>

class Model;
class GameMap;
class Camera;
class PlayerState;
class EnemyRight;
class EnemyLeft;
class SoundManager;
class Effect;
class Player
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pMap">マップ</param>
	void Init(std::shared_ptr<GameMap> pMap);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="enemyR">右の敵</param>
	/// <param name="enemyL">左の敵</param>
	void Update(const Camera& camera,const EnemyRight& enemyR, const EnemyLeft& enemyL);
	
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void End();

	/// <summary>
	/// 距離の近い敵を探す
	/// </summary>
	/// <param name="enemyR">右の敵</param>
	/// <param name="enemyL">左の敵</param>
	void NearByEnemy(const EnemyRight& enemyR, const EnemyLeft& enemyL);

	/// <summary>
	/// 移動値設定
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="upMoveVec"></param>
	/// <param name="leftMoveVec"></param>
	void OldMoveValue(const Camera& camera, VECTOR& upMoveVec, VECTOR& leftMoveVec);

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="MoveVector">移動ベクトル</param>
	/// <param name="enemyR">右の敵</param>
	/// <param name="enemyL">左の敵</param>
	void Move(const VECTOR& MoveVector, const EnemyRight& enemyR, const EnemyLeft& enemyL);

	/// <summary>
	/// 回転処理
	/// </summary>
	void Angle();

	/// <summary>
	/// 攻撃処理
	/// </summary>
	/// <param name="enemyR">右の敵</param>
	/// <param name="enemyL">左の敵</param>
	void Attack(const EnemyRight& enemyR, const EnemyLeft& enemyL);

	/// <summary>
	/// ジャンプ処理
	/// </summary>
	void Jump();

	/// <summary>
	/// 死亡処理
	/// </summary>
	void Death();

	/// <summary>
	/// 当たり判定の更新処理
	/// </summary>
	void ColUpdate();

	// 各状態ごとの初期化
	void IdleStateInit() {};		//待機状態の初期化
	void WalkStateInit() {};		//歩き状態の初期化
	void JumpStateInit();			//ジャンプ状態の初期化
	void AttackSordStateInit();		//剣攻撃状態の初期化
	void AttackBowStateInit();		//弓攻撃状態の初期化

	// 各状態ごとの更新
	void IdleStateUpdate();			// 待機状態の更新
	void WalkStateUpdate();			// 歩き状態の更新
	void JumpStateUpdate();			// ジャンプ状態の更新
	void AttackSordStateUpdate();	// 剣攻撃状態の更新
	void AttackBowStateUpdate();	// 弓攻撃状態の更新

	int GetAddDamage() const { return m_addDamage; }					// 敵に与えるダメージ量を渡す
	float GetColRadius() const { return m_colRadius; }					// プレイヤーの当たり判定半径
	bool GetAttackRight()const { return m_isSordAttackToRightEnemy; }	// 右の敵を攻撃するかのフラグを渡す
	bool GetAttackLeft()const { return m_isSordAttackToLeftEnemy; }		// 左の敵を攻撃するかのフラグを渡す
	bool GetDeathFlag() const { return m_isDeath; }						// プレイヤーが死亡しているかのフラグを渡す

	VECTOR GetPos()const { return m_pos; }					// プレイヤーの座標渡し
	Collision GetCol() const { return m_colSphere; }		// プレイヤーの当たり判定渡し

private:

	// 攻撃の種類
	enum class AttackKind{
		kNone = -1,		// 無し
		kAttackSord = 0,// 剣攻撃
		kAttackBow = 1	// 弓攻撃
	};

	// アニメーション情報
	struct AnimationData{
		int8_t kIdle = 1;		//待機モーション
		int8_t kWalk = 3;		//歩きモーション
		int8_t kJump = 12;		//ジャンプ時モーション
		int8_t kAttack1 = 30;	//攻撃モーション1
		int8_t kAttack2 = 31;	//攻撃モーション2
		int8_t kAttack3 = 32;	//攻撃モーション3
		int8_t kAttack4 = 33;	//攻撃モーション4
		int8_t kAttackBow = 54;	//攻撃モーション(銃)
	};

	struct MapPlace{
		VECTOR leftBack;		// マップ左奥座標
		VECTOR rightFront;		// マップ右前座標
	}mp;

private:
	int m_hp;						// プレイヤーHP
	int m_addDamage;				// プレイヤーが敵に与えるダメージ量
	int m_multiAttack;				// 連続攻撃用変数
	std::array<int, 6> m_uiGraph{};	// UI用画像

	float m_angle;					// プレイヤー向き角度
	float m_jumpPower;				// Ｙ軸方向の速度
	float m_gravity;				// プレイヤーにかかる重力
	float m_colRadius;				// プレイヤーの当たり判定半径

	bool m_isWalk;					// 移動中フラグ
	bool m_isJump;					// ジャンプ中フラグ
	bool m_isDeath;					// 死亡したかフラグ
	bool m_isAttack;				// 攻撃中フラグ
	bool m_isFirstAttack;			// 最初の攻撃かフラグ
	bool m_isNextAttackFlag;		// 次の攻撃が実行されるかのフラグ
	bool m_isSordAttackDamage;		// 剣攻撃を受けてHPが変動するかのフラグ
	bool m_isBowAttackDamage;		// 剣攻撃を受けてHPが変動するかのフラグ
	bool m_isSordAttackToRightEnemy;// 剣で右の敵を攻撃しているフラグ
	bool m_isSordAttackToLeftEnemy;	// 剣で左の敵を攻撃しているフラグ
	bool m_isBowAttackToLeftEnemy;	// 弓で左の敵を攻撃しているフラグ
	bool m_isCol;					// 敵との攻撃当たり判定フラグ
	bool m_isBodyCol;				// 敵の体とプレイヤーの体との当たり判定

	VECTOR m_pos;			// 座標
	VECTOR m_move;			// 移動量
	VECTOR m_targetDir;		// プレイヤーが向くべき方向のベクトル
	VECTOR m_vecToEnemy;	// プレイヤー迄の距離
	VECTOR m_UpPos;			// カプセル上座標
	VECTOR m_attackRange;	// 攻撃範囲

	VECTOR m_RightEnemyPos;
	VECTOR m_LeftEnemyPos;

	// プレイヤーアニメーションデータ
	AnimationData m_animData;
	// 攻撃種類データ
	AttackKind m_attackKind;
	// 当たり判定
	Collision m_colSphere;

	// プレイヤーステイト
	std::shared_ptr<PlayerState> m_pState;
	// エフェクト
	std::shared_ptr<Effect> m_pEffect;
	//モデルクラス
	std::shared_ptr<Model> m_pModel;
	// サウンドマネージャークラス
	SoundManager* m_pSound;
};