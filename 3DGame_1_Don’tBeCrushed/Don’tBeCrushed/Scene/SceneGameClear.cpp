#include "SceneGameClear.h"
#include "SceneTitle.h"
#include "../Pad.h"
#include "DxLib.h"

namespace {
}

SceneGameClear::SceneGameClear()
{
	// 画像読み込み
}

SceneGameClear::~SceneGameClear()
{
	// 画像削除
}

std::shared_ptr<SceneBase> SceneGameClear::Update()
{
	Pad::Update();

	//// フェードイン
	//if (!m_isFadeIn)
	//{
	//	m_fadeAlpha -= 8;
	//	if (m_fadeAlpha < 0)
	//	{
	//		m_fadeAlpha = 0;
	//		m_isFadeIn = true;
	//	}
	//}
	//// 「Spaceキーをクリック」画像点滅用
	//if (m_displayCount >= 80)
	//{
	//	m_displayCount = 0;
	//}
	//m_displayCount++;
	//if (Pad::IsTrigger(PAD_INPUT_10))		// スペースキーが押されたら
	//{
	//	m_isFadeOut = true;					// フェードアウトフラグをtrueにする
	//	m_pSound->SoundButton();			// SEを鳴らす
	//}
	//// フェードアウト
	//if (m_isFadeOut)
	//{
	//	if (m_isSceneEnd)
	//	{
	//		return make_shared<SceneTitle>();		// タイトルシーンへ行く
	//	}
	//	m_fadeAlpha += 8;
	//	if (m_fadeAlpha >= 255)
	//	{
	//		m_pSound->StopBGMGameClear();	// BGMを止める
	//		m_fadeAlpha = 255;
	//		m_isSceneEnd = true;
	//	}
	//}

	if (Pad::IsTrigger(PAD_INPUT_10))
	{
		return make_shared<SceneTitle>();
	}

	return shared_from_this();		// 自身のshared_ptrを返す
}

void SceneGameClear::Draw()
{
	DrawString(0, 0, "SceneGameOver", 0xffffff);

	//// フェードイン・フェードアウト描画
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// 半透明で表示開始
	//DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, 0x00000, true);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}