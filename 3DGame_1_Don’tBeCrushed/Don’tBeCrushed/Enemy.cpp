#include "DxLib.h"
#include "Enemy.h"


namespace {
	const char* const kModelEnemy = "data/model/book.mv1";
}

Enemy::Enemy()
{
	EnemyModelBase = MV1LoadModel(kModelEnemy);
	EnemyScale = 50.0f;
	EnemyModel[m_enemyNum] = { 0,0,0,0 };
	for (int i = 0; i < m_enemyNum; i++)
	{
		EnemyModel[i] = MV1DuplicateModel(EnemyModelBase);
		MV1SetScale(EnemyModel[i], VGet(EnemyScale, EnemyScale + 50, EnemyScale));
	}
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	// À•WÝ’è
	m_pos[0] = VGet(1.0f, 1.0f, 24.0f);	// ‰œ
	m_pos[1] = VGet(1.0f, 1.0f, -20.0f);	// ‘O
	m_pos[2] = VGet(-30.0f, 1.0f, 2.0f);	// ‰E
	m_pos[3] = VGet(35.0f, 1.0f, 2.0f);	// ¶

	// “Gƒ‚ƒfƒ‹‰ñ“]
	MV1SetRotationXYZ(EnemyModel[0], VGet(0.0f, 0.0f, 1.58f));
	MV1SetRotationXYZ(EnemyModel[1], VGet(0.0f, 0.0f, 1.58f));
	MV1SetRotationXYZ(EnemyModel[2], VGet(1.58f, 0.0f, 1.58f));
	MV1SetRotationXYZ(EnemyModel[3], VGet(1.58f, 0.0f, 1.58f));
	for (int i = 0; i < m_enemyNum; i++)
	{
		MV1SetPosition(EnemyModel[i], m_pos[i]);
	}
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
	for (int i = 0; i < m_enemyNum; i++)
	{
		MV1DrawModel(EnemyModel[i]);
	}

}

void Enemy::End()
{
	MV1DeleteModel(EnemyModelBase);
	EnemyModelBase = -1;

}
