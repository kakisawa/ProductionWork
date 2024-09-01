#include "EnemyBase.h"
#include "../Player/Player.h"
#include "../Model.h"
#include "DxLib.h"

namespace {
	constexpr float kModelSize = 10.0f;
	constexpr int kHPMax = 100;
}

EnemyBase::EnemyBase(const char* model, VECTOR pos):
	m_model(-1),
	m_hp(kHPMax),
	m_pos(VGet(0.0f,0.0f,0.0f))
{
	//���f���C���X�^���X�쐬
	m_pModel = std::make_shared<Model>(model);

	// �v���C���[�����ݒ�
	m_pModel->SetSize(VGet(kModelSize, kModelSize, kModelSize));
	m_pModel->SetRota(VGet(0.0f, 0.0f, 0.0f));
	m_pModel->SetPos(pos);
}

EnemyBase::~EnemyBase()
{
	MV1DeleteModel(m_model);
}

void EnemyBase::Init()
{

}

void EnemyBase::Update(const Player& player)
{
}

void EnemyBase::Draw()
{
}

void EnemyBase::End()
{
}
