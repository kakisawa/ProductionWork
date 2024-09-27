#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"
#include "Font.h"
#include "UI.h"
#include <algorithm>

// 定数
namespace
{
	/*背景関連*/
	constexpr int kBackColor = 0xdddddd;				// 背景の色
	constexpr int kBackBoxColor = 0x494949;				// 四角の色
	constexpr int kBackBoxLTPos = 140;					// 四角の左上位置
	constexpr int kBackBoxWidth = 490;					// 四角の幅
	const Vec2 kBackHandleLTPos = { 750.0f, 200.0f };	// 背景画像の左上位置
	const Vec2 kBackHandleRBPos = { 1779.0f, 670.0f };	// 背景画像の右下位置
	constexpr int kMulaPal = 255;						// 乗算値

	const Vec2 kTextBoxLTPos = { 750.0f, 700.0f };		// ステージ選択画面のテキストボックス左上位置
	const Vec2 kTextBoxRBPos = { 1779.0f, 890.0f };		// ステージ選択画面のテキストボックス右下位置

	const Vec2 kPauseBackLTPos = { 600.0f, 200.0f };	// ポーズ画面の背景画像左上位置
	const Vec2 kPauseBackRBPos = { 1320.0f, 880.0f };	// ポーズ画面の背景画像右下位置

	const Vec2 kClearTimeBgFrameLTPos = { 170, 420 };	// クリア画面(クリアタイム部分)の背景画像左上位置
	const Vec2 kClearTimeBgFrameRBPos = { 900, 950 };	// クリア画面(クリアタイム部分)の背景画像右下位置
	const Vec2 kClearRankBgFrameLTPos = { 950, 420 };	// クリア画面(ランキング部分)の背景画像左上位置
	const Vec2 kClearRankBgFrameRBPos = { 1800, 950 };	// クリア画面(ランキング部分)の背景画像右下位置

	const Vec2 kGameoverBgFrameLTPos = { 620, 580 };	// ゲームオーバー画面の背景画像左上位置
	const Vec2 kGameoverBgFrameRBPos = { 1300, 1000 };	// ゲームオーバー画面の背景画像右下位置

	/*カーソル関連*/
	constexpr float kCursorWidth = 489.0f;				// カーソルの横幅
	constexpr float kOptionCursorWidth = 1000.0f;		// オプション画面のカーソルの横幅
	constexpr float kCursorHeight = 90.0f;				// カーソルの縦幅
	constexpr float kCursorSpeed = 60.0f;				// カーソルの横幅の伸びる量
	constexpr int kTextDisplayTime = 2;					// カーソルの表示間隔
	constexpr int kTextDisplayAnimTime = 240;			// カーソルアニメーションの時間
	constexpr int kMaxAlpha = 255;						// 最大アルファ値
	constexpr int kMinAlpha = 40;						// 最小アルファ値

	/*ボタンの画像とテキストの位置*/
	const Vec2 kButtonPos = { 1500, 1000 };				// ボタン表示位置
	const Vec2 kButtonTextPos = { 1530, 970 };			// テキストの位置
	const Vec2 kTitleButtonPos = { 1605, 1005 };		// タイトル画面ボタン位置
	const Vec2 kTitleButtonTextPos = { 1630, 980 };		// タイトル画面ボタンテキスト位置
	const Vec2 kClearButtonPos = { 1450, 1000 };		// クリア時ボタン表示位置
	const Vec2 kClearButtonTextPos = { 1480, 975 };		// クリア時のテキスト位置
	constexpr float kButtonWidth = 180.0f;				// 表示するボタンの幅
	constexpr int kButtonSize = 32;						// ボタン画像のサイズ
	constexpr float kButtonScale = 1.5f;				// ボタンの拡大率
	constexpr float kButtonTextWidth = 170.0f;			// テキストの幅
	constexpr int kTextColor = 0xffffff;				// テキストの色
	constexpr int kTitleTextColor = 0x000000;			// タイトル部分のテキストの色
}


/// <summary>
/// コンストラクタ
/// </summary>
UI::UI():
	m_isOption(false),
	m_cursorWidth(0.0f),
	m_cursorAlpha(kMaxAlpha)
{
	m_buttonHandle = LoadGraph("data/UI/button.png");
	m_handle.resize(HandleKind::kHandleNum);
	m_handle[HandleKind::kCursor] = LoadGraph("data/UI/cursor.png");
	m_handle[HandleKind::kBg] = LoadGraph("data/UI/back.png");
}


/// <summary>
/// デストラクタ
/// </summary>
UI::~UI()
{
	DeleteGraph(m_buttonHandle);
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
}


/// <summary>
/// 初期化
/// </summary>
void UI::Init()
{
	m_cursorWidth = 0.0f;
}


/// <summary>
/// 更新
/// </summary>
void UI::Update()
{
	// カーソルの横幅を更新
	// オプション画面の場合のみカーソルの長さを変える
	m_cursorWidth += kCursorSpeed;
	if (m_isOption)
	{
		m_cursorWidth = std::min(m_cursorWidth, kOptionCursorWidth);
	}
	else
	{
		m_cursorWidth = std::min(m_cursorWidth, kCursorWidth);
	}

	// カーソルのα値を更新
	m_cursorDisplayTime += kTextDisplayTime;
	m_cursorDisplayTime %= kTextDisplayAnimTime;
	// MEMO:sin波を使って0～1の範囲にする
	float sinAlpha = 0.5f + 0.5f * sinf(static_cast<float>(m_cursorDisplayTime) / kTextDisplayAnimTime * DX_PI_F);
	m_cursorAlpha = kMinAlpha + static_cast<int>((kMaxAlpha - kMinAlpha) * sinAlpha);
}


