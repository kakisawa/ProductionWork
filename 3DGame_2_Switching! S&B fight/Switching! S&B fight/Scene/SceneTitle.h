#pragma once
#include "SceneBase.h"
#include <array>

class SceneTitle :
	public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneTitle();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneTitle();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新
	/// </summary>
	virtual shared_ptr<SceneBase> Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 終了
	/// </summary>
	void End() override;

	/// <summary>
	/// 次のシーンを選択
	/// </summary>
	void SwitchingScene();

	/// <summary>
	/// テロップ描画
	/// </summary>
	void DrawCaption();

private:
	int m_bgGraph;				// 背景
	int m_titleGraph;			// タイトルロゴ
	std::array<int, 13> m_uiGraph{};	// UI用画像

	float m_scrollX;			// スクロール移動量
	bool m_isMenu;				// メニュー表示フラグ

	struct Size             // テロップ画像のサイズ
	{
		int m_width;        // 横
		int m_height;       // 縦
	};

	enum nextScene			// 次のシーン
	{
		kNone,				// 無し
		kGameScene,			// ゲームシーン
		kOptionScene,		// オプションシーン
		kRankingScene,		// ランキングシーン
		kGameEnd,			// ゲーム終了
	};

	struct Cursor			// 選択中のセレクト
	{
		VECTOR m_selectBox1;	// 四角始点
		VECTOR m_selectBox2;	// 四角終点
	}c1;

	nextScene m_nextScene;	// 次のシーン
};