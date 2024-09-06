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

	enum class nextScene
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
	int m_gameStart;
	int m_option;
	int m_ranking;
	int m_gameFinish;


	std::array<int, 7> m_uiGraph{};	// UI用画像

	nextScene m_nextScene;
};