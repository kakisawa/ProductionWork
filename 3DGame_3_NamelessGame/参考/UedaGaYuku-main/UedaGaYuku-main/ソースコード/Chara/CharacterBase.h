#pragma once
#include <memory>
#include <vector>

class LoadData;
class UIBattle;
class EffectManager;

/// <summary>
/// キャラクターの基底クラス
/// </summary>
class CharacterBase
{
protected:
	// 当たり判定位置
	struct Collision
	{
		VECTOR armStartPos;		// 腕の当たり判定始点
		VECTOR armEndPos;		// 腕の当たり判定終点
		VECTOR legStartPos;		// 脚の当たり判定始点
		VECTOR legEndPos;		// 脚の当たり判定終点
		VECTOR bodyTopPos;		// 全身の当たり判定始点
		VECTOR bodyBottomPos;	// 全身の当たり判定終点
	};
	Collision m_col;

public:
	CharacterBase();
	~CharacterBase() {};
	virtual void Init(std::shared_ptr<EffectManager> pEffect, VECTOR pos) = 0;
	virtual void Draw() = 0;
	virtual void OnDamage(float damage);		// ダメージを受けた際の処理

	void SetIsStartProduction(bool isStartProduction) { m_isStartProduction = isStartProduction; }				// スタート演出状態をセットする
	void SetIsClearProduction(bool isClearProduction) { m_isClearProduction = isClearProduction; }				// クリア演出状態をセットする
	void SetIsGameoverProduction(bool isGameoverProduction) { m_isGameoverProduction = isGameoverProduction; }  // ゲームオーバー演出状態をセットする

	VECTOR GetPos() const { return m_pos; }			// 現在地取得
	float GetHp() const { return m_hp; }			// 現在のHPを取得
	bool GetIsGuard() const { return m_isGuard; }	// ガード中かどうか取得
	Collision GetCol() const { return m_col; }		// 当たり判定位置を取得

	// キャラクターの種類
	enum class CharaType
	{
		kPlayer,		// プレイヤー
		kEnemyTuto,		// チュートリアルの敵
		kEnemyNinja,	// 忍者
		kEnemyChef,		// シェフ
		kEnemyAbe,		// Abe
		kCharaNum,		// キャラ数
	};

	// キャラクターの状態
	enum class State
	{
		kAvoid = 0,			 // 回避
		kDown = 1,			 // 倒れる
		kFightIdle = 2,		 // 構え(停止中)
		kFightWalk = 3,		 // 構え(移動中)
		kGettingUp = 4,		 // 起き上がる
		kGrab = 5,			 // 掴み
		kGuard = 6,			 // ガード
		kKick = 7,			 // キック
		kPunch1 = 8,		 // パンチ(1コンボ)
		kPunch2 = 9,		 // パンチ(2コンボ)
		kPunch3 = 10,		 // パンチ(3コンボ)
		kReceive = 11,		 // 攻撃を受ける
		kRun = 12,			 // 移動
		kSpecialAttack = 13, // 必殺技 
		kStand = 14,		 // 待機
		kStumble = 15,		 // 掴み失敗
	};

	// アニメーション種別
	enum class AnimKind
	{
		kNone = -1,			 // なし
		kAvoid = 0,			 // 回避
		kDown = 1,			 // 倒れる
		kFightIdle = 2,		 // 構え(停止中)
		kFightWalk = 3,		 // 構え(移動中)
		kGettingUp = 4,		 // 起き上がる
		kGrab = 5,			 // 掴み
		kGuard = 6,			 // ガード
		kKick = 7,			 // キック
		kPunch1 = 8,		 // パンチ(1コンボ)
		kPunch2 = 9,		 // パンチ(2コンボ)
		kPunch3 = 10,		 // パンチ(3コンボ)
		kReceive = 11,	  	 // 攻撃を受ける
		kRun = 12,			 // 移動
		kSpecialAttack = 13, // 必殺技 
		kStand = 14,		 // 待機
		kStumble = 15,		 // 掴み失敗
	};

	// アニメーション速度
	struct AnimSpeed
	{
		float none;			// なし
		float avoid;		// 回避
		float down;			// 倒れる
		float fightIdle;	// 構え(停止中)
		float fightWalk;	// 構え(移動中)
		float gettingUp;	// 起き上がる
		float grab;			// 掴み
		float guard;		// ガード
		float kick;			// キック
		float punch1;		// パンチ(1コンボ)
		float punch2;		// パンチ(2コンボ)
		float punch3;		// パンチ(3コンボ)
		float specialAttack;// 必殺技
		float receive;		// 攻撃を受ける
		float run;			// 移動
		float stand;		// 待機
		float Stumble;		// 掴み失敗
	};
	AnimSpeed m_animSpeed;

