#include "SceneGameClear.h"
#include "SceneTitle.h"
#include "../Pad.h"
#include "DxLib.h"
#include "../Game.h"

namespace {
}

SceneGameClear::SceneGameClear() :
	m_fadeAlpha(255),
	m_isFadeIn(false),
	m_isFadeOut(false),
	m_isSceneEnd(false)
{
	// 画像読み込み
	graph = LoadGraph("data/graph/GameClear2.png");
}

SceneGameClear::~SceneGameClear()
{
	// 画像削除
}

std::shared_ptr<SceneBase> SceneGameClear::Update()
{
	Pad::Update();

	// フェードイン
	if (!m_isFadeIn)
	{
		m_fadeAlpha -= 8;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
			m_isFadeIn = true;
		}
	}

	if (Pad::IsTrigger(PAD_INPUT_10))		// スペースキーが押されたら
	{
		m_isFadeOut = true;					// フェードアウトフラグをtrueにする
	}
	// フェードアウト
	if (m_isFadeOut)
	{
		if (m_isSceneEnd)
		{
			return make_shared<SceneTitle>();		// タイトルシーンへ行く
		}
		m_fadeAlpha += 8;
		if (m_fadeAlpha >= 255)
		{
			m_fadeAlpha = 255;
			m_isSceneEnd = true;
		}
	}

	return shared_from_this();		// 自身のshared_ptrを返す
}

void SceneGameClear::Draw()
{
	DrawString(0, 0, "SceneGameClear", 0xffffff);


	DrawGraph(100, 200, graph, true);

	// フェードイン・フェードアウト描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// 半透明で表示開始
	DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, 0x00000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}