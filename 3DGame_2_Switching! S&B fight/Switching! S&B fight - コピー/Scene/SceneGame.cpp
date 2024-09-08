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
}

SceneGame::SceneGame():
	m_pos(VGet(0.0f,0.0f,0.0f))
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

	m_pSound->InitSound();	// サウンドの初期化

	m_pSound->LoadBGM(SoundManager::BGM_Type::kGameBGM);	// サウンドの読み込み
	m_pSound->PlayBGM(SoundManager::BGM_Type::kGameBGM, DX_PLAYTYPE_LOOP);
}

shared_ptr<SceneBase> SceneGame::Update()
{
	m_pFade->FadeIn(m_pFade->GatFadeInFlag());

	m_pFade->FadeOut(m_pFade->GatFadeOutFlag());
	m_isNextSceneFlag = m_pFade->GatNextSceneFlag();

	m_pPlayer->Update(*m_pCamera,*m_pEnemyRight,*m_pEnemyLeft);
	m_pEnemyRight->Update(*m_pPlayer);
	m_pEnemyLeft->Update(*m_pPlayer);
	m_pCamera->Update(*m_pPlayer);

	if (m_pPlayer->GetDeathFlag())
	{
		//End();
		m_pFade->SetFadeOutFlag(true);
		if (m_isNextSceneFlag)
		{
			return make_shared<SceneGameOver>();	// ゲームオーバーへ行く
		}
	}

	if (m_pEnemyLeft->GetHp() <= 0 && m_pEnemyRight->GetHp() <= 0)
	{
		//End();
		m_pFade->SetFadeOutFlag(true);
		if (m_isNextSceneFlag)
		{
			return make_shared<SceneGameClear>();	// ゲームクリアへ行く
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

	SceneBase::End();
}