#include "Enemy.h"
#include "../Object/Map.h"
#include "../LoadCsv.h"
#include <cassert>
#include <cmath>

namespace {
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// Vector値初期価値
	constexpr float kInitFloat = 0.0f;				// float値初期化

	float kMove = 1.0f;
	constexpr int kMaxHp = 1000;				// 最大HP
	constexpr int kAttackRightArm = 30;			// 右腕攻撃力
	constexpr int kAttackLeftArm = 30;			// 左腕攻撃力

	const char* kModelFilePath = "Data/Model/EnemyModel.mv1";


	bool isMove = true;
}

Enemy::Enemy():
	m_targetPos(kInitVec)
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
	m_hp = kMaxHp;		// HPに最大値を入れる
	m_attack = kAttackRightArm;



	// アニメーションの設定
	SetAnimation(static_cast<int>(EnemyAnim::Idle), m_animSpeed.Idle,true, false);
}

void Enemy::Update(const Map& map)
{
	ModelBase::Update();

	SearchNearPosition(map);

	Move(map);
}

void Enemy::Draw()
{
	ModelBase::Draw();
}

void Enemy::Move(const Map& map)
{
	m_move = kInitVec;
	
	if (isMove)
	{
		// Pos1からPos2
		if (m_pos.x != map.GetPointPos().point2.x) {
			m_move.x = kMove;
		}
		else if (m_pos.z != map.GetPointPos().point2.z)
		{
			m_move.z = kMove;
		}





		// Pos2からPos3


		// Pos3からPos4


		// Pos4からPos1

	}

	
	m_pos = VAdd(m_pos, m_move);
}

void Enemy::SearchNearPosition(const Map& map)
{

	//VECTOR target1;
	//VECTOR target2;
	//VECTOR target3;
	//VECTOR target4;

	//target1 = VSub(m_pos, map.GetPointPos().point1);
	//target2 = VSub(m_pos, map.GetPointPos().point2);
	//target3 = VSub(m_pos, map.GetPointPos().point3);
	//target4 = VSub(m_pos, map.GetPointPos().point4);

	//m_targetPos = std::min(target1, target2);
	//m_targetPos = std::min(m_targetPos, target3);
	//m_targetPos = std::min(m_targetPos, target4);
}
