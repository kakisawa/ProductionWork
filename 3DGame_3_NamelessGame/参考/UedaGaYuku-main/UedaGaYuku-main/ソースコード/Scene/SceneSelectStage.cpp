#include "EffekseerForDXLib.h"
#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"
#include "Input.h"
#include "UI.h"
#include "Sound.h"
#include "Font.h"
#include "Ranking.h"
#include "Player.h"
#include "Camera.h"
#include "Stage.h"
#include "SceneStage1.h"
#include "SceneStage2.h"
#include "SceneOption.h"
#include "SceneTitle.h"
#include "SceneSelectStage.h"

// 定数
namespace
{
	constexpr int kBackColor = 0xdddddd;					// 背景の色
	constexpr int kBackBoxColor = 0x494949;					// 四角の色
	constexpr int kBackBoxLTPos = 140;						// 四角の左上位置
	constexpr int kBackBoxWidth = 490;						// 四角の幅

	const Vec2 kSelectTextPos = { 260.0f, 300.0f };			// 選択テキスト表示位置
	constexpr float kSelectTextInterval = 120.0f;			// 選択テキスト表示間隔
	constexpr float kSelectTextAdj = 40.0f;					// テキスト表示位置調整
	const Vec2 kRankingTextPos = { 1000.0f, 250.0f };		// "ランキング"表示位置
	const Vec2 kIntroductionTextPos = { 900.0f, 770.0f };	// ステージ紹介テキスト位置
	constexpr float kIntroductionTextAdj = 30.0f;			// ステージ紹介テキスト位置調整
	const Vec2 kCursorLTPos = { 140.0f, 290.0f };			// カーソル左上位置
	constexpr int kTextColor = 0xffffff;					// テキストの色

	constexpr int kStartFadeAlpha = 255;					// スタート時のフェード値
	constexpr int kFadeFrame = 8;							// フェード変化量
}


/// <summary>
/// コンストラクタ
/// </summary>
SceneSelectStage::SceneSelectStage()
{
	m_fadeAlpha = kStartFadeAlpha;
	m_select = SelectScene::kStage1;
}


/// <summary>
/// デストラクタ
/// </summary>
SceneSelectStage::~SceneSelectStage()
{
	StopSoundMem(Sound::m_bgmHandle[static_cast<int>(Sound::BgmKind::kStageSelect)]);
}


/// <summary>
/// 初期化
/// </summary>
void SceneSelectStage::Init()
{
	// ランキング取得
	if (m_select == SelectScene::kStage1)
	{
		m_pRank->GetRanking(StageKind::kStage1);
	}
	else if (m_select == SelectScene::kStage2)
	{
		m_pRank->GetRanking(StageKind::kStage2);
	}
}


/// <summary>
/// 更新
/// </summary>
/// <param name="input">入力処理</param>
/// <returns>遷移先のシーン</returns>
std::shared_ptr<SceneBase> SceneSelectStage::Update(Input& input)
{
	FadeOut(kFadeFrame); // フェードアウト

	//選択状態更新
	UpdateSelect(input, kSelectNum);
	m_pUI->Update();

	// ランキングを再取得
	if (input.IsTriggered("up") || input.IsTriggered("down"))
	{
		if (m_select == SelectScene::kStage1 || m_select == SelectScene::kStage2)
		{
			Init();
		}
	}

	// BGMを鳴らす
	if (!CheckSoundMem(Sound::m_bgmHandle[static_cast<int>(Sound::BgmKind::kStageSelect)]))
	{
		PlaySoundMem(Sound::m_bgmHandle[static_cast<int>(Sound::BgmKind::kStageSelect)], DX_PLAYTYPE_LOOP);
	}

	// シーン遷移
	if (input.IsTriggered("OK"))
	{
		PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kSelect)], DX_PLAYTYPE_BACK); // SEを鳴らす

		std::shared_ptr<Player> pPlayer = std::make_shared<Player>();
		std::shared_ptr<Camera> pCamera = std::make_shared<Camera>();
		std::shared_ptr<Stage> pStage = std::make_shared<Stage>();

		if (m_select == SelectScene::kStage1)
		{
			FadeIn(kFadeFrame); // フェードイン
			return std::make_shared<SceneStage1>(pPlayer, pCamera, pStage);
		}
		else if (m_select == SelectScene::kStage2)
		{
			FadeIn(kFadeFrame); // フェードイン
			return std::make_shared<SceneStage2>(pPlayer, pCamera, pStage);
		}
		else if (m_select == kOption)
		{
			FadeIn(kFadeFrame); // フェードイン
			return std::make_shared<SceneOption>(shared_from_this());
		}
		else if (m_select == kEnd)
		{
			Effkseer_End();	// Effekseerの終了処理
			DxLib_End();
		}
	}
	if (input.IsTriggered("back"))
	{
		FadeIn(kFadeFrame); // フェードイン
		return std::make_shared<SceneTitle>();
	}

	return shared_from_this();
}


