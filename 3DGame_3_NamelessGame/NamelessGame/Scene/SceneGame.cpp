#include "SceneGame.h"
#include "SceneDebug.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Camera.h"
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
	m_pCamera->Init();
	m_pPlayer->Init();
	m_pEnemy->Init();

	model = MV1LoadModel("Data/Model/book.mv1");
	MV1SetScale(model, VGet(50.0f, 50.0f, 50.0f));
	MV1SetPosition(model,VGet(0.0f, 0.0f, 0.0f));
}

std::shared_ptr<SceneBase> SceneGame::Update()
{
	
	m_pPlayer->Update(*m_pCamera);
	m_pEnemy->Update();
	m_pCamera->Update(*m_pPlayer);

#ifdef _DEBUG
	if (input.IsTrigger(InputInfo::DebugStart)) {			// STARTボタン

		return std::make_shared<SceneDebug>();	// ゲームシーンへ行く
	}
#endif // DEBUG

	return shared_from_this();
}

void SceneGame::Draw()
{
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
