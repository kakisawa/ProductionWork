#include "SceneGame.h"
#include "DxLib.h"
#include "../Enemy.h"

SceneGame::SceneGame()
{
	m_enemyNum = 4;

	m_pEnemy.resize(m_enemyNum);

	m_pEnemy[0] = make_shared<Enemy>(VGet(1.0f, 1.0f, 24.0f));	// 奥
	m_pEnemy[1] = make_shared<Enemy>(VGet(1.0f, 1.0f, -20.0f));	// 前
	m_pEnemy[2] = make_shared<Enemy>(VGet(-35.0f, 1.0f, 2.0f));	// 左
	m_pEnemy[3] = make_shared<Enemy>(VGet(35.0f, 1.0f, 2.0f));	// 右

	// 敵モデル回転
	m_pEnemy[0]->GetModelAngle(VGet(0.0f, 0.0f, 1.58f));	// 奥
	m_pEnemy[1]->GetModelAngle(VGet(0.0f, 0.0f, 1.58f));	// 前
	m_pEnemy[2]->GetModelAngle(VGet(1.58f, 0.0f, 1.58f));	// 左
	m_pEnemy[3]->GetModelAngle(VGet(1.58f, 0.0f, 1.58f));	// 右

	m_pEnemy[0]->GetAddModelScale(50);
	m_pEnemy[1]->GetAddModelScale(50);
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		m_pEnemy[i]->Init();
	}
}

shared_ptr<SceneBase> SceneGame::Update()
{
	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		m_pEnemy[i]->Update();
	}
	return shared_ptr<SceneBase>();
}

void SceneGame::Draw()
{
	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		m_pEnemy[i]->Draw();
	}
}

void SceneGame::End()
{
	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		m_pEnemy[i]->End();
	}
}
