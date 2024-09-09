#include "SceneGameClear.h"
#include "SceneTitle.h"
#include "../Manager/SoundManager.h"
#include "../Util/Pad.h"
#include "../Util/Fade.h"

namespace {
	constexpr int kTitlePosX = 500;
	constexpr int kTitlePosY = 150;
}

SceneGameClear::SceneGameClear() :
	m_graph(-1),
	m_movie(-1)
{
	m_pFade = std::make_shared<Fade>();
}

SceneGameClear::~SceneGameClear()
{
}

void SceneGameClear::Init()
{
	m_graph = LoadGraph("data/Image/GameClearScene/UI/GameClear!!.png");
	m_pSound->Init();	// サウンドの初期化
	m_pSound->LoadBGM(SoundManager::BGM_Type::kGameClearBGM);
	m_pSound->PlayBGM(SoundManager::BGM_Type::kGameClearBGM, DX_PLAYTYPE_LOOP);

	m_movie=LoadGraph("data/Movie/Movie_004.mp4");
	PlayMovieToGraph(m_movie);

	//https://dova-s.jp/bgm/download18024.html
}

shared_ptr<SceneBase> SceneGameClear::Update()
{
	m_pFade->FadeIn(m_pFade->GatFadeInFlag());

	m_pFade->FadeOut(m_pFade->GatFadeOutFlag());
	m_isNextSceneFlag = m_pFade->GatNextSceneFlag();

	

	if (m_isNextSceneFlag)
	{
		return make_shared<SceneTitle>();	// タイトルシーンへ行く
	}

	if (Pad::IsTrigger(PAD_INPUT_A))		// Aボタン
	{
		m_pFade->SetFadeOutFlag(true);
		m_pSound->PlaySE(SoundManager::SE_Type::kButtonSE, DX_PLAYTYPE_BACK);
	}


#ifdef _DEBUG
#endif // DEBUG

	return shared_from_this();
}

void SceneGameClear::Draw()
{
	DrawGraph(0, 0, m_movie, false);
	DrawGraph(kTitlePosX, kTitlePosY, m_graph, true);
	m_pFade->Draw();
#ifdef _DEBUG
	DrawString(0, 0, "SceneGameClear", 0xffffff);
	DrawString(0, 20, "Please Press Button START", 0x00ffff);

	/*DrawExtendGraph(kTitlePosX, kTitlePosY,
		kTitlePosX + 1200, kTitlePosY + 430, m_graph, true);*/
#endif // DEBUG
}

void SceneGameClear::End()
{
	DeleteGraph(m_graph);
	SceneBase::End();
}
