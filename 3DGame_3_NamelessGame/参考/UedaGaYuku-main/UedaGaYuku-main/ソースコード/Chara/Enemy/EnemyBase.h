#pragma once
#include "CharacterBase.h"
#include <memory>

class SceneStageBase;
class Player;
class Stage;

/// <summary>
/// 敵の基底クラス
/// </summary>
class EnemyBase : public CharacterBase
{
public:
	EnemyBase();
	virtual ~EnemyBase();
	virtual void Init(std::shared_ptr<EffectManager> pEffect, VECTOR pos) = 0;
	virtual void Update(Player& player, Stage& stage, SceneStageBase& sceneStage);
	virtual void Draw();
	virtual void DrawUi() = 0; // UIの表示
	// 攻撃を受けた際の処理
	virtual void OnDamage(float damage) override;
	// プレイヤーとの当たり判定をチェックする
	void CheckHitPlayerCol(Player& player, VECTOR eCapPosTop, VECTOR eCapPosBottom, float eCapRadius);

	int GetEnemyType() const { return m_enemyType; }
	float GetHp() const { return m_hp; }				// 敵のHPを取得
	VECTOR GetPos() const { return m_pos; }				// 敵の座標を取得
	VECTOR GetDir() const { return m_eToPDirVec; }		// 敵の向きベクトルを取得

#ifdef _DEBUG
	void DebugDamage(Input& input); // 1ボタンで敵のHPを0にする
#endif


protected:
	// 状態を更新する
	CharacterBase::State UpdateState(Player& player, SceneStageBase& sceneStage, VECTOR& upMoveVec, VECTOR& leftMoveVec, VECTOR& moveVec);
	// 移動処理
	void Move(const VECTOR& moveVec, Player& player, Stage& stage);
	// パンチ処理
	CharacterBase::State Punch();
	// キック処理
	CharacterBase::State kick();
	// 回避処理
	CharacterBase::State Avoid();
	// 構え処理
	CharacterBase::State Fighting();
	// ガード処理
	CharacterBase::State Guard();
	// ガード状態を更新
	void UpdateGuard();
	// ガード状態を解除する
	CharacterBase::State OffGuard();
	// 攻撃を受けている最中の処理
	void Receive();
	// ダウン処理
	void Down();
	// 敵の角度を更新
	void UpdateAngle();

protected:
	int m_enemyType;			// 敵の種類
	int m_stopTime;				// 停止する時間
	int m_angleIntervalTime;	// 角度を更新するまでの時間
	int m_intervalTime;			// 次の状態を更新するまでの時間
	int m_guardTime;			// ガードの時間
	int m_shadowMap;			// シャドウマップ
	VECTOR m_eToPDirVec;		// 敵の位置からプレイヤー位置のベクトル
};