/// <summary>
///  カーソル表示
/// </summary>
/// <param name="pos">表示位置</param>
/// <param name="select">選択状態</param>
/// <param name="interval">表示間隔</param>
void UI::DrawCursor(Vec2 pos, int select, float interval, bool isOption)
{
	m_isOption = isOption;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_cursorAlpha);
	DrawExtendGraphF(pos.x, pos.y + interval * select,
		pos.x + m_cursorWidth, pos.y + interval * select + kCursorHeight,
		m_handle[HandleKind::kCursor], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


/// <summary>
/// ポーズ画面の背景表示
/// </summary>
void UI::DrawPauseBack()
{
	// 背景画像表示
	DrawExtendGraphF(kPauseBackLTPos.x, kPauseBackLTPos.y, kPauseBackRBPos.x, kPauseBackRBPos.y, m_handle[HandleKind::kBg], true);
}


/// <summary>
/// メニューの背景表示
/// </summary>
void UI::DrawMenuBg()
{
	// 背景描画
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, kBackColor, true);
	// 背景の四角部分表示
	DrawBox(kBackBoxLTPos, 0, kBackBoxLTPos + kBackBoxWidth, Game::kScreenHeight, kBackBoxColor, true);
	// 背景画像表示
	DrawExtendGraphF(kBackHandleLTPos.x, kBackHandleLTPos.y, kBackHandleRBPos.x, kBackHandleRBPos.y, m_handle[HandleKind::kBg], true);
	// テキストボックス表示
	DrawExtendGraphF(kTextBoxLTPos.x, kTextBoxLTPos.y, kTextBoxRBPos.x, kTextBoxRBPos.y, m_handle[HandleKind::kBg], true);
}


/// <summary>
/// ボタンの画像とテキストを表示する
/// </summary>
void UI::DrawButtonText()
{
	// テキスト表示
	DrawStringFToHandle(kButtonTextPos.x, kButtonTextPos.y,
		"決定", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kButtonText)]);
	DrawStringFToHandle(kButtonTextPos.x + kButtonTextWidth, kButtonTextPos.y,
		"もどる", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kButtonText)]);

	// ボタン画像表示
	DrawRectRotaGraphF(kButtonPos.x, kButtonPos.y, kButtonSize * ButtonKind::kAButton, 0, kButtonSize, kButtonSize, kButtonScale, 0.0f, m_buttonHandle, true);
	DrawRectRotaGraphF(kButtonPos.x + kButtonWidth, kButtonPos.y, kButtonSize * ButtonKind::kBButton, 0, kButtonSize, kButtonSize, kButtonScale, 0.0f, m_buttonHandle, true);
}


/// <summary>
/// タイトル画面でボタンの画像とテキストを表示する
/// </summary>
void UI::DrawTitleButtonText()
{
	// エッジ付きテキスト表示
	DrawFormatString2FToHandle(kTitleButtonTextPos.x, kTitleButtonTextPos.y, kTitleTextColor, kTextColor, 
		Font::m_fontHandle[static_cast<int>(Font::FontId::kTitleButtonText)], "でスキップ");

	// ボタン画像表示
	DrawRectRotaGraphF(kTitleButtonPos.x, kTitleButtonPos.y, kButtonSize * ButtonKind::kAButton, 0, kButtonSize, kButtonSize, kButtonScale, 0.0f, m_buttonHandle, true);
}


/// <summary>
/// クリア画面でボタンの画像とテキスト表示する
/// </summary>
void UI::DrawClearButtonText()
{
	// テキスト表示
	DrawStringFToHandle(kClearButtonTextPos.x, kClearButtonTextPos.y,
		"ステージ選択にもどる", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kClearButtonText)]);

	// ボタン画像表示
	DrawRectRotaGraphF(kClearButtonPos.x, kClearButtonPos.y, kButtonSize * ButtonKind::kAButton, 0, kButtonSize, kButtonSize, kButtonScale, 0.0f, m_buttonHandle, true);
}


/// <summary>
/// クリア画面の背景枠表示
/// </summary>
void UI::DrawClearBgFrame()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, kMulaPal);
	DrawExtendGraphF(kClearTimeBgFrameLTPos.x, kClearTimeBgFrameLTPos.y, kClearTimeBgFrameRBPos.x, kClearTimeBgFrameRBPos.y, m_handle[HandleKind::kBg], true);
	DrawExtendGraphF(kClearRankBgFrameLTPos.x, kClearRankBgFrameLTPos.y, kClearRankBgFrameRBPos.x, kClearRankBgFrameRBPos.y, m_handle[HandleKind::kBg], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


/// <summary>
/// ゲームオーバーの背景枠表示
/// </summary>
void UI::DrawGameoverBgFrame()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, kMulaPal);
	DrawExtendGraphF(kGameoverBgFrameLTPos.x, kGameoverBgFrameLTPos.y, kGameoverBgFrameRBPos.x, kGameoverBgFrameRBPos.y, m_handle[HandleKind::kBg], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
