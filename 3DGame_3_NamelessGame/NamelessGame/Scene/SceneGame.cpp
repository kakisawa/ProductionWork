#include "SceneGame.h"
#include "SceneDebug.h"
#include "SceneGameClear.h"
#include "SceneGameOver.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Item.h"
#include "../Object/Camera.h"
#include "../Object/Map.h"
#include "../UI/UISceneGame.h"
#include "../Time.h"
#include "DxLib.h"

using namespace MyInputInfo;

SceneGame::SceneGame():
	m_isPause(false)
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	m_pItem->Init();
	m_pCamera->Init();
	m_pPlayer->Init();
	m_pEnemy->Init();
	m_pMap->Init();
	m_pUI->Init(*m_pPlayer,*m_pEnemy);
	m_pTime->Init();


	m_pSound->InitBGM();
	m_pSound->LoadBGM(SoundManager::BGM_Type::kGameBGM);

	m_pSound->PlayBGM(SoundManager::BGM_Type::kGameBGM, DX_PLAYTYPE_LOOP);
}

std::shared_ptr<SceneBase> SceneGame::Update(Input& input)
{
	// スタートボタンを押したらポーズ状態にする
	if (input.IsTrigger(InputInfo::DebugStart)) {
		if (m_isPause) {
			m_isPause = false;
		}
		else {
			m_isPause = true;
		}
	}

	if (!m_isPause)
	{
		m_pMap->Update();
		m_pPlayer->Update(*m_pEnemy, *m_pItem, *m_pCamera, input);
		m_pEnemy->Update(*m_pMap, *m_pPlayer);
		m_pCamera->Update(*m_pPlayer);
		m_pItem->Update();
		m_pUI->Update(*m_pPlayer,*m_pEnemy);
		m_pTime->Update();

		// 敵が死亡したら
		if (m_pEnemy->GetDeathFlag()) {
			return std::make_shared<SceneGameClear>();	// ゲームクリアシーンへ行く
		}

		// プレイヤーが死亡したら・制限時間を過ぎてしまったら
		if (m_pPlayer->GetDeathFlag()||m_pTime->GetTimeUp()) {
			return std::make_shared<SceneGameOver>();	// ゲームオーバーシーンへ行く
		}
	}
	

#ifdef _DEBUG
	//if (input.IsTrigger(InputInfo::DebugStart)) {			// STARTボタン

	//	return std::make_shared<SceneDebug>();	// ゲームシーンへ行く
	//}
#endif // DEBUG

	return shared_from_this();
}

void SceneGame::Draw()
{
	m_pMap->Draw();
	m_pItem->Draw();
	m_pPlayer->Draw();
	m_pEnemy->Draw();
	m_pUI->Draw();
	m_pTime->Draw();

#ifdef _DEBUG
	DrawString(0, 0, "SceneGame", 0xffffff);
#endif // DEBUG
}

void SceneGame::End()
{
	m_pUI->End();
	m_pSound->ReleaseSound();
}
