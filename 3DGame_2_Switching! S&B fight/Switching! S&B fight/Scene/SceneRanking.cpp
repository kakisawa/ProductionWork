#include "SceneRanking.h"
#include "SceneTitle.h"
#include "../Util/Pad.h"
#include "../Util/Fade.h"
#include <cassert>

namespace
{
	const char* const kUI[4]{

		"data/Image/RankingScene/Firefly-魔王城-外観-001-scaled.jpg",
		"data/Image/RankingScene/UI/ComingSoon.png",
		"data/Image/RankingScene/UI/説明背景.png",// セレクト背景
		"data/Image/RankingScene/UI/説明文.png",	// 説明文//要修正
	};

	constexpr float kCaptionScroll = 1.5f;

	constexpr int kCaptionBgPosY = 1003;
	constexpr int kCaptionPosY = 1015;
}

SceneRanking::SceneRanking()
{
}

SceneRanking::~SceneRanking()
{
}

void SceneRanking::Init()
{
	// UI画像の読み込み
	for (int i = 0; i < m_uiGraph.size(); i++) {
		m_uiGraph[i] = LoadGraph(kUI[i]);
		assert(m_uiGraph[i] != -1);
	}


	m_pSound->Init();	// サウンドの初期化
	m_pSound->LoadSE(SoundManager::SE_Type::kButtonSE);

	m_pFade = std::make_shared<Fade>();
}

shared_ptr<SceneBase> SceneRanking::Update()
{
	m_pFade->FadeIn(m_pFade->GatFadeInFlag());

	// 背景スクロール
	m_scrollX_1 += kCaptionScroll;

	m_pFade->FadeOut(m_pFade->GatFadeOutFlag());
	m_isNextSceneFlag = m_pFade->GatNextSceneFlag();

	if (Pad::IsTrigger(PAD_INPUT_A))		// RBボタン
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kButtonSE, DX_PLAYTYPE_BACK);
		m_pFade->SetFadeOutFlag(true);
	}

	if (m_isNextSceneFlag)
	{
		return make_shared<SceneTitle>();	// タイトルシーンへ行く
	}
#ifdef _DEBUG

#endif // DEBUG
	return shared_from_this();
}

void SceneRanking::Draw()
{
	DrawExtendGraph(0, 0, 1920, 1080, m_uiGraph[0], false);	// 背景
	DrawGraph(200, 400, m_uiGraph[1], true);

	DrawExplanation();

	m_pFade->Draw();

#ifdef _DEBUG
	DrawString(0, 0, "SceneRanking", 0xffffff); 
	DrawString(0, 20, "未実装", 0xffffff);
#endif // DEBUG
}

void SceneRanking::End()
{
	// UI画像の削除
	for (int i = 0; i < m_uiGraph.size(); i++) {
		DeleteGraph(m_uiGraph[i]);
	}

	SceneBase::End();
}

void SceneRanking::DrawExplanation()
{
	// 背景画像のサイズ取得
	Size expBgSize, expSize;
	GetGraphSize(m_uiGraph[2], &expBgSize.m_width, &expBgSize.m_height);
	GetGraphSize(m_uiGraph[3], &expSize.m_width, &expSize.m_height);

	// スクロール量を計算
	int scrollBg = static_cast<int>(m_scrollX_1) % static_cast<int>(expBgSize.m_width);

	// 説明文背景描画
	for (int index = 0; index < 4; index++)
	{
		DrawGraph(-scrollBg + index * expBgSize.m_width,
			static_cast<int>(kCaptionBgPosY),
			m_uiGraph[2], true);

		DrawGraph(-scrollBg + index * expSize.m_width,
			static_cast<int>(kCaptionPosY),
			m_uiGraph[3], true);
	}
}