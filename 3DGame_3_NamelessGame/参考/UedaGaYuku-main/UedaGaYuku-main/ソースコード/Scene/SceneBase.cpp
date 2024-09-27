#include "DxLib.h"
#include "Input.h"
#include "Game.h"
#include "UI.h"
#include "Ranking.h"
#include "Sound.h"
#include "SceneBase.h"

namespace
{
	constexpr int kFadeColor = 0x1a0306; // フェードの色
	constexpr int kMaxFade = 255;		 // フェードの最大値
}


/// <summary>
/// コンストラクタ
/// </summary>
SceneBase::SceneBase():
	m_select(0),
	m_fadeAlpha(0),
	m_isFadeOut(false)
{
	m_pUI = std::make_shared<UI>();
	m_pRank = std::make_shared<Ranking>();
}


/// <summary>
/// デストラクタ
/// </summary>
SceneBase::~SceneBase()
{
}


/// <summary>
/// 選択状態を更新
/// </summary>
/// <param name="input">入力状態</param>
/// <param name="selectNum">選択肢の数</param>
void SceneBase::UpdateSelect(Input& input, int selectNum)
{
	// 選択状態を1つ下げる
	if (input.IsTriggered("down"))
	{
		m_select = (m_select + 1) % selectNum;
		m_pUI->Init();
		PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kCursor)], DX_PLAYTYPE_BACK);
	}
	// 選択状態を1つ上げる
	if (input.IsTriggered("up"))
	{
		m_select = (m_select + (selectNum - 1)) % selectNum;
		m_pUI->Init();
		PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kCursor)], DX_PLAYTYPE_BACK);
	}
}


/// <summary>
/// フェードイン処理
/// </summary>
/// <param name="fadeFrame">フェード変化量</param>
void SceneBase::FadeIn(int fadeFrame)
{
	m_fadeAlpha += fadeFrame;
	m_fadeAlpha = std::min(m_fadeAlpha, kMaxFade);
}


/// <summary>
/// フェードアウト処理
/// </summary>
/// <param name="fadeFrame">フェード変化量</param>
void SceneBase::FadeOut(int fadeFrame)
{
	m_fadeAlpha -= fadeFrame;
	m_fadeAlpha = std::max(0, m_fadeAlpha);

	if (m_fadeAlpha <= 0)
	{
		m_isFadeOut = false;
	}
	else
	{
		m_isFadeOut = true;
	}
}


/// <summary>
/// フェードインアウトの描画
/// </summary>
void SceneBase::DrawFade()
{
	// フェードイン
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, kFadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
