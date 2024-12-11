﻿#include "SceneSelect.h"
#include "SceneTitle.h"
#include "SceneRanking.h"
#include "SceneOption.h"
#include "SceneGame.h"
#include "SceneExplanation.h"
#include "SceneDebug.h"
#include "../Fade.h"

using namespace MyInputInfo;

namespace {
	VECTOR kScenenSelectUI[5]{
		VGet(90.0f,110.0f,0.0f),		// ゲームを始める
		VGet(110.0f,590,0.0f),			// ランキング
		VGet(620.0f,590,0.0f),			// 設定
		VGet(110.0f,770.0f,0.0f),		// 操作説明
		VGet(620.0f,770.0f,0.0f)		// ゲームを終了する
	};

	VECTOR kExampleGraph = VGet(975.0f, 75.0f, 0.0f);		// 例画像座標
	VECTOR kExplanatoryText = VGet(975.0f, 720.0f, 0.0f);	// 説明文座標

	VECTOR kCursorUI[5]{
		VGet(kScenenSelectUI[0].x - 2,kScenenSelectUI[0].y - 2,0.0f),	// ゲームを始める
		VGet(kScenenSelectUI[1].x - 2,kScenenSelectUI[1].y - 2,0.0f),	// ランキング
		VGet(kScenenSelectUI[2].x - 2,kScenenSelectUI[2].y - 2,0.0f),	// 設定
		VGet(kScenenSelectUI[3].x - 2,kScenenSelectUI[3].y - 2,0.0f),	// 操作説明
		VGet(kScenenSelectUI[4].x - 2,kScenenSelectUI[4].y - 2,0.0f),	// ゲームを終了する
	};

	const char* const kSelectUI[5]{
		"Data/Image/SceneSelect/ゲームを始める.png",
		"Data/Image/SceneSelect/ランキング.png",
		"Data/Image/SceneSelect/設定.png",
		"Data/Image/SceneSelect/操作説明.png",
		"Data/Image/SceneSelect/ゲームを終了する.png",
	};
}

SceneSelect::SceneSelect():
	m_selectGraphX(0),
	m_selectGraphY(0),
	m_sceneSelectGraph(-1),
	m_nextSceneGrapgh(-1),
	m_nextSceneintroduction(-1),
	m_cursorUI(-1),
	m_nextScene(nextScene::GameScene)
{
}

SceneSelect::~SceneSelect()
{
}

void SceneSelect::Init()
{
	for (int i = 0; i < 5; i++)
	{
		m_sceneSelectUI[i]= LoadGraph(kSelectUI[i]);
	}
	m_nextSceneGrapgh = LoadGraph("Data/Image/SceneSelect/例画像.png");
	m_nextSceneintroduction = LoadGraph("Data/Image/SceneSelect/説明文.png");
	m_cursorUI = LoadGraph("Data/Image/SceneSelect/カーソル.png");


	m_sceneSelectGraph = m_sceneSelectUI[0];
	GetGraphSize(m_sceneSelectGraph, &m_selectGraphX, &m_selectGraphY);
	c1.m_selectBox1 = kCursorUI[0];
	c1.m_selectBox2 = VGet(kCursorUI[0].x + m_selectGraphX, kCursorUI[0].y + m_selectGraphY, 0.0f);

	m_pSound->InitSound();
	m_pSound->LoadBGM(SoundManager::BGM_Type::kSelectBGM);
	m_pSound->LoadSE(SoundManager::SE_Type::kButtonSE);
	m_pSound->LoadSE(SoundManager::SE_Type::kSelectSE);
	m_pSound->PlayBGM(SoundManager::BGM_Type::kSelectBGM, DX_PLAYTYPE_LOOP);
}

