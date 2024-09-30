#include "SceneGame.h"
#include "../Player.h"
#include "../Camera.h"
#include "DxLib.h"

SceneGame::SceneGame()
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	m_pCamera->Init();
	m_pPlayer->Init();

	model = MV1LoadModel("Data/Model/book.mv1");
	MV1SetScale(model, VGet(50.0f, 50.0f, 50.0f));
	MV1SetPosition(model,VGet(0.0f, 0.0f, 0.0f));
}

std::shared_ptr<SceneBase> SceneGame::Update()
{
	
	m_pPlayer->Update(*m_pCamera);
	m_pCamera->Update(*m_pPlayer);

	return shared_from_this();
}

void SceneGame::Draw()
{
	MV1DrawModel(model);
	m_pPlayer->Draw();
}

void SceneGame::End()
{
	MV1DeleteModel(model);
}
