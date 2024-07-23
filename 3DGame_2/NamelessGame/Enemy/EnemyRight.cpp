#include "EnemyRight.h"

namespace {
	const char* kModelEnemy = "data/model/EnemyModel/Enemy1.mv1";
}

EnemyRight::EnemyRight():
	EnemyBase(kModelEnemy)
{
	m_pos = VGet(10.0f, 0.0f, 10.0f);
	MV1SetPosition(m_model, m_pos);							// プレイヤーの初期位置	
}

EnemyRight::~EnemyRight()
{
}

void EnemyRight::Init()
{
}

void EnemyRight::Update()
{
}

void EnemyRight::Draw()
{
	// モデルの描画
	MV1DrawModel(m_model);
}

void EnemyRight::End()
{
}
