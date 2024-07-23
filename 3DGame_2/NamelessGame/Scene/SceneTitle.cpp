#include "SceneTitle.h"
#include "DxLib.h"
#include "SceneGame.h"
#include "SceneOption.h"
#include "SceneRanking.h"
#include "../Pad.h"

SceneTitle::SceneTitle():
	graph(-1),
	graph2(-1),
	graph3(-1),
	m_titleGraph(-1),
	m_nextScene(nextScene::kNone)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	graph = LoadGraph("data/UI/Asset1080p.png");	// 外枠
	graph2 = LoadGraph("data/UI/Asset8_1080p.png");	// 未選択
	graph3 = LoadGraph("data/UI/Asset_1080p.png");	// 選択中

	m_titleGraph = LoadGraph("data/Title.png");

	m_isNextSceneFlag = false;
}

shared_ptr<SceneBase> SceneTitle::Update()
{
	
	// シーン遷移
	if (m_isNextSceneFlag)
	{
		if (Pad::IsTrigger(PAD_INPUT_Z)) {		// RBボタン
			if (m_nextScene == nextScene::kGameScene)
			{
				return make_shared<SceneGame>();	// ゲームシーンへ行く
			}

			if (m_nextScene == nextScene::kOptionScene)
			{
				return make_shared<SceneOption>();	// オプションシーンへ行く
			}

			if (m_nextScene == nextScene::kRankingScene)
			{
				return make_shared<SceneRanking>();	// ランキングシーンへ行く
			}
		}
		
	}

	SwitchingScene();


#ifdef _DEBUG

	if (m_nextScene == nextScene::kNone)
	{
		if (Pad::IsNotPress(PAD_INPUT_Z)) {	// RBボタン
			m_isNextSceneFlag = true;
			m_nextScene = nextScene::kGameScene;
		}
	}
	
#endif // DEBUG

	return shared_from_this();
}

void SceneTitle::Draw()
{
	DrawString(0, 0, "SceneTitle", 0xffffff);

	DrawExtendGraph(150, 20, 1350, 450, m_titleGraph, true);

	DrawString(0, 20, "Please Press Button RB", 0x00ffff);

#ifdef _DEBUG

	DrawFormatString(0, 700, 0xffffff, "m_nextScene=%d", m_nextScene);

#endif // DEBUG
}

void SceneTitle::End()
{
}

void SceneTitle::SwitchingScene()
{
	// 下キーを押すと次のシーンの変更をする
	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		if (m_nextScene == nextScene::kGameScene)
		{
			m_nextScene = nextScene::kOptionScene;
		}
		else if (m_nextScene == nextScene::kOptionScene)
		{
			m_nextScene = nextScene::kRankingScene;
		}
		else if (m_nextScene == nextScene::kRankingScene)
		{
			m_nextScene = nextScene::kGameScene;
		}
	}

	// 上キーを押すと次のシーンの変更をする
	if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		if (m_nextScene == nextScene::kGameScene)
		{
			m_nextScene = nextScene::kRankingScene;
		}
		else if (m_nextScene == nextScene::kRankingScene)
		{
			m_nextScene = nextScene::kOptionScene;
		}
		else if (m_nextScene == nextScene::kOptionScene)
		{
			m_nextScene = nextScene::kGameScene;
		}
	}
}
