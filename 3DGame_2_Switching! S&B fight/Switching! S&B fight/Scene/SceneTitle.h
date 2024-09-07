#pragma once
#include "SceneBase.h"
#include <array>

class SceneTitle :
	public SceneBase
{
public:
	SceneTitle();
	virtual ~SceneTitle();

	virtual void Init();
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	void End() override;

	/// <summary>
	/// 次のシーンを選択
	/// </summary>
	void SwitchingScene();

	void DrawExplanation();

	enum nextScene
	{
		kNone,
		kGameScene,
		kOptionScene,
		kRankingScene,
		kGameEnd,
	};

private:
	int m_bgGraph;

	int m_titleGraph;
	int m_pressBgGraph;
	int m_pressEnyButtonGraph;

	bool m_isSelect;
	bool m_isBackScene;
	int m_fadeAlpha;        // フェードイン、アウト
	bool m_isFadeIn;		// フェードイン用のフラグ
	bool m_isFadeOut;		// フェードアウト用のフラグ

	std::array<int, 13> m_uiGraph{};	// UI用画像

	VECTOR m_explanationBgPos;
	VECTOR m_explanationPos;

	struct Size             // 説明用画像のサイズ
	{
		int m_width;        // 横
		int m_height;       // 縦
	};

	// スクロール移動量
	float m_scrollX_1;

	struct Cursor
	{
		VECTOR m_selectBox1;	// 四角始点
		VECTOR m_selectBox2;	// 四角終点
	}c1;

	nextScene m_nextScene;
};