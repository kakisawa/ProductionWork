﻿#include "Enemy.h"
#include "../LoadCsv.h"
#include <cassert>

namespace {
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// Vector値初期価値
	constexpr float kInitFloat = 0.0f;				// float値初期化

	const char* kModelFilePath = "Data/Model/EnemyModel.mv1";
}

Enemy::Enemy()
{
	// プレイヤー外部データ読み込み
	LoadCsv::GetInstance().LoadData(m_chara, "enemy");
	// モデルの読み込み
	m_model = MV1LoadModel(kModelFilePath);
	assert(m_model != -1);

	// 座標初期値
	m_pos = VGet(m_chara.initPosX, m_chara.initPosY, m_chara.initPosZ);
	MV1SetScale(m_model, VGet(m_chara.modelSize, m_chara.modelSize, m_chara.modelSize));

	m_status.situation.isMoving = false;
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	ModelBase::Init();

	// アニメーションの設定
	SetAnimation(static_cast<int>(EnemyAnim::Idle), true, false);
}

void Enemy::Update()
{
	ModelBase::Update();
}

void Enemy::Draw()
{
	ModelBase::Draw();
}

void Enemy::ChangeAnimNo(const EnemyAnim anim, const bool isAnimLoop, const int changeTime)
{
	m_status.animNo = static_cast<int>(anim);
	m_status.isLoop = isAnimLoop;
	ChangeAnimation(m_status.animNo, m_status.isLoop, false, changeTime);
}

void Enemy::ChangeAnimIdle()
{	
	// 待機アニメーションに変更する
	if (!m_status.situation.isMoving) {
		ChangeAnimNo(EnemyAnim::Idle, true, m_animChangeTime.Run);
	}
}