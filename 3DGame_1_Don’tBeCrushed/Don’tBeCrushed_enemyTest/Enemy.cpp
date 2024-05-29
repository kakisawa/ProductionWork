#include "DxLib.h"
#include "Enemy.h"

namespace {
	const char* const kModelEnemy = "data/model/book.mv1";

	VECTOR testPos = VGet(35.0f, 1.0f, 2.0f);
}

Enemy::Enemy(VECTOR pos) :
	m_model(-1),
	m_scale(50),
	m_addScale(0),
	m_isAddMove(false),
	m_pos(pos),
	m_angle(VGet(0, 0, 0)),
	m_move(VGet(0, 0, 0))
{
	m_modelBase = MV1LoadModel(kModelEnemy);	// �G���[�h
	m_model = MV1DuplicateModel(m_modelBase);	// �G�R�s�[
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	// �G���f����]
	MV1SetRotationXYZ(m_model, m_angle);
	// �G���f�����W
	MV1SetPosition(m_model, m_pos);
	// �G���f���T�C�Y����
	MV1SetScale(m_model, VGet(m_scale, m_scale + m_addScale, m_scale));
}

void Enemy::Update()
{
	// �e�X�g�p
	if (m_isAddMove == true)
	{
		if (testPos.z >= 16.0f)
		{
			testPos=VScale(testPos, -1.0f);
		}

		if (testPos.z <= -21.0f)
		{
			testPos=VScale(testPos, -1.0f);
		}
		testPos = VAdd(testPos, m_move);
	}



	// �G�ړ�
	if (m_isAddMove == true)
	{
		if (m_pos.z >= 11.2f)
		{
			m_pos = VScale(m_pos, -1.0f);
		}
		m_pos = VAdd(m_pos, m_move);
	}

	MV1SetPosition(m_model, m_pos);
}

void Enemy::Draw()
{
	MV1DrawModel(m_model);
	DrawFormatString(0, 0, 0xffffff, "x=%.2f,y=%.2f,z=%.2f", testPos.x, testPos.y, testPos.z);
	DrawFormatString(0, 15, 0xffffff, "isAddMove=%d", m_isAddMove);
}

void Enemy::End()
{
	MV1DeleteModel(m_modelBase);
	m_modelBase = -1;
}
