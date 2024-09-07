#include "SceneTitle.h"
#include "DxLib.h"
#include "SceneGame.h"
#include "SceneOption.h"
#include "SceneRanking.h"
#include "../Util/Pad.h"
#include <cassert>

namespace {
	const char* const kUI[13]{
		"data/Title.png",							// タイトルロゴ
		"data/UI/TItleScene/Back.png",				// セレクト背景
		"data/UI/TItleScene/PressEnyButtom.png",	// PressEnyButton
		"data/UI/TItleScene/GameStart.png",			// GameStart
		"data/UI/TItleScene/Option.png",			// Option
		"data/UI/TItleScene/Ranking.png",			// Ranking
		"data/UI/TItleScene/GameFinish.png",		// GameFinish
		"data/UI/TItleScene/説明背景.png",			// 説明背景
		"data/UI/TItleScene/移動先1.png",			// 説明文1
		"data/UI/TItleScene/移動先2.png",			// 説明文2
		"data/UI/TItleScene/移動先3.png",			// 説明文3
		"data/UI/TItleScene/移動先4.png",			// 説明文4
		"data/UI/TItleScene/gf_o.png",				// カーソル
	};

	constexpr int kTitlePosX = 300;
	constexpr int kTitlePosY = 50;

	constexpr int kPressBgPosX = 630;
	constexpr int kPressBgPosY = 780;

	constexpr int kSelectBgPosX = 200;
	constexpr int kSelectBgPosY = 790;

	constexpr int kPressEnyButtonPosX = 725;
	constexpr int kPressEnyButtonPosY = 810;

	constexpr int kSelect1X = 300;
	constexpr int kSelect2X = 650;
	constexpr int kSelect3X = 950;
	constexpr int kSelect4X = 1250;
	constexpr int kSelectPosY = 805;

	constexpr int kExplanationBgPosY = 1003;
	constexpr int kExplanationPosY = 1015;

	constexpr float kBgScroll1 = 1.5f;

	const VECTOR kSelectPos[8]{
		(VGet(kSelect1X,kSelectPosY,0)),
		(VGet(kSelect1X + 272 ,kSelectPosY + 71,0)),
		(VGet(kSelect2X,kSelectPosY,0)),
		(VGet(kSelect2X + 168 ,kSelectPosY + 71,0)),
		(VGet(kSelect3X,kSelectPosY,0)),
		(VGet(kSelect3X + 210 ,kSelectPosY + 71,0)),
		(VGet(kSelect4X,kSelectPosY,0)),
		(VGet(kSelect4X + 306 ,kSelectPosY + 71,0))
	};
}

SceneTitle::SceneTitle() :
	m_bgGraph(),
	m_scrollX_1(0.0f),
	m_fadeAlpha(255),
	m_isSelect(false),
	m_isFadeIn(true),
	m_isFadeOut(false),
	m_isBackScene(false),
	m_nextScene(nextScene::kGameScene),
	c1{ kSelectPos[0],kSelectPos[1] }
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
	for (int i = 0; i < m_uiGraph.size(); i++) {
		m_uiGraph[i] = LoadGraph(kUI[i]);
		assert(m_uiGraph[i] != -1);
	}

	//ポインタの確保はSceneBase
	m_pSound->Init();
	m_pSound->LoadBGM(SoundManager::BGM_Type::kTitleBGM);	// サウンドの読み込み
	m_pSound->LoadSE(SoundManager::SE_Type::kSelectSE);
	m_pSound->LoadSE(SoundManager::SE_Type::kButtonSE);

	m_pSound->PlayBGM(SoundManager::BGM_Type::kTitleBGM, DX_PLAYTYPE_LOOP);

	m_isNextSceneFlag = false;
}

shared_ptr<SceneBase> SceneTitle::Update()
{
	// フェードイン
	if (m_isFadeIn)
	{
		m_fadeAlpha -= 8;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
			m_isFadeIn = false;
		}
	}

	

	if (m_isSelect) {

		// 背景スクロール
		m_scrollX_1 += kBgScroll1;

		// 次のシーン選択
		SwitchingScene();

		// フェードアウト
		if (m_isFadeOut)
		{
			m_fadeAlpha += 8;
			if (m_fadeAlpha > 255)
			{
				m_fadeAlpha = 255;
				m_isNextSceneFlag = true;
				m_isFadeOut = false;
			}
		}

		// シーン遷移
		if (Pad::IsTrigger(PAD_INPUT_R)) {			// STARTボタン

			m_pSound->PlaySE(SoundManager::SE_Type::kButtonSE, DX_PLAYTYPE_NORMAL);
			m_isFadeOut = true;
		}

		if (m_isNextSceneFlag)
		{
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
	}
	else {
		m_isSelect = CheckHitKeyAll(DX_CHECKINPUT_PAD);
	}

	return shared_from_this();
}

