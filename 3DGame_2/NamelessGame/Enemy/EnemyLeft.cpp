#include "EnemyLeft.h"

namespace {
	const char* kModelEnemy = "data/model/EnemyModel/Enemy2.mv1";
}

EnemyLeft::EnemyLeft():
	EnemyBase(kModelEnemy)
{
	m_pos = VGet(-10.0f, 0.0f, 10.0f);
	MV1SetPosition(m_model, m_pos);							// プレイヤーの初期位置	
}

EnemyLeft::~EnemyLeft()
{
}

void EnemyLeft::Init()
{
}

void EnemyLeft::Update()
{
}

void EnemyLeft::Draw()
{
	// モデルの描画
	MV1DrawModel(m_model);
}

void EnemyLeft::End()
{
}
