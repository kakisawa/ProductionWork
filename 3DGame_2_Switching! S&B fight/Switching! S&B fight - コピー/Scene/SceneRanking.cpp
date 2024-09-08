#include "SceneRanking.h"
#include "SceneTitle.h"
#include "../Util/Pad.h"
#include "../Util/Fade.h"

SceneRanking::SceneRanking()
{
}

SceneRanking::~SceneRanking()
{
}

void SceneRanking::Init()
{
	m_pSound->Init();	// サウンドの初期化
	m_pSound->LoadSE(SoundManager::SE_Type::kButtonSE);

	m_pFade = std::make_shared<Fade>();
}

shared_ptr<SceneBase> SceneRanking::Update()
{
	m_pFade->FadeIn(m_pFade->GatFadeInFlag());

	m_pFade->FadeOut(m_pFade->GatFadeOutFlag());
	m_isNextSceneFlag = m_pFade->GatNextSceneFlag();

	if (Pad::IsTrigger(PAD_INPUT_R))		// RBボタン
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
	DrawString(0, 20, "未実装", 0xffffff);
	m_pFade->Draw();

#ifdef _DEBUG
	DrawString(0, 0, "SceneRanking", 0xffffff); 
#endif // DEBUG
}

void SceneRanking::End()
{
	SceneBase::End();
}
