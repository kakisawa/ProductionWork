#include "SceneTitle.h"
#include "DxLib.h"
#include "SceneGame.h"
#include "SceneOption.h"
#include "SceneRanking.h"
#include "../Util/Pad.h"
#include "../Util/Fade.h"
#include <cassert>

namespace {
	const char* const kUI[13]{
		"data/Image/TitleScene/UI/Title.png",			// タイトルロゴ
		"data/Image/TitleScene/UI/Back.png",				// セレクト背景
		"data/Image/TitleScene/UI/PressEnyButtom.png",	// PressEnyButton
		"data/Image/TitleScene/UI/GameStart.png",			// GameStart
		"data/Image/TitleScene/UI/Option.png",			// Option
		"data/Image/TitleScene/UI/Ranking.png",			// Ranking
		"data/Image/TitleScene/UI/GameFinish.png",		// GameFinish
		"data/Image/TitleScene/UI/説明背景.png",			// テロップ背景
		"data/Image/TitleScene/UI/移動先1.png",			// テロップ1
		"data/Image/TitleScene/UI/移動先2.png",			// テロップ2
		"data/Image/TitleScene/UI/移動先3.png",			// テロップ3
		"data/Image/TitleScene/UI/移動先4.png",			// テロップ4
		"data/Image/TitleScene/UI/gf_o.png",				// カーソル
	};

	constexpr int kTitlePosX = 300;			// タイトルロゴ座標
	constexpr int kTitlePosY = 50;

	constexpr int kPressBgPosX = 630;		// 「PressEnyButton」背景座標
	constexpr int kPressBgPosY = 780;
	constexpr int kPressEnyButtonPosX = 725;// 「PressEnyButton」UI座標
	constexpr int kPressEnyButtonPosY = 810;

	constexpr int kSelectBgPosX = 200;		// セレクト背景座標
	constexpr int kSelectBgPosY = 790;
	constexpr int kSelect1X = 300;			// セレクト座標1
	constexpr int kSelect2X = 650;			// セレクト座標2
	constexpr int kSelect3X = 950;			// セレクト座標3
	constexpr int kSelect4X = 1250;			// セレクト座標4
	constexpr int kSelectPosY = 805;		// セレクト座標

	constexpr int kCaptionBgPosY = 1003;	// テロップ背景座標
	constexpr int kCaptionPosY = 1015;		// テロップ座標

	constexpr float kCaptionScroll = 1.5f;		// テロップスクロール移動量
	constexpr float kCaptionPosX = -30.0f;		// テロップスクロール移動量

	const VECTOR kSelectPos[8]{
		(VGet(kSelect1X,kSelectPosY,0)),			// セレクト1座標左上
		(VGet(kSelect1X + 272 ,kSelectPosY + 71,0)),// セレクト1座標右下
		(VGet(kSelect2X,kSelectPosY,0)),			// セレクト2座標左上
		(VGet(kSelect2X + 168 ,kSelectPosY + 71,0)),// セレクト2座標右下
		(VGet(kSelect3X,kSelectPosY,0)),			// セレクト3座標左上
		(VGet(kSelect3X + 210 ,kSelectPosY + 71,0)),// セレクト3座標右下
		(VGet(kSelect4X,kSelectPosY,0)),			// セレクト4座標左上
		(VGet(kSelect4X + 306 ,kSelectPosY + 71,0))	// セレクト4座標右下
	};
}

SceneTitle::SceneTitle() :
	m_bgGraph(-1),
	m_titleGraph(-1),
	m_scrollX(kCaptionPosX),
	m_isMenu(false),
	m_nextScene(nextScene::kGameScene),
	c1{ kSelectPos[0],kSelectPos[1] }
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	// 背景画像の読み込み
	m_bgGraph = LoadGraph("data/Image/TitleScene/Castle_Forrest10.png");

	// UI画像の読み込み
	for (int i = 0; i < m_uiGraph.size(); i++) {
		m_uiGraph[i] = LoadGraph(kUI[i]);
		assert(m_uiGraph[i] != -1);
	}

	// サウンド関係
	//ポインタの確保はSceneBase
	m_pSound->Init();
	m_pSound->LoadBGM(SoundManager::BGM_Type::kTitleBGM);	// サウンドの読み込み
	m_pSound->LoadSE(SoundManager::SE_Type::kSelectSE);
	m_pSound->LoadSE(SoundManager::SE_Type::kButtonSE);

	// BGM
	m_pSound->PlayBGM(SoundManager::BGM_Type::kTitleBGM, DX_PLAYTYPE_LOOP);

	// フェード
	m_pFade = std::make_shared<Fade>();

	m_isNextSceneFlag = false;
}

shared_ptr<SceneBase> SceneTitle::Update()
{
	m_pFade->FadeIn(m_pFade->GatFadeInFlag());	// フェードイン

	if (m_isMenu) {					// いずれかのボタンが押されたら、メニューを表示する

		// テロップスクロール
		m_scrollX += kCaptionScroll;

		// 次のシーン選択
		SwitchingScene();

		// フェードアウト
		m_pFade->FadeOut(m_pFade->GatFadeOutFlag());
		m_isNextSceneFlag = m_pFade->GatNextSceneFlag();
		

		// シーン遷移
		if (m_pFade->GatFadeInFlag()==false&&Pad::IsTrigger(PAD_INPUT_A)) {			// Aボタン

			m_pSound->PlaySE(SoundManager::SE_Type::kButtonSE, DX_PLAYTYPE_BACK);	// SE
			m_pFade->SetFadeOutFlag(true);			// フェードアウトフラグ
		}

		if (m_isNextSceneFlag)						// 次のシーン
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
		m_isMenu = CheckHitKeyAll(DX_CHECKINPUT_ALL);		// いずれかのボタンを押す
	}

	return shared_from_this();
}

void SceneTitle::Draw()
{
	DrawExtendGraph(0, 0, 1920, 1080, m_bgGraph, true);		// 背景
	DrawGraph(kTitlePosX, kTitlePosY, m_uiGraph[0], true);	// タイトルロゴ

	if (!m_isMenu) {
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

		DrawExtendGraphF(c1.m_selectBox1.x - 20, c1.m_selectBox1.y - 1,
			c1.m_selectBox2.x + 20, c1.m_selectBox2.y + 1, m_uiGraph[12], true);

		DrawCaption();
	}

	m_pFade->Draw();
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

		m_scrollX = kCaptionPosX;

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

		m_scrollX = kCaptionPosX;

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

void SceneTitle::DrawCaption()
{
	// 背景画像のサイズ取得
	Size expBgSize, expSize;
	GetGraphSize(m_uiGraph[7], &expBgSize.m_width, &expBgSize.m_height);
	GetGraphSize(m_uiGraph[8], &expSize.m_width, &expSize.m_height);

	// スクロール量を計算
	int scrollBg = static_cast<int>(m_scrollX) % static_cast<int>(expBgSize.m_width);

	//DrawGraph(0.0f,static_cast<int>(kCaptionBgPosY),m_uiGraph[7], true);

	// 説明文背景描画
	for (int index = 0; index < 4; index++)
	{
		DrawGraph(-100-scrollBg + index * expBgSize.m_width,
			static_cast<int>(kCaptionBgPosY),
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
			static_cast<int>(kCaptionPosY),
			m_uiGraph[next], true);
	}
}
