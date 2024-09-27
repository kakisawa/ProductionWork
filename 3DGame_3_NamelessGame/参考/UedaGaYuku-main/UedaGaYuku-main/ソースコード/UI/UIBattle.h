#pragma once
#include "UI.h"

/// <summary>
/// バトル中に表示するUIを管理する
/// </summary>
class UIBattle : public UI
{
public:
	UIBattle() = delete;
	UIBattle(float maxHp, int charType);
	virtual ~UIBattle();
	void UpdateHpBar();												 // HP表示を更新する
	void SetDamageTimer();											 // ダメージを受けた際にタイマーをセットする
	void OnDamage(float damage);									 // 受けたダメージ量を計算する
	void ResetStartProduction();									 // スタート演出をリセットする
	void DrawStartProduction(int time, int matchNum, int maxMatch);  // スタート演出を表示
	void ResetClearProduction();									 // クリア演出をリセットする
	void DrawClearProduction(int time);								 // クリア演出を表示
	void DrawGameoverProduciton();									 // ゲームオーバー演出を表示
	void DrawPlayerName();											 // プレイヤーの名前を表示
	void DrawPlayerHP(float currentHp);								 // プレイヤーのHPバーを表示
	void DrawPlayerGauge(float currentGauge, float MaxGauge);		 // プレイヤーのゲージバーを表示
	void DrawEnemyName(int charType);								 // 敵の名前を表示
	void DrawEnemyHp(float currentHp);								 // 敵のHPバーを表示
	void DrawPlayerSilhouette();									 // シルエットを表示
	void DrawEnemySilhouette(int charType);							 // シルエットを表示
	void ResetSpecialAttack();										 // 必殺技の表示をリセットする
	void DrawSpecialAttack(float currentGauge, float maxGauge);		 // 必殺技のテキスト表示
	void DrawOperation();											 // 操作説明を表示
	void DrawTutoButtonText();										 // チュートリアル画面のボタン表示

	void SetEnemyKind(int enmeyKind){m_currentEnemy = enmeyKind; }   // 現在の敵をセットする

private:
	float m_decreaseHp;				// ダメージを受けた後のHP
	float m_currentHp;				// 現在のHP
	float m_maxHp;					// 最大HP
	float m_damage;					// 受けたダメージ量
	int m_intervalTime;				// HPバーが減少するまでの時間
	int m_currentEnemy;				// 現在の敵
	float m_enemyNameScale;			// 敵名前の拡大率
	float m_gekihaTextScale;		// "撃破"テキストの拡大率
	float m_specialTextScale;		// "必殺技"テキストの拡大率
	std::vector<int> m_handle;		// ハンドル

	// 画像の種類
	enum HandleKind
	{
		kTutoName = 1,	 // チュートリアル敵の名前
		kNinjaName = 2,	 // 忍者の名前
		kCiefName = 3,	 // シェフの名前
		kOldManName = 4, // 老人の名前
		kNameBack,	     // 名前バー
		kGaugeBar,		 // ゲージバー
		kSilhouette,	 // キャラクターのシルエット
		kFightText,		 // "Fight!"のテキスト
		kGekihaText,	 // "撃破"のテキスト
		kClearBg,		 // クリア時の背景
		kGameoverBg,	 // ゲームオーバー時の背景
		kNumText,		 // 数字
		kHandleNum		 // ハンドルの種類
	};

	// 操作説明で表示する順番
	enum OperationOrder
	{
		kMove,			// 移動
		kCameraMove,	// カメラ移動
		kPunch,			// パンチ
		kKick,			// キック
		kSpecialAttack, // 必殺技
		kAvoid,			// 回避
		kGuard,			// ガード
		kRockon,		// ロックオン
		kPause,			// ポーズ
	};
};
