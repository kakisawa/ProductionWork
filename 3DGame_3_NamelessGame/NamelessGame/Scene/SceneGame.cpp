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
}

std::shared_ptr<SceneBase> SceneGame::Update()
{
	m_pCamera->Update(*m_pPlayer);
	m_pPlayer->Update();

	return shared_from_this();
}

void SceneGame::Draw()
{
	m_pPlayer->Draw();
}

void SceneGame::End()
{
}
