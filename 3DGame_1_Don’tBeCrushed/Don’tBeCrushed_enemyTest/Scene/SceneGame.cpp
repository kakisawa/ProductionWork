#include "SceneGame.h"
#include "DxLib.h"
#include "../Enemy.h"
#include "../Player.h"
#include "../Stage.h"

namespace {
	constexpr static int kEnemyNum = 4;	// “G‚Ì”

	constexpr float kEnemyPlaceX = 35.0f;	// “GXÀ•W
	constexpr float kEnemyPlaceZ = 24.0f;	// “GZÀ•W

	constexpr float kEnemyRota = 1.58f;		// “G‰ñ“]—Ê
	constexpr float kEnemyAddScale = 50.0f;	// “G’Ç‰ÁƒTƒCƒY(’²®—p)
	constexpr float kEnemyMove = 0.05f;		// “GˆÚ“®—Ê
	constexpr float kEnemyAttackInterval = 180.0f;	// “GUŒ‚‚ÌŠÔŠu
}

SceneGame::SceneGame():
	m_enemyNum(kEnemyNum),
	m_enemyInterval(0)
{
	m_pEnemy.resize(m_enemyNum);

	m_pEnemy[0] = make_shared<Enemy>(VGet(1.0f, 1.0f, kEnemyPlaceZ));	// ‰œ
	m_pEnemy[1] = make_shared<Enemy>(VGet(1.0f, 1.0f, -kEnemyPlaceZ));	// ‘O
	m_pEnemy[2] = make_shared<Enemy>(VGet(-kEnemyPlaceX, 1.0f, 2.0f));	// ¶
	m_pEnemy[3] = make_shared<Enemy>(VGet(kEnemyPlaceX, 1.0f, 2.0f));	// ‰E

	// “Gƒ‚ƒfƒ‹‰ñ“]
	m_pEnemy[0]->GetModelAngle(VGet(0.0f, 0.0f, kEnemyRota));	// ‰œ
	m_pEnemy[1]->GetModelAngle(VGet(0.0f, 0.0f, kEnemyRota));	// ‘O
	m_pEnemy[2]->GetModelAngle(VGet(kEnemyRota, 0.0f, kEnemyRota));	// ¶
	m_pEnemy[3]->GetModelAngle(VGet(kEnemyRota, 0.0f, kEnemyRota));	// ‰E

	m_pEnemy[0]->GetAddModelScale(kEnemyAddScale);
	m_pEnemy[1]->GetAddModelScale(kEnemyAddScale);

	m_pEnemy[2]->GetMove(VGet(0, 0, kEnemyMove));
	m_pEnemy[3]->GetMove(VGet(0, 0, kEnemyMove));

	m_pEnemy[2]->GetAddMove(true);
	m_pEnemy[3]->GetAddMove(true);
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	m_pPlayer->Init();

	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		m_pEnemy[i]->Init();
	}

	m_pStage->Init();
}

shared_ptr<SceneBase> SceneGame::Update()
{
	m_enemyInterval++;
	if (m_enemyInterval >= kEnemyAttackInterval)
	{
		m_pEnemy[GetRand(3)]->GetAttack(true);
		m_enemyInterval = 0;
	}

	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		m_pEnemy[i]->Update();
	}

	m_pPlayer->Update();



	return shared_ptr<SceneBase>();
}

void SceneGame::Draw()
{
	m_pStage->Draw();

	m_pPlayer->Draw();

	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		m_pEnemy[i]->Draw();
	}
	DrawFormatString(0, 70, 0xffffff, "m_enemyInterval=%d", m_enemyInterval);
}

void SceneGame::End()
{
	m_pPlayer->End();

	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		m_pEnemy[i]->End();
	}
}
