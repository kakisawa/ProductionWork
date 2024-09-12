#include "EnemyBase.h"
#include "../Player/Player.h"
#include "../Model.h"
#include "../GameMap.h"
#include "DxLib.h"

namespace {
	constexpr float kModelSize = 10.0f;
	constexpr int kHPMax = 300;

	const VECTOR kInit = VGet(0.0f, 0.0f, 0.0f);
}

EnemyBase::EnemyBase(const char* model, VECTOR pos):
	m_model(-1),
	m_hp(kHPMax),
	m_pos(pos),
	m_isAlive(false)
{
	//モデルインスタンス作成
	m_pModel = std::make_shared<Model>(model);

	// プレイヤー初期設定
	m_pModel->SetSize(VGet(kModelSize, kModelSize, kModelSize));
	m_pModel->SetRota(VGet(0.0f, 0.0f, 0.0f));
	m_pModel->SetPos(pos);
}

EnemyBase::~EnemyBase()
{
	MV1DeleteModel(m_model);
}

void EnemyBase::Init(std::shared_ptr<GameMap> pMap)
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