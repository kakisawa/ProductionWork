                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 #include "SceneGameOver.h"
#include "SceneTitle.h"
#include "../Pad.h"
#include "DxLib.h"

namespace {
}

SceneGameOver::SceneGameOver()
{
	// 画像読み込み
}

SceneGameOver::~SceneGameOver()
{
	// 画像削除
}

std::shared_ptr<SceneBase> SceneGameOver::Update()
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
	//		return std::make_shared<SceneTitle>();	// タイトルシーンへ行く
	//	}
	//	m_fadeAlpha += 8;
	//	if (m_fadeAlpha >= 255)
	//	{
	//		// BGMを止める
	//		m_pSound->StopBGMGameOver();
	//		m_fadeAlpha = 255;
	//		m_isSceneEnd = true;
	//	}
	//}
	if (Pad::IsTrigger(PAD_INPUT_10))
	{
		return make_shared<SceneTitle>();
	}

	return shared_from_this();	// 自身のshared_ptrを返す
}

void SceneGameOver::Draw()
{

	DrawString(0, 0, "SceneGameOver", 0xffffff);

	//// フェードイン・フェードアウト描画
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// 半透明で表示開始
	//DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, 0x00000, true);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}