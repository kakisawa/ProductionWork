#pragma once
#include "Vec2.h"

/// <summary>
/// UIの表示に関するクラス
/// </summary>
class UI
{
public:
	UI();
	virtual ~UI();
	void Init();
	void Update();
	void DrawCursor(Vec2 pos, int select, float interval, bool isOption = false); // カーソル表示
	void DrawPauseBack();		// ポーズ画面の背景表示
	void DrawMenuBg();			// メニューの背景表示
	void DrawButtonText();		// ボタンテキスト表示
	void DrawTitleButtonText();	// タイトル画面でのボタンテキスト表示
	void DrawClearButtonText();	// クリア画面でのボタンテキスト表示
	void DrawClearBgFrame();	// クリア画面の背景枠表示
	void DrawGameoverBgFrame();	// ゲームオーバー画面の背景枠表示


protected:
	int m_buttonHandle;		// ボタンの画像

	// ボタンの種類
	enum ButtonKind
	{
		kAButton,		// A
		kBButton,		// B
		kXButton,		// X
		kYButton,		// Y
		kMenuButton,	// メヌー
		kViewButton,	// ビュー
		kLStick,		// 左スティック
		kRStick,		// 右スティック
		kLBButton,		// LB
		kRBButton,		// RB
	};

private:
	bool m_isOption;			// オプション画面かどうか
	float m_cursorWidth;		// カーソルの横幅
	int m_cursorDisplayTime;	// カーソルを表示する時間
	int m_cursorAlpha;			// カーソルのα値
	std::vector<int> m_handle;  // ハンドル

	// 画像の種類
	enum HandleKind
	{
		kCursor,	// カーソル
		kBg,		// 背景
		kHandleNum	// ハンドルの種類
	};
};