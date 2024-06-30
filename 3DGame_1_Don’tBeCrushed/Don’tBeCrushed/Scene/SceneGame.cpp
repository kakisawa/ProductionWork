#include "SceneGame.h"
#include "DxLib.h"
#include "../Enemy.h"
#include "../Player.h"
#include "../Stage.h"
#include "../Camera.h"
#include "../Pad.h"
#include "SceneGameClear.h"
#include "SceneGameOver.h"

namespace {
	constexpr static int kEnemyNum = 4;				// “G‚Ì”

	constexpr float kEnemyPlaceX = 35.0f;			// “GXÀ•W
	constexpr float kEnemyPlaceZ = 24.0f;			// “GZÀ•W

	constexpr float kEnemyRota = 1.58f;				// “G‰ñ“]—Ê
	constexpr float kEnemyAddScale = 50.0f;			// “G’Ç‰ÁƒTƒCƒY(’²®—p)
	constexpr float kEnemyMove = 0.05f;				// “GˆÚ“®—Ê
	constexpr float kEnemyAttackInterval = 240.0f;	// “GUŒ‚‚ÌŠÔŠu

	bool col = false;								// “G‚Æ‚Ô‚Â‚©‚Á‚Ä‚¢‚é‚©
}

SceneGame::SceneGame() :
	m_enemyNum(kEnemyNum),
	m_enemyInterval(0),
	m_enemyAttckNum(0),
	m_enemyAttackPos(VGet(0.0f, 0.0f, 0.0f))
{
	m_pEnemy.resize(m_enemyNum);

	m_pEnemy[0] = make_shared<Enemy>(VGet(1.0f, 1.0f, kEnemyPlaceZ));	// ‰œ
	m_pEnemy[1] = make_shared<Enemy>(VGet(1.0f, 1.0f, -kEnemyPlaceZ));	// ‘O
	m_pEnemy[2] = make_shared<Enemy>(VGet(-kEnemyPlaceX, 1.0f, 2.0f));	// ¶
	m_pEnemy[3] = make_shared<Enemy>(VGet(kEnemyPlaceX, 1.0f, 2.0f));	// ‰E

	// “Gƒ‚ƒfƒ‹‰ñ“]
	m_pEnemy[0]->SetModelAngle(VGet(0.0f, 0.0f, kEnemyRota));	// ‰œ
	m_pEnemy[1]->SetModelAngle(VGet(0.0f, 0.0f, kEnemyRota));	// ‘O
	m_pEnemy[2]->SetModelAngle(VGet(kEnemyRota, 0.0f, kEnemyRota));	// ¶
	m_pEnemy[3]->SetModelAngle(VGet(kEnemyRota, 0.0f, kEnemyRota));	// ‰E

	m_pEnemy[0]->SetAddModelScale(kEnemyAddScale);
	m_pEnemy[1]->SetAddModelScale(kEnemyAddScale);

	m_pEnemy[2]->SetMove(VGet(0, 0, kEnemyMove));
	m_pEnemy[3]->SetMove(VGet(0, 0, kEnemyMove));

	m_pEnemy[2]->SetAddMove(true);
	m_pEnemy[3]->SetAddMove(true);
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	m_pPlayer->Init(m_pStage);

	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		m_pEnemy[i]->Init();
	}

	m_pStage->Init();
}

shared_ptr<SceneBase> SceneGame::Update()
{
	Pad::Update();

	m_enemyInterval++;
	if (m_enemyInterval >= kEnemyAttackInterval)
	{	
		m_enemyAttckNum= GetRand(3);
		m_pEnemy[m_enemyAttckNum]->SetAttackNum(m_enemyAttckNum);
		m_pEnemy[m_enemyAttckNum]->SetAttack(true);
		if (m_enemyAttckNum < 2)	// 0or1
		{
			int flag = false;
			m_enemyAttackPos = m_pEnemy[m_enemyAttckNum]->GetPos();

			if (m_enemyAttckNum == 0) {
				flag = false;
			}
			else if (m_enemyAttckNum == 1) {
				flag = true;
			}

			m_colRect.SetLandscape(m_enemyAttackPos, 5.2f, 30.0f, 19.0f, flag);
		}

		if (m_enemyAttckNum > 1)
		{
			int flag = false;
			m_enemyAttackPos = m_pEnemy[m_enemyAttckNum]->GetPos();
			
			if (m_enemyAttckNum == 2) {
				flag = false;
			}
			else if (m_enemyAttckNum == 3) {
				flag = true;
			}

			m_colRect.SetPortrait(m_enemyAttackPos, 5.2f, 30.0f, 19.0f, flag);
		}
		m_enemyInterval = 0;
	}

	if (m_pEnemy[m_enemyAttckNum]->GetFall())
	{
		// “–‚½‚è”»’èˆ—
		Collision playerRect = m_pPlayer->GetColRect();
		Collision enemyRect = m_colRect;
		if (playerRect.IsCollision(m_enemyAttckNum, m_colRect))
		{
			col = true;
		}
	}
	else {
		col = false;
	}
	
	m_pPlayer->Update();

	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		m_pEnemy[i]->Update();
	}

	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		return make_shared<SceneGameClear>();
	}
	if (Pad::IsTrigger(PAD_INPUT_2))
	{
		return make_shared<SceneGameOver>();
	}

	return shared_ptr<SceneBase>();
}

void SceneGame::Draw()
{
	m_pStage->Draw();

	if (m_pEnemy[m_enemyAttckNum]->GetAttack()){
		m_colRect.EnemyDraw(0x000000, true);
	}
	
	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		m_pEnemy[i]->Draw();
	}
	m_pPlayer->Draw();

	DrawString(0, 0, "SceneGame", 0xffffff);

#ifdef DEBUG
	DrawFormatString(0, 0, 0xffffff, "m_enemyAttckNum=%d", m_enemyAttckNum);
	DrawFormatString(0, 70, 0xffffff, "m_enemyInterval=%d", m_enemyInterval);
	DrawFormatString(0, 200, 0xffffff,"ifPos.x=%.2f:z=%.2f", m_enemyAttackPos.x, m_enemyAttackPos.z);
	DrawFormatString(0, 360, 0xffffff, "col=%d", col);
#endif // DEBUG

}

void SceneGame::End()
{
	m_pPlayer->End();

	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		m_pEnemy[i]->End();
	}
}

void SceneGame::EnemyCol()
{


}
