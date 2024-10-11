#pragma once
#include "SceneBase.h"
#include "DxLib.h"
#include <memory>
#include <string>
#include <vector>

class GameMap;
class Camera;
class Player;
class EnemyRight;
class EnemyLeft;
class SceneGame :
    public SceneBase
{
public:
	SceneGame();
	virtual ~SceneGame();

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual shared_ptr<SceneBase> Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void End();

private:
	int m_explanationGraph;	// 操作説明画像
	int m_pauseGraph;	// ポーズ表示画像
	int m_waitCount;		// ゲームクリア条件達成後の待機時間

	struct Size {			// 操作説明画像座標
		int m_width;		// 左上
		int m_height;		// 左下
		int m_widthMax;		// 右上
		int m_heightMax;	// 右下
	}size;

	bool m_isGameClearFlag;			// ゲームクリアフラグ
	bool m_isExplanationSet;		// 操作説明画像の移動開始フラグ
	bool m_isExplanationFinishFlag;	// 操作説明画像の移動終了フラグ
	bool m_isExplanationView;		// 操作説明画像の表示切替フラグ
	bool m_isPause;					// ポーズ切り替えフラグ

	// マップ
	shared_ptr<GameMap> m_pMap = make_shared<GameMap>();
	// カメラ
	shared_ptr<Camera> m_pCamera = make_shared<Camera>();
	// プレイヤー
	shared_ptr<Player> m_pPlayer = make_shared<Player>();
	// 右の敵
	shared_ptr<EnemyRight> m_pEnemyRight = make_shared<EnemyRight>();
	// 左の敵
	shared_ptr<EnemyLeft> m_pEnemyLeft = make_shared<EnemyLeft>();
};

