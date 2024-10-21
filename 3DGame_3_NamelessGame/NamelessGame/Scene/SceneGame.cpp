#include "SceneGame.h"
#include "SceneDebug.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Item.h"
#include "../Object/Camera.h"
#include "../Object/Map.h"
#include "DxLib.h"

SceneGame::SceneGame() :
	model(-1)
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
	

	model = MV1LoadModel("Data/Model/book.mv1");
	MV1SetScale(model, VGet(50.0f, 50.0f, 50.0f));
	MV1SetPosition(model,VGet(0.0f, 0.0f, 0.0f));
}

std::shared_ptr<SceneBase> SceneGame::Update(Input& input)
{
	m_pMap->Update();
	m_pPlayer->Update(*m_pEnemy, *m_pItem, *m_pCamera, input);
	m_pEnemy->Update(*m_pMap);
	m_pCamera->Update(*m_pPlayer);
	m_pItem->Update();

#ifdef _DEBUG
	if (input.IsTrigger(InputInfo::DebugStart)) {			// STARTボタン

		return std::make_shared<SceneDebug>();	// ゲームシーンへ行く
	}
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

#ifdef _DEBUG
	DrawString(0, 0, "SceneGame", 0xffffff);
#endif // DEBUG
}

void SceneGame::End()
{
	MV1DeleteModel(model);
}
