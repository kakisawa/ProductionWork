#include "SceneSelect.h"
#include "SceneTitle.h"
#include "SceneRanking.h"
#include "SceneOption.h"
#include "SceneGame.h"
#include "SceneDebug.h"

namespace {
	VECTOR kScenenSelectUI[5]{
		VGet(95.0f,110,0.0f),
		VGet(120.0f,590,0.0f),
		VGet(630.0f,590,0.0f),
		VGet(120.0f,770.0f,0.0f),
		VGet(630.0f,770.0f,0.0f)
	};

	VECTOR kExampleGraph = VGet(1000.0f, 80.0f, 0.0f);		// 例画像座標
	VECTOR kExplanatoryText = VGet(1000.0f, 720.0f, 0.0f);	// 説明文座標
}

SceneSelect::SceneSelect():
	m_sceneSelectUI1(-1),
	m_sceneSelectUI2(-1),
	m_sceneSelectUI3(-1),
	m_sceneSelectUI4(-1),
	m_sceneSelectUI5(-1),
	m_nextSceneGrapgh(-1),
	m_nextSceneintroduction(-1),
	m_nextScene(nextScene::GameScene)
{
}

SceneSelect::~SceneSelect()
{
}

void SceneSelect::Init()
{
	m_sceneSelectUI1 = LoadGraph("Data/Image/UI/01,ゲームを始める.png");
	m_sceneSelectUI2 = LoadGraph("Data/Image/UI/02,ランキング.png");
	m_sceneSelectUI3 = LoadGraph("Data/Image/UI/03,設定.png");
	m_sceneSelectUI4 = LoadGraph("Data/Image/UI/04,操作説明.png");
	m_sceneSelectUI5 = LoadGraph("Data/Image/UI/05,ゲームを 終了する.png");
	m_nextSceneGrapgh = LoadGraph("Data/Image/UI/例画像.png");
	m_nextSceneintroduction = LoadGraph("Data/Image/UI/説明文.png");

}

std::shared_ptr<SceneBase> SceneSelect::Update(Input& input)
{
	
	if (input.IsTrigger(InputInfo::OK)) {			// STARTボタン
		m_isNextSceneFlag = true;
	}


	if (m_isNextSceneFlag)						// 次のシーン
	{
		if (m_nextScene == nextScene::TitleScene) 
		{
			return std::make_shared<SceneTitle>();
		}
		if (m_nextScene == nextScene::GameScene)
		{
			return std::make_shared<SceneGame>();	// ゲームシーンへ行く
		}
		if (m_nextScene == nextScene::OptionScene)
		{
			return std::make_shared<SceneOption>();	// オプションシーンへ行く
		}
		if (m_nextScene == nextScene::RankingScene)
		{
			return std::make_shared<SceneRanking>();	// ランキングシーンへ行く
		}
		if (m_nextScene == nextScene::GameEnd)
		{
			DxLib_End();						// ゲームを終了する
		}
	}

	SwitchingScene(input);


#ifdef _DEBUG
	if (input.IsTrigger(InputInfo::DebugStart)) {			// STARTボタン

		return std::make_shared<SceneDebug>();	// ゲームシーンへ行く
	}
#endif // DEBUG

	return shared_from_this();
}

void SceneSelect::Draw()
{
	DrawString(0, 0, "SceneSelect", 0xffffff);
	DrawFormatString(0, 50, 0xffffff, "m_nextScene=%d", m_nextScene);


	DrawString(0, 70, "1 = TitleScene", 0xffffff);
	DrawString(0, 90, "2 = GameScene", 0xffffff);
	DrawString(0, 110, "3 = OptionScene", 0xffffff);
	DrawString(0, 130, "4 = RankingScene", 0xffffff);
	DrawString(0, 150, "5 = GameEnd", 0xffffff);

	DrawGraph(kScenenSelectUI[0].x, kScenenSelectUI[0].y, m_sceneSelectUI1,true);
	DrawGraph(kScenenSelectUI[1].x, kScenenSelectUI[1].y, m_sceneSelectUI2, true);
	DrawGraph(kScenenSelectUI[2].x, kScenenSelectUI[2].y, m_sceneSelectUI3, true);
	DrawGraph(kScenenSelectUI[3].x, kScenenSelectUI[3].y, m_sceneSelectUI4, true);
	DrawGraph(kScenenSelectUI[4].x, kScenenSelectUI[4].y, m_sceneSelectUI5, true);

	DrawGraph(kExampleGraph.x, kExampleGraph.y, m_nextSceneGrapgh, true);
	DrawGraph(kExplanatoryText.x, kExplanatoryText.y, m_nextSceneintroduction, true);


#ifdef _DEBUG
	

#endif // DEBUG
}

void SceneSelect::End()
{
	DeleteGraph(m_sceneSelectUI1);
	DeleteGraph(m_sceneSelectUI2);
	DeleteGraph(m_sceneSelectUI3);
	DeleteGraph(m_sceneSelectUI4);
	DeleteGraph(m_sceneSelectUI5);

	DeleteGraph(m_nextSceneGrapgh);
	DeleteGraph(m_nextSceneintroduction);
}

void SceneSelect::SwitchingScene(Input& input)
{
	// 下キーを押すと次のシーンの変更をする
	if (input.IsTrigger(InputInfo::Down))
	{
		if (m_nextScene == nextScene::TitleScene)
		{
			m_nextScene = nextScene::GameScene;
		}
		else if (m_nextScene == nextScene::GameScene)
		{
			m_nextScene = nextScene::OptionScene;
		}
		else if (m_nextScene == nextScene::OptionScene)
		{
			m_nextScene = nextScene::RankingScene;
		}
		else if (m_nextScene == nextScene::RankingScene)
		{
			m_nextScene = nextScene::GameEnd;
		}
		else if (m_nextScene == nextScene::GameEnd)
		{
			m_nextScene = nextScene::TitleScene;
		}
	}

	// 上キーを押すと次のシーンの変更をする
	if (input.IsTrigger(InputInfo::Up))
	{
		if (m_nextScene == nextScene::TitleScene)
		{
			m_nextScene = nextScene::GameEnd;
		}
		else if (m_nextScene == nextScene::GameScene)
		{
			m_nextScene = nextScene::TitleScene;
		}
		else if (m_nextScene == nextScene::GameEnd)
		{
			m_nextScene = nextScene::RankingScene;
		}
		else if (m_nextScene == nextScene::RankingScene)
		{
			m_nextScene = nextScene::OptionScene;
		}
		else if (m_nextScene == nextScene::OptionScene)
		{
			m_nextScene = nextScene::GameScene;
		}
	}
}
