#include "SceneTitle.h"
#include "SceneGame.h"
#include "DxLib.h"
#include "../Pad.h"

namespace {
}

SceneTitle::SceneTitle()
{
	// 画像読み込み
}

SceneTitle::~SceneTitle()
{
	// 画像削除
}

shared_ptr<SceneBase> SceneTitle::Update()
{
	Pad::Update();

	//// フェードイン
	//if (m_isFadeIn)
	//{
	//	m_fadeAlpha -= 8;
	//	if (m_fadeAlpha < 0)
	//	{
	//		m_fadeAlpha = 0;
	//		m_isFadeIn = false;
	//	}
	//}
	//if (Pad::IsTrigger(PAD_INPUT_10))
	//{
	//	m_pSound->SoundButton();
	//	m_isFadeOut = true;
	//	m_isFadeIn = false;
	//}
	//// フェードアウト
	//if (m_isFadeOut)
	//{
	//	if (m_isSceneEnd)
	//	{
	//		return make_shared<SceneGame>();	// ゲームシーンへ行く
	//	}
	//	m_fadeAlpha += 8;
	//	if (m_fadeAlpha >= 255)
	//	{
	//		m_isSceneEnd = true;
	//		m_fadeAlpha = 255;
	//	}
	//}


	if (Pad::IsTrigger(PAD_INPUT_10))
	{
		return make_shared<SceneGame>();
	}
	return shared_from_this();
}

void SceneTitle::Draw()
{
	DrawString(0, 0, "SceneTitle", 0xffffff);

	//// フェードイン・フェードアウト描画
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// 半透明で表示開始
	//DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, 0x00000, true);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}