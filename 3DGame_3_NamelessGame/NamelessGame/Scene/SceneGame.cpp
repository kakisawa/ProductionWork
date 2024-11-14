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
#include "DxLib.h"



SceneGame::SceneGame() :
	model(-1),
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
	m_pUI->Init();

	model = MV1LoadModel("Data/Model/book.mv1");
	MV1SetScale(model, VGet(50.0f, 50.0f, 50.0f));
	MV1SetPosition(model,VGet(0.0f, 0.0f, 0.0f));


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
		m_pUI->Update(*m_pPlayer);

		// 敵が死亡したら
		if (m_pEnemy->GetDeathFlag()) {
			return std::make_shared<SceneGameClear>();	// ゲームクリアへ行く
		}

		// プレイヤーが死亡したら
		if (m_pPlayer->GetDeathFlag()) {
			return std::make_shared<SceneGameOver>();
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
	MV1DrawModel(model);
	m_pPlayer->Draw();
	m_pEnemy->Draw();
	m_pUI->Draw();

#ifdef _DEBUG
	DrawString(0, 0, "SceneGame", 0xffffff);
#endif // DEBUG
}

void SceneGame::End()
{
	MV1DeleteModel(model);
	
	m_pUI->End();
}