std::shared_ptr<SceneBase> SceneSelect::Update(Input& input)
{
	m_pFade->FadeIn(m_pFade->GatFadeInFlag());
	
	if (input.IsTrigger(InputInfo::OK)) {			// STARTボタン
		m_pSound->PlaySE(SoundManager::SE_Type::kButtonSE, DX_PLAYTYPE_BACK);
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
		if (m_nextScene == nextScene::ExplanationScene)
		{
			return std::make_shared<SceneExplanation>();	// 操作説明シーンへ行く
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
	//if (input.IsTrigger(InputInfo::DebugStart)) {			// STARTボタン

	//	return std::make_shared<SceneDebug>();	// ゲームシーンへ行く
	//}
#endif // DEBUG

	return shared_from_this();
}

void SceneSelect::Draw()
{
	for (int i = 0; i < 5; i++)
	{
		DrawGraphF(kScenenSelectUI[i].x, kScenenSelectUI[i].y, m_sceneSelectUI[i], true);
	}

	DrawGraphF(kExampleGraph.x, kExampleGraph.y, m_nextSceneGrapgh, true);
	DrawGraphF(kExplanatoryText.x, kExplanatoryText.y, m_nextSceneintroduction, true);
	DrawExtendGraphF(c1.m_selectBox1.x, c1.m_selectBox1.y, c1.m_selectBox2.x, c1.m_selectBox2.y, m_cursorUI, true);

	m_pFade->Draw();

#ifdef _DEBUG
	/*DrawString(0, 0, "SceneSelect", 0xffffff);
	DrawFormatString(0, 50, 0xffffff, "m_nextScene=%d", m_nextScene);
	DrawString(0, 70, "1 = TitleScene", 0xffffff);
	DrawString(0, 90, "2 = GameScene", 0xffffff);
	DrawString(0, 110, "3 = OptionScene", 0xffffff);
	DrawString(0, 130, "4 = RankingScene", 0xffffff);
	DrawString(0, 150, "5 = GameEnd", 0xffffff);*/
#endif // DEBUG
}

void SceneSelect::End()
{
	m_pSound->ReleaseSound();

	DeleteGraph(m_nextSceneGrapgh);
	DeleteGraph(m_nextSceneintroduction);

	DeleteGraph(m_cursorUI);
}

void SceneSelect::SwitchingScene(Input& input)
{
	// 下キーを押すと次のシーンの変更をする
	if (input.IsTrigger(InputInfo::Down))
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kSelectSE, DX_PLAYTYPE_BACK);

		// 左側
		if (m_nextScene == nextScene::GameScene)
		{
			m_nextScene = nextScene::RankingScene;
			m_sceneSelectGraph = m_sceneSelectUI[1];
			ChangeCursorInfo(1);
		}
		else if (m_nextScene == nextScene::RankingScene)
		{
			m_nextScene = nextScene::ExplanationScene;
			m_sceneSelectGraph = m_sceneSelectUI[3];
			ChangeCursorInfo(3);
		}
		else if (m_nextScene == nextScene::ExplanationScene)
		{
			m_nextScene = nextScene::GameScene;
			m_sceneSelectGraph = m_sceneSelectUI[0];
			ChangeCursorInfo(0);
		}

		// 右側
		if (m_nextScene == nextScene::OptionScene)
		{
			m_nextScene = nextScene::GameEnd;
			m_sceneSelectGraph = m_sceneSelectUI[4];
			ChangeCursorInfo(4);
		}
		else if (m_nextScene == nextScene::GameEnd)
		{
			m_nextScene = nextScene::GameScene;
			m_sceneSelectGraph = m_sceneSelectUI[0];
			ChangeCursorInfo(0);
		}
	}

	// 上キーを押すと次のシーンの変更をする
	if (input.IsTrigger(InputInfo::Up))
	{	
		m_pSound->PlaySE(SoundManager::SE_Type::kSelectSE, DX_PLAYTYPE_BACK);
		// 左側
		if (m_nextScene == nextScene::GameScene)
		{
			m_nextScene = nextScene::ExplanationScene;
			m_sceneSelectGraph = m_sceneSelectUI[3];
			ChangeCursorInfo(3);
		}
		else if (m_nextScene == nextScene::ExplanationScene)
		{
			m_nextScene = nextScene::RankingScene;
			m_sceneSelectGraph = m_sceneSelectUI[1];
			ChangeCursorInfo(1);
		}
		else if (m_nextScene == nextScene::RankingScene)
		{
			m_nextScene = nextScene::GameScene;
			m_sceneSelectGraph = m_sceneSelectUI[0];
			ChangeCursorInfo(0);
		}

		// 右側
		if (m_nextScene == nextScene::GameEnd)
		{
			m_nextScene = nextScene::OptionScene;
			m_sceneSelectGraph = m_sceneSelectUI[2];
			ChangeCursorInfo(2);
		}
		else if (m_nextScene == nextScene::OptionScene)
		{
			m_nextScene = nextScene::GameScene;
			m_sceneSelectGraph = m_sceneSelectUI[0];
			ChangeCursorInfo(0);
		}
	}

	// 右キー・左キーを押すと次のシーンの変更をする
	if (input.IsTrigger(InputInfo::Right)|| input.IsTrigger(InputInfo::Left))
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kSelectSE, DX_PLAYTYPE_BACK);
		if (m_nextScene == nextScene::RankingScene)
		{
			m_nextScene = nextScene::OptionScene;
			m_sceneSelectGraph = m_sceneSelectUI[2];
			ChangeCursorInfo(2);
		}
		else if (m_nextScene == nextScene::OptionScene)
		{
			m_nextScene = nextScene::RankingScene;
			m_sceneSelectGraph = m_sceneSelectUI[1];
			ChangeCursorInfo(1);
		}

		if (m_nextScene == nextScene::ExplanationScene)
		{
			m_nextScene = nextScene::GameEnd;
			m_sceneSelectGraph = m_sceneSelectUI[4];
			ChangeCursorInfo(4);
		}
		else if (m_nextScene == nextScene::GameEnd)
		{
			m_nextScene = nextScene::ExplanationScene;
			m_sceneSelectGraph = m_sceneSelectUI[3];
			ChangeCursorInfo(3);
		}
	}
}

void SceneSelect::ChangeCursorInfo(int num)
{
	GetGraphSize(m_sceneSelectGraph, &m_selectGraphX, &m_selectGraphY);
	c1.m_selectBox1 = kCursorUI[num];
	c1.m_selectBox2 = VGet(kCursorUI[num].x + m_selectGraphX+2, kCursorUI[num].y + m_selectGraphY+2, 0.0f);
}
