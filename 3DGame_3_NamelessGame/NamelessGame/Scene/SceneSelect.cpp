#include "SceneSelect.h"
#include "SceneTitle.h"
#include "SceneRanking.h"
#include "SceneOption.h"
#include "SceneGame.h"
#include "SceneDebug.h"

SceneSelect::SceneSelect():
	m_nextScene(nextScene::GameScene)
{
}

SceneSelect::~SceneSelect()
{
}

void SceneSelect::Init()
{
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
#ifdef _DEBUG
	

#endif // DEBUG
}

void SceneSelect::End()
{
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
