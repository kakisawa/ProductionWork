#include "DxLib.h"
#include "Vec2.h"
#include "Input.h"
#include "Game.h"
#include "UI.h"
#include "Font.h"
#include "ScenePause.h"
#include "SceneOption.h"
#include "SceneSelectStage.h"

// 定数
namespace
{
	constexpr int kAlpha = 200;
	constexpr int kBackColor = 0x1a1a1a;				// 背景の色
	constexpr int kBackBoxColor = 0x494949;				// 四角の色
	constexpr int kTextColor = 0xffffff;				// テキストの色
	const Vec2 kBackTextPos = { 800, 350 };				// "ゲームにもどる"表示位置
	const Vec2 kOptionTextPos = { 860, 500 };			// "オプション"表示位置
	const Vec2 kStageTextPos = { 760, 650 };			// "ステージ選択にもどる"表示位置
	constexpr float kSelectTextInterval = 150.0f;		// 選択テキスト表示間隔
	const Vec2 kCursorPos = { 740, 340 };				// カーソル表示位置
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pScene">前に実行していたシーン</param>
ScenePause::ScenePause(std::shared_ptr<SceneBase> pScene)
{
	m_select = Select::kBack;
	m_pPrevScene = pScene;
}


/// <summary>
/// デストラクタ
/// </summary>
ScenePause::~ScenePause()
{
}


/// <summary>
/// 初期化
/// </summary>
void ScenePause::Init()
{
}


/// <summary>
/// 更新
/// </summary>
/// <param name="input">入力状態</param>
/// <returns>遷移先のポインタ</returns>
std::shared_ptr<SceneBase> ScenePause::Update(Input& input)
{
	// 選択状態を更新
	UpdateSelect(input, Select::kSelectNum);
	m_pUI->Update();

	if (input.IsTriggered("OK"))
	{
		if (m_select == Select::kBack)
		{
			return m_pPrevScene;	// ゲーム画面にもどる
		}
		else if (m_select == Select::kOption)
		{
			return std::make_shared<SceneOption>(shared_from_this());	// オプション画面に遷移
		}
		else if (m_select == Select::kStageSelect)
		{
			return std::make_shared<SceneSelectStage>(); // ステージ選択画面に遷移
		}
	}
	else if (input.IsTriggered("pause"))
	{
		return m_pPrevScene;	// ゲーム画面にもどる
	}

	return shared_from_this();	// 自身のshared_ptrを返す
}


/// <summary>
/// 描画
/// </summary>
void ScenePause::Draw()
{
	// プレイ画面を薄く表示する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAlpha);
	m_pPrevScene->Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 背景表示
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAlpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, kBackColor, true);
	m_pUI->DrawPauseBack();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// カーソル表示
	m_pUI->DrawCursor(kCursorPos, m_select, kSelectTextInterval);

	// テキスト表示
	DrawStringFToHandle(kBackTextPos.x, kBackTextPos.y,
		"ゲームにもどる", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kPause)]);
	DrawStringFToHandle(kOptionTextPos.x, kOptionTextPos.y,
		"オプション", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kPause)]);
	DrawStringFToHandle(kStageTextPos.x, kStageTextPos.y,
		"ステージ選択にもどる", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kPause)]);

#ifdef _DEBUG	// デバッグ表示
	// 現在のシーン
	DrawString(0, 0, "ポーズ画面", 0xffffff);
	// 中心線
	//DrawLine(Game::kScreenWidth * 0.5, 0, Game::kScreenWidth * 0.5, Game::kScreenHeight, 0x0000ff);
#endif
}
