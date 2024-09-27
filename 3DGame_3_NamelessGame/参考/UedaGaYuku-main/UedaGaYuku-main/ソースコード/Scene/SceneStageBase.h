#pragma once
#include "SceneBase.h"
#include <vector>

class Player;
class Camera;
class Stage;
class EnemyBase;
class UIBattle;
class EffectManager;
class Light;

/// <summary>
/// プレイシーンの基底クラス
/// </summary>
class SceneStageBase : public SceneBase
{
public:
	SceneStageBase();
	SceneStageBase(std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera, std::shared_ptr<Stage> pStage);
	virtual ~SceneStageBase();
	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update(Input& input) = 0;
	virtual void Draw();
	virtual void ClearProduction();	// クリア演出を行う
	// 試合が始まるまでの時間を取得
	int GetBattleStartTime() const { return m_nextBattleTime; }
	// ポーズ状態かどうか取得
	bool IsGetPause() const {return m_isPause; }

protected:
	void UpdateNextBattle();	// 敵を倒して次試合が始まる前の処理
	void GameoverProduction();	// ゲームオーバー演出を行う

protected:
	std::shared_ptr<Player> m_pPlayer;			// プレイヤー
	std::shared_ptr<Camera> m_pCamera;			// カメラ
	std::shared_ptr<Stage> m_pStage;			// ステージ
	std::shared_ptr<EnemyBase> m_pEnemy;		// 敵
	std::shared_ptr<UIBattle> m_pUIBattle;		// バトルUI
	std::shared_ptr<EffectManager> m_pEffect;	// エフェクト
	int m_battleNum;							// 現在のバトル数
	int m_clearProductionTime;					// クリア演出の時間
	int m_gameoverProductionTime;				// ゲームオーバー演出の時間
	int m_nextBattleTime;						// 次の試合が始まるまでの時間
	int m_elapsedTime;							// 経過時間
	std::vector<int> m_clearTime;				// 各試合ごとのクリアタイム
	bool m_isPause;								// ポーズ状態かどうか(true:ポーズ状態)
	int m_shadowMap;							// シャドウマップ
	int m_clearBackHandle;						// クリア時の背景画像
};

