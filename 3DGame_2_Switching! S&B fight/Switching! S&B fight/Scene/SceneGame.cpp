#include "SceneGame.h"
#include "SceneGameClear.h"
#include "SceneGameOver.h"
#include "../Object/GameMap.h"
#include "../Object/Enemy/EnemyRight.h"
#include "../Object/Enemy/EnemyLeft.h"
#include "../Object/Player/Player.h"
#include "../Object/Camera.h"
#include "../Util/Pad.h"
#include "../Util/Fade.h"
#include <cassert>

namespace {
	constexpr int kExplanationInitPosX = 620;
	constexpr int kExplanationInitPosY = 130;

	constexpr int kExplanationPosX = 1472;
	constexpr int kExplanationPosY = 0;

	constexpr int kExplanationSizeX = 448;
	constexpr int kExplanationSizeY = 530;
}

SceneGame::SceneGame() :
	m_isGameClearFlag(false),
	m_isExplanationFinishFlag(false),
	m_isExplanationSet(false),
	m_explanationGraph(-1),
	m_waitCount(100)
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	m_pMap->Init();
	m_pPlayer->Init(m_pMap);
	m_pEnemyRight->Init(m_pMap);
	m_pEnemyLeft->Init(m_pMap);
	m_pCamera->Init();
	m_pFade = std::make_shared<Fade>();

	m_pSound->Init();	// サウンドの初期化

	m_pSound->LoadBGM(SoundManager::BGM_Type::kGameBGM);	// サウンドの読み込み
	m_pSound->PlayBGM(SoundManager::BGM_Type::kGameBGM, DX_PLAYTYPE_LOOP);

	m_explanationGraph = LoadGraph("data/Image/GameScene/UI/操作説明.png");
	size.m_width = kExplanationInitPosX;
	size.m_height = kExplanationInitPosY;
	size.m_widthMax = kExplanationInitPosX + 800;
	size.m_heightMax = kExplanationInitPosY + 800;
}

shared_ptr<SceneBase> SceneGame::Update()
{
	m_pFade->FadeIn(m_pFade->GatFadeInFlag());

	m_pCamera->Update(*m_pPlayer);

	if (m_isExplanationFinishFlag) {
		m_pFade->FadeOut(m_pFade->GatFadeOutFlag());
		m_isNextSceneFlag = m_pFade->GatNextSceneFlag();

		m_pPlayer->Update(*m_pCamera, *m_pEnemyRight, *m_pEnemyLeft);
		m_pEnemyRight->Update(*m_pPlayer);
		m_pEnemyLeft->Update(*m_pPlayer);
		

		m_isGameClearFlag = m_pEnemyLeft->GetHp() <= 0 && m_pEnemyRight->GetHp() <= 0;

		

		if (m_isGameClearFlag)
		{	
			m_waitCount--;
			if (m_waitCount <= 0) {
				m_pFade->SetFadeOutFlag(true);
				if (m_isNextSceneFlag)
				{
					return make_shared<SceneGameClear>();	// ゲームクリアへ行く
				}
			}
		}

		if (m_pPlayer->GetDeathFlag())
		{
			//End();
			m_pFade->SetFadeOutFlag(true);
			if (m_isNextSceneFlag)
			{
				return make_shared<SceneGameOver>();	// ゲームオーバーへ行く
			}
		}
	}
	else {	// ゲーム開始前処理

		if (Pad::IsTrigger(PAD_INPUT_A)) {
			m_isExplanationSet = true;
		}

		if (size.m_height <= 0 && size.m_width >= kExplanationPosX)
		{
			m_isExplanationFinishFlag = true;
		}

		if (m_isExplanationSet)
		{
			size.m_width= min(size.m_width, kExplanationPosX);
			size.m_widthMax= min(size.m_widthMax, 1920);
			size.m_height= max(size.m_height, 0);
			size.m_heightMax = max(size.m_heightMax, kExplanationSizeY);

			size.m_height-=9;
			size.m_heightMax-=9;
			size.m_width+=16;
			size.m_widthMax+=16;
		}
	}


#ifdef _DEBUG
	if (Pad::IsTrigger(PAD_INPUT_L))		// LBボタンを押したら
	{
		return make_shared<SceneGameClear>();	// ゲームクリアへ行く
	}
	if (Pad::IsTrigger(PAD_INPUT_R))		// RBボタンを押したら
	{
		return make_shared<SceneGameOver>();	// ゲームオーバーへ行く
	}

#endif // _DEBUG

	return shared_from_this();
}

void SceneGame::Draw()
{
	m_pMap->Draw();
	m_pEnemyRight->Draw();
	m_pEnemyLeft->Draw();
	m_pPlayer->Draw();

	m_pEnemyLeft->UIDraw();
	m_pEnemyRight->UIDraw();

	DrawExtendGraph(size.m_width, size.m_height,
		size.m_widthMax, size.m_heightMax,
		m_explanationGraph, true);

	m_pFade->Draw();

#ifdef _DEBUG
	DrawString(0, 120, "SceneGame", 0xffffff);
	DrawString(0, 140, "Please Press Button RB or LB", 0x00ffff);	
#endif
}

void SceneGame::End()
{
	m_pMap->End();
	m_pPlayer->End();
	m_pEnemyRight->End();
	m_pEnemyLeft->End();

	DeleteGraph(m_explanationGraph);
	SceneBase::End();
}

void SceneGame::GameClearDirection()
{

}
