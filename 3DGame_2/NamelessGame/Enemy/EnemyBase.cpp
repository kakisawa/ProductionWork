#include "EnemyBase.h"
#include "DxLib.h"

namespace {
	constexpr float kModelSize = 10.0f;
}

EnemyBase::EnemyBase(const char* model):
	m_model(-1),
	m_pos(VGet(0.0f,0.0f,0.0f))
{
	m_model = MV1LoadModel(model);	// ���f���̓ǂݍ���
	MV1SetScale(m_model, VGet(kModelSize, kModelSize, kModelSize));	// �G�̏����T�C�Y
	MV1SetRotationXYZ(m_model, VGet(0.0f, 0.0f, 0.0f));		// �v���C���[�̏����p�x
}

EnemyBase::~EnemyBase()
{
	MV1DeleteModel(m_model);
}

void EnemyBase::Init()
{
}

void EnemyBase::Update()
{
}

void EnemyBase::Draw()
{
}

void EnemyBase::End()
{
}
