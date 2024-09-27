#pragma once
#include "SceneBase.h"

/// <summary>
/// タイトル画面
/// </summary>
class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	virtual  ~SceneTitle();
	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();

private:
	void UpdateDisplay();

private:
	int m_time;					// 現在の時間
	int m_textDisplayTime;		// テキストを表示する時間
	float m_titleLogoScale;		// タイトルロゴの拡大率
	float m_titleLogoRot;		// タイトルロゴの回転率
	int m_textAlpha;			// テキストのアルファ値
	int m_opStartTime;			// オープニング動画を再生するまでの時間
	std::vector<int> m_handle;  // ハンドル

	// ハンドルの種類
	enum HandleKind
	{
		kTitleLogo,		// タイトルロゴ
		kTitleLogoBack,	// タイトルロゴの背景
		kPressText,		// "PRESSANYBUTTON"のテキスト
		kOpMovie,		// OP動画
		kHandleNum		// ハンドルの数
	};
};