/// <summary>
/// 描画
/// </summary>
void SceneSelectStage::Draw()
{
	// 背景描画
	m_pUI->DrawMenuBg();
	// カーソル表示
	m_pUI->DrawCursor(kCursorLTPos, m_select, kSelectTextInterval);
	m_pUI->DrawButtonText();

	// テキスト表示
	DrawStringFToHandle(kSelectTextPos.x, kSelectTextPos.y + kSelectTextInterval * SelectScene::kStage1,
		"練習へ往く", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kSelectStage)]);
	DrawStringFToHandle(kSelectTextPos.x, kSelectTextPos.y + kSelectTextInterval * SelectScene::kStage2,
		"闘いへ往く", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kSelectStage)]);
	DrawStringFToHandle(kSelectTextPos.x, kSelectTextPos.y + kSelectTextInterval * SelectScene::kOption,
		"オプション", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kSelectStage)]);
	DrawStringFToHandle(kSelectTextPos.x - kSelectTextAdj, kSelectTextPos.y + kSelectTextInterval * SelectScene::kEnd,
		"ゲームを終わる", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kSelectStage)]);

	// ランキング表示
	if (m_select == SelectScene::kStage1 || m_select == SelectScene::kStage2)
	{
		DrawStringFToHandle(kRankingTextPos.x, kRankingTextPos.y,
			"オンラインランキング", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kRankingText)]);
		m_pRank->DrawStageSelectRanking();
	}

	//ステージの紹介文を表示する
	DrawIntroductionText();

	DrawFade();	// フェードインアウト描画

#ifdef _DEBUG	// デバッグ表示
	// 現在のシーン
	DrawString(0, 0, "ステージ選択画面", 0xffffff);
#endif
}


/// <summary>
/// ステージの紹介文を表示する
/// </summary>
void SceneSelectStage::DrawIntroductionText()
{
	if (m_select == SelectScene::kStage1)
	{
		DrawStringFToHandle(kIntroductionTextPos.x, kIntroductionTextPos.y,
			"操作方法の説明を行います。", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kIntroductionText)]);
	}
	else if (m_select == SelectScene::kStage2)
	{
		DrawStringFToHandle(kIntroductionTextPos.x, kIntroductionTextPos.y - kIntroductionTextAdj,
			"3連続勝利したら優勝です。\n優勝めざして頑張ってください!", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kIntroductionText)]);
	}
	else if (m_select == SelectScene::kOption)
	{
		DrawStringFToHandle(kIntroductionTextPos.x, kIntroductionTextPos.y,
			"音量や画面サイズの設定を行います。", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kIntroductionText)]);
	}
	else if (m_select == SelectScene::kEnd)
	{
		DrawStringFToHandle(kIntroductionTextPos.x, kIntroductionTextPos.y,
			"ゲームを終了します。", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kIntroductionText)]);
	}
}