	// キャラクターのステータス
	struct Status
	{
		float maxHp;				// 最大HP
		int punchReceptionTime;		// パンチコンボの入力受付時間
		int punchCoolTime;			// パンチのクールタイム
		int kickCoolTime;			// キックのクールタイム
		int punchTime;				// パンチ処理の時間
		int kickTime;				// キック処理の時間
		float punchPower;			// パンチの攻撃力
		float secondPunchPower;		// パンチ2コンボ目の攻撃力
		float thirdPunchPower;		// パンチ3コンボ目の攻撃力
		float kickPower;			// キックの攻撃力
		float specialAttackPower;	// 必殺技の攻撃力
		float maxMoveSpeed;			// 最大の移動速度
		float fightWalkSpeed;		// 構え時の移動速度
		float acceleration;			// 加速度
		float deceleration;			// 減速度
		float avoidDist;			// 回避の距離
		int maxAvoidCount;			// 連続で回避できる回数
		int avoidCoolTime;			// 回避できるようになるまでの時間
		float guardAnimTime;		// ガードのアニメーションを止める時間
		float backMove;				// ガード中に攻撃を受けた際に移動する距離
	};
	Status m_status;

	// 初期の当たり判定情報
	struct CollisionInfo
	{
		float bodyHeight;		// 全身の当たり判定の高さ
		float bodyRadius;		// 全身の当たり判定の半径
		float aimRadius;		// 腕の当たり判定の半径
		float legRadius;		// 脚の当たり判定の半径
		VECTOR armStartPos;		// 腕の当たり判定始点
		VECTOR armEndPos;		// 腕の当たり判定終点
		VECTOR legStartPos;		// 脚の当たり判定始点
		VECTOR legEndPos;		// 脚の当たり判定終点
		VECTOR bodyTopPos;		// 全身の当たり判定始点
		VECTOR bodyBottomPos;	// 全身の当たり判定終点
	};
	CollisionInfo m_colInfo;

	// 敵の情報
	struct EnemyInfo
	{
		float approachRange;	// プレイヤーに近づく範囲
		float attackRange;		// プレイヤーを攻撃する範囲
		int minStopTime;		// 最小の停止時間
		int maxStopTime;		// 最大の停止時間
		int maxProb;			// 最大確率%
		int punchProb;			// パンチ確率
		int kickProb;			// キック確率
		int avoidProb;			// 回避確率
		int guardProb;			// ガード確率
		int grabProb;			// 掴み確率
		int changeAngleProb;	// 角度を更新する確率
		int changeAngleFrame;	// 角度を更新するフレーム数
		int guardTime;			// ガード時間
		int stateIntervalTime;	// 状態を更新するまでの時間
	};
	EnemyInfo m_enemyInfo;

protected:
	// 当たり判定位置の更新
	void UpdateCol();
	// アニメーションステートの更新
	void UpdateAnimState(CharacterBase::State prevState);
	// アニメーションを再生する
	void PlayAnim(AnimKind playAnimIndex);
	// アニメーション処理
	void UpdateAnim();
	// 位置ログを更新する
	void UpdatePosLog();
	// 回避の場合残像を表示する
	void DrawAfterImage();
	
protected:
	std::shared_ptr<LoadData> m_pLoadData;			// キャラクター情報
	std::shared_ptr<UIBattle> m_pUIBattle;			// バトル中に表示するUI
	std::shared_ptr<EffectManager> m_pEffect;		// エフェクト

	float m_hp;					  // HP
	VECTOR m_pos;				  // 位置
	std::vector<VECTOR> m_posLog; // 過去の位置を保存する
	float m_moveSpeed;			  // 移動速度
	float m_angle;				  // 向いている方向の角度
	int m_punchCount;			  // 現在のパンチのコンボ数
	int m_punchComboTime;		  // コンボ入力受付時間
	int m_punchCoolTime;		  // パンチできない時間
	int m_kickCoolTime;			  // キックできない時間
	int m_attackTime;			  // 攻撃時間
	int m_avoidCoolTime;		  // 回避できない時間
	int m_avoidCount;			  // 回避した回数
	bool m_isAttack;			  // 攻撃中かどうか(true:攻撃中)
	bool m_isGuard;				  // ガード中かどうか(ture:ガード中)
	bool m_isMove;				  // 移動したかどうか(true:移動した)
	bool m_isFighting;			  // 構え中かどうか(true:構え中)
	bool m_isReceive;			  // 攻撃を受けている最中かどうか(true:攻撃を受けている)
	bool m_isSpecialAttack;		  // 必殺技を発動中か(true:発動中)
	bool m_isStartProduction;	  // スタート演出中か(true:演出中)
	bool m_isClearProduction;	  // クリア演出中か(true:演出中)
	bool m_isGameoverProduction;  // ゲームオーバー演出中か(true:演出中)
	State m_currentState;		  // 現在の状態
	int m_modelHandle;			  // キャラクターの3Dモデル

	// アニメーション情報
	int m_currentPlayAnim;		  // 再生中のアニメーションのアタッチ番号
	float m_currentAnimCount;	  // 再生中のアニメーションの再生時間
	int m_prevPlayAnim;			  // 前の再生アニメーションのアタッチ番号
	float m_prevAnimCount;		  // 前の再生アニメーションの再生時間
	float m_animBlendRate;		  // 現在と過去のアニメーションのブレンド率
};