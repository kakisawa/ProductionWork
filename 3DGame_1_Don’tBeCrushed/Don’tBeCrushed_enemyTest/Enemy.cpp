#include "DxLib.h"
#include "Enemy.h"


namespace {
	const char* const kModelEnemy = "data/model/book.mv1";
}

Enemy::Enemy(VECTOR pos) :
	m_model(-1),
	m_scale(50),
	m_addScale(0),
	m_pos(pos),
	m_angle(VGet(0, 0, 0))
{
	m_modelBase = MV1LoadModel(kModelEnemy);

	m_model = MV1DuplicateModel(m_modelBase);
	
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	// ìGÉÇÉfÉãâÒì]
	MV1SetRotationXYZ(m_model, m_angle);
	// ìGÉÇÉfÉãç¿ïW
	MV1SetPosition(m_model, m_pos);

	MV1SetScale(m_model, VGet(m_scale, m_scale + m_addScale, m_scale));
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
	MV1DrawModel(m_model);
}

void Enemy::End()
{
	MV1DeleteModel(m_modelBase);
	m_modelBase = -1;
}
