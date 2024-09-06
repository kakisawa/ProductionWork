#include "SceneTitle.h"
#include "DxLib.h"
#include "SceneGame.h"
#include "SceneOption.h"
#include "SceneRanking.h"
#include "../Util/Pad.h"
#include <cassert>

namespace {
	const char* const kUI[7]{
		"data/Title.png",							// タイトルロゴ
		"data/UI/TItleScene/Back.png",				// セレクト背景
		"data/UI/TItleScene/PressEnyButtom.png",	// PressEnyButton
		"data/UI/TItleScene/GameStart.png",			// GameStart
		"data/UI/TItleScene/Option.png",			// Option
		"data/UI/TItleScene/Ranking.png",			// Ranking
		"data/UI/TItleScene/GameFinish.png"			// GameFinish
	};

	constexpr int kTitlePosX = 300;
	constexpr int kTitlePosY = 50;

	constexpr int kPressBgPosX = 630;
	constexpr int kPressBgPosY = 780;

	constexpr int kSelectBgPosX = 200;
	constexpr int kSelectBgPosY = 790;

	constexpr int kPressEnyButtonPosX = 725;
	constexpr int kPressEnyButtonPosY = 805;

	constexpr int kSelect1X = 300;
	constexpr int kSelectY = 300;
}

SceneTitle::SceneTitle() :
	m_bgGraph(),
	m_nextScene(nextScene::kGameScene)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{

	// 画像の読み込み
	m_bgGraph = LoadGraph("data/Castle_Forrest10.png");

	// UI画像の読み込み
	for (int i = 0; i < m_uiGraph.size(); i++){
		m_uiGraph[i] = LoadGraph(kUI[i]);
		assert(m_uiGraph[i] != -1);
	}

	//ポインタの確保はSceneBase
	m_pSound->InitSound();	// サウンドの初期化
	m_pSound->LoadBGM(SoundManager::BGM_Type::kTitleBGM);	// サウンドの読み込み
	m_pSound->LoadSE(SoundManager::SE_Type::kSelectSE);
	m_pSound->LoadSE(SoundManager::SE_Type::kButtonSE);

	//m_pSound->PlayBGM(SoundManager::BGM_Type::kTitleBGM, DX_PLAYTYPE_LOOP);

	m_isNextSceneFlag = false;
}

shared_ptr<SceneBase> SceneTitle::Update()
{

	// 次のシーン選択
	SwitchingScene();

	// シーン遷移
	if (Pad::IsTrigger(PAD_INPUT_R)) {			// STARTボタン

		m_pSound->PlaySE(SoundManager::SE_Type::kButtonSE, DX_PLAYTYPE_NORMAL);

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
		if (m_nextScene == nextScene::kGameEnd)
		{
			DxLib_End();						// ゲームを終了する
		}
	}

#ifdef _DEBUG
#endif // DEBUG

	return shared_from_this();
}

void SceneTitle::Draw()
{
	DrawExtendGraph(0, 0,1920,1080, m_bgGraph, true);
	DrawGraph(kTitlePosX, kTitlePosY, m_uiGraph[0], true);

	//// パターン1
	//DrawGraph(kPressBgPosX, kPressBgPosY, m_uiGraph[1], true);
	//DrawGraph(kPressEnyButtonPosX, kPressEnyButtonPosY, m_uiGraph[2], true);

	// パターン2
	DrawExtendGraph(kSelectBgPosX, kSelectBgPosY, kSelectBgPosX+1500, kSelectBgPosY+90, m_uiGraph[1], true);
	DrawGraph(kPressEnyButtonPosX, kPressEnyButtonPosY, m_uiGraph[3], true);
	DrawGraph(kPressEnyButtonPosX, kPressEnyButtonPosY, m_uiGraph[4], true);
	DrawGraph(kPressEnyButtonPosX, kPressEnyButtonPosY, m_uiGraph[5], true);
	DrawGraph(kPressEnyButtonPosX, kPressEnyButtonPosY, m_uiGraph[6], true);




	DrawString(0, 20, "Please Press AnyButton", 0x00ffff);

#ifdef _DEBUG
	DrawString(0, 0, "SceneTitle", 0xffffff);
	DrawFormatString(0, 700, 0xffffff, "m_nextScene=%d", m_nextScene);
#endif // DEBUG
}

void SceneTitle::End()
{
	DeleteGraph(m_bgGraph);

	// UI画像の読み込み
	for (int i = 0; i < m_uiGraph.size(); i++) {
		DeleteGraph(m_uiGraph[i]);
	}

	SceneBase::End();
}

void SceneTitle::SwitchingScene()
{
	// 下キーを押すと次のシーンの変更をする
	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kSelectSE, DX_PLAYTYPE_BACK);

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
			m_nextScene = nextScene::kGameEnd;
		}
		else if (m_nextScene == nextScene::kGameEnd)
		{
			m_nextScene = nextScene::kGameScene;
		}
	}

	// 上キーを押すと次のシーンの変更をする
	if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kSelectSE, DX_PLAYTYPE_BACK);

		if (m_nextScene == nextScene::kGameScene)
		{
			m_nextScene = nextScene::kGameEnd;
		}
		else if (m_nextScene == nextScene::kGameEnd)
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