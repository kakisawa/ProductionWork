#pragma once
#include <memory>

class Input;
class UI;
class Ranking;

/// <summary>
/// シーン基底クラス
/// </summary>
class SceneBase : public std::enable_shared_from_this<SceneBase>
{
public:
	SceneBase();
	virtual ~SceneBase();
	virtual void Init() = 0;
	virtual std::shared_ptr<SceneBase> Update(Input& input) = 0;
	virtual void Draw() = 0;

protected:
	void UpdateSelect(Input& input, int selectNum);	// 選択状態を更新
	virtual void FadeIn(int fadeFrame);				// フェードインの処理
	virtual void FadeOut(int fadeFrame);			// フェードアウトの処理
	void DrawFade();								// フェードインアウトの描画

protected:
	std::shared_ptr<Ranking> m_pRank;
	std::shared_ptr<UI> m_pUI;	// UIのポインタ
	int m_select;				// 現在の選択状態
	int m_fadeAlpha;			// フェードのα値
	int m_isFadeOut;			// フェード中かどうか(true:フェード中)

	// ステージの種類
	enum StageKind
	{
		kStage1 = 1,	// ステージ1
		kStage2 = 2,	// ステージ2
	};

#ifdef _DEBUG
	// デバッグ状況
	enum class DebugState
	{
		Normal,	// 通常
		Pause	// ポーズ
	};
	DebugState m_debugState = DebugState::Normal;
#endif
};