void SceneTitle::Draw()
{
	DrawExtendGraph(0, 0, 1920, 1080, m_bgGraph, true);
	DrawGraph(kTitlePosX, kTitlePosY, m_uiGraph[0], true);

	if (!m_isSelect) {
		static int m_fadeAlpha;
		static bool isFade;

		if (isFade) {
			m_fadeAlpha += 3;

			if (m_fadeAlpha >= 255) {
				isFade = false;
			}
		}
		else {
			m_fadeAlpha -= 5;
			if (m_fadeAlpha <= 0) {
				isFade = true;
			}
		}

		// フェードイン・フェードアウト描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// 半透明で表示開始
		DrawGraph(kPressBgPosX, kPressBgPosY, m_uiGraph[1], true);
		DrawGraph(kPressEnyButtonPosX, kPressEnyButtonPosY, m_uiGraph[2], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく	
	}
	else {
		DrawExtendGraph(kSelectBgPosX, kSelectBgPosY, kSelectBgPosX + 1500, kSelectBgPosY + 100, m_uiGraph[1], true);
		DrawGraph(kSelect1X, kSelectPosY, m_uiGraph[3], true);
		DrawGraph(kSelect2X, kSelectPosY + 2, m_uiGraph[4], true);
		DrawGraph(kSelect3X, kSelectPosY + 3, m_uiGraph[5], true);
		DrawGraph(kSelect4X, kSelectPosY, m_uiGraph[6], true);

		DrawExtendGraph(c1.m_selectBox1.x - 20, c1.m_selectBox1.y - 1,
			c1.m_selectBox2.x + 20, c1.m_selectBox2.y + 1, m_uiGraph[12], true);

		DrawExplanation();
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// 半透明で表示開始
	DrawBoxAA(0, 0, 1920, 1080, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
#ifdef _DEBUG
	DrawString(0, 0, "SceneTitle", 0xffffff);
	DrawFormatString(0, 700, 0xffffff, "m_nextScene=%d", m_nextScene);
#endif // DEBUG
}

void SceneTitle::End()
{
	DeleteGraph(m_bgGraph);

	// UI画像の削除
	for (int i = 0; i < m_uiGraph.size(); i++) {
		DeleteGraph(m_uiGraph[i]);
	}

	SceneBase::End();
}

void SceneTitle::SwitchingScene()
{
	// 下キーを押すと次のシーンの変更をする
	if (Pad::IsTrigger(PAD_INPUT_RIGHT))
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kSelectSE, DX_PLAYTYPE_BACK);

		if (m_nextScene == nextScene::kGameScene)
		{
			c1 = { kSelectPos[2],kSelectPos[3] };
			m_nextScene = nextScene::kOptionScene;
		}
		else if (m_nextScene == nextScene::kOptionScene)
		{
			c1 = { kSelectPos[4],kSelectPos[5] };
			m_nextScene = nextScene::kRankingScene;
		}
		else if (m_nextScene == nextScene::kRankingScene)
		{
			c1 = { kSelectPos[6],kSelectPos[7] };
			m_nextScene = nextScene::kGameEnd;
		}
		else if (m_nextScene == nextScene::kGameEnd)
		{
			c1 = { kSelectPos[0],kSelectPos[1] };
			m_nextScene = nextScene::kGameScene;
		}
	}

	// 上キーを押すと次のシーンの変更をする
	if (Pad::IsTrigger(PAD_INPUT_LEFT))
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kSelectSE, DX_PLAYTYPE_BACK);

		if (m_nextScene == nextScene::kGameScene)
		{
			c1 = { kSelectPos[6],kSelectPos[7] };
			m_nextScene = nextScene::kGameEnd;
		}
		else if (m_nextScene == nextScene::kGameEnd)
		{
			c1 = { kSelectPos[4],kSelectPos[5] };
			m_nextScene = nextScene::kRankingScene;
		}
		else if (m_nextScene == nextScene::kRankingScene)
		{
			c1 = { kSelectPos[2],kSelectPos[3] };
			m_nextScene = nextScene::kOptionScene;
		}
		else if (m_nextScene == nextScene::kOptionScene)
		{
			c1 = { kSelectPos[0],kSelectPos[1] };
			m_nextScene = nextScene::kGameScene;
		}
	}
}

void SceneTitle::DrawExplanation()
{
	// 背景画像のサイズ取得
	Size expBgSize, expSize;
	GetGraphSize(m_uiGraph[7], &expBgSize.m_width, &expBgSize.m_height);
	GetGraphSize(m_uiGraph[8], &expSize.m_width, &expSize.m_height);

	// スクロール量を計算
	int scrollBg = static_cast<int>(m_scrollX_1) % static_cast<int>(expBgSize.m_width);

	// 説明文背景描画
	for (int index = 0; index < 4; index++)
	{
		DrawGraph(-scrollBg + index * expBgSize.m_width,
			static_cast<int>(kExplanationBgPosY),
			m_uiGraph[7], true);
	}

	static int next;
	// 説明文描画
	if (m_nextScene == nextScene::kGameScene) {
		next = 8;
	}
	if (m_nextScene == nextScene::kOptionScene) {
		next = 9;
	}
	if (m_nextScene == nextScene::kRankingScene) {
		next = 10;
	}
	if (m_nextScene == nextScene::kGameEnd) {
		next = 11;
	}

	for (int index = 0; index < 4; index++)
	{
		DrawGraph(-scrollBg + index * expSize.m_width,
			static_cast<int>(kExplanationPosY),
			m_uiGraph[next], true);
	}
}
