#include "Enemy.h"
#include "Player.h"
#include "../Object/Map.h"
#include "../LoadCsv.h"
#include <cassert>
#include <cmath>
#include <algorithm>

namespace {
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// Vector値初期価値
	constexpr float kInitFloat = 0.0f;				// float値初期化

	const VECTOR kBodyColUpPos = VGet(0.0f, 70.0f, 0.0f);	// 体当たり判定頂点

	constexpr int kAttackHandArm = 30;			// 右腕攻撃力
	constexpr int kAttackMachineArm = 30;			// 左腕攻撃力

	const char* kModelFilePath = "Data/Model/EnemyModel.mv1";

	bool isMove = true;
}

Enemy::Enemy():
	m_targetDistance(0.0f),
	m_targetMoveDistance(0.0f),
	m_colPos(kInitVec),
	m_startPos(kInitVec),
	m_targetPos(kInitVec),
	m_isNextTargetPosSearch(true)
{
	// プレイヤー外部データ読み込み
	LoadCsv::GetInstance().LoadCommonFileData(m_chara, "enemy");
	// モデルの読み込み
	m_model = MV1LoadModel(kModelFilePath);
	assert(m_model != -1);

	m_hp = m_chara.maxHp;

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
	m_hp = m_chara.maxHp;		// HPに最大値を入れる
	m_attack = kAttackHandArm;



	// アニメーションの設定
	SetAnimation(static_cast<int>(EnemyAnim::Idle), m_animSpeed.Idle,true, false);
}

void Enemy::Update(const Map& map, const Player& player)
{
	ModelBase::Update();

	// 当たり判定用座標調整
	m_colPos = VAdd(m_pos, kBodyColUpPos);
	// 体当たり判定更新
	m_col.TypeChangeCapsuleUpdate(m_col.m_body, m_pos, m_colPos, m_chara.bodyColRad);


	SearchNearPosition(map);
	Move(map);

	m_hp -= player.GetAttack();

	// 死亡時処理
	if (m_hp <= 0) {
		m_deathFlag = true;
	}


	ChangeAnimIdle();
}

void Enemy::Draw()
{
	ModelBase::Draw();

#ifdef _DEBUG
	m_col.TypeChangeCapsuleDraw(m_col.m_body, 0x0000ff, false);

	DrawFormatString(0, 80, 0xffffff, "Enemy: HP=%d", m_hp);

	DrawFormatString(0, 780, 0xffffff, "Enemy:m_pos.x=%.2f:z=%.2f", m_pos.x, m_pos.z);
	DrawFormatString(0, 800, 0xffffff, "m_targetMoveDistance=%.2f", m_targetMoveDistance);
	DrawFormatString(0, 820, 0xffffff, "m_targetDistance=%.2f", m_targetDistance);
	DrawFormatString(0, 840, 0xffffff, "m_isNextTargetPosSearch=%d", m_isNextTargetPosSearch);
	DrawFormatString(0, 860, 0xffffff, "m_startPos.x=%.2f:z=%.2f", m_startPos.x, m_startPos.z);
	DrawFormatString(0, 880, 0xffffff, "m_targetPos.x=%.2f:z=%.2f", m_targetPos.x, m_targetPos.z);
	DrawFormatString(0, 900, 0xffffff, "m_move.x=%.2f:z=%.2f", m_move.x, m_move.z);
	
#endif // DEBUG
}

void Enemy::Move(const Map& map)
{
	m_move = kInitVec;
	
	if (isMove)
	{
		// Pos1からPos2
		/*if (m_pos.x != map.GetPointPos().point2.x) {
			m_move.x = m_chara.walkSpeed;
		}
		else if (m_pos.z != map.GetPointPos().point2.z)
		{
			m_move.z = m_chara.walkSpeed;
		}*/



		//if(m_targetPos)



		// Pos2からPos3


		// Pos3からPos4


		// Pos4からPos1


		if (m_pos.x != m_targetPos.x) {
			if (m_pos.x >= m_targetPos.x)
			{
				m_move.x = -m_chara.walkSpeed;
			}
			else if (m_pos.x <= m_targetPos.x)
			{
				m_move.x = +m_chara.walkSpeed;
			}

			
		}
		else if (m_pos.z != m_targetPos.z)
		{
			if (m_pos.z >= m_targetPos.z)
			{
				m_move.z = -m_chara.walkSpeed;
			}
			else if (m_pos.z <= m_targetPos.z)
			{
				m_move.z = +m_chara.walkSpeed;
			}
		}



	}

	// 正規化と移動速度の適用
	if (VSize(m_move) > 0.0f) {
		m_move = VNorm(m_move); // 正規化
		m_targetDir = m_move;  // 目標方向を保存
		m_move = VScale(m_move, m_chara.walkSpeed); // 移動速度を適用
	}

	m_pos = VAdd(m_pos, m_move);

	

	// 移動処理の更新
	MoveUpdate();
}

void Enemy::MoveUpdate()
{
	// 移動値を入れる
	float movingSpeed = std::max(abs(m_move.x), abs(m_move.z));

	// プレイヤーの移動状態を初期化する
	m_status.situation.isMoving = false;

	// 移動値があった場合
	if (movingSpeed != 0.0f) {

		// プレイヤーの移動状態をtrueにする
		m_status.situation.isMoving = true;

		// プレイヤーが移動している時のみ、移動アニメーションを入れる
		//if (m_status.situation.isMoving)
		{
			ChangeAnimNo(EnemyAnim::Walk, m_animSpeed.Walk, true, m_animChangeTime.Walk);
		}
	}
}

void Enemy::SearchNearPosition(const Map& map)
{

	if (m_isNextTargetPosSearch)
	{
		m_isNextTargetPosSearch = false;

		m_startPos = m_pos;

		VECTOR target1;
		VECTOR target2;
		VECTOR target3;
		VECTOR target4;

		target1 = VSub(map.GetPointPos().point1, m_pos);
		target2 = VSub(map.GetPointPos().point2, m_pos);
		target3 = VSub(map.GetPointPos().point3, m_pos);
		target4 = VSub(map.GetPointPos().point4, m_pos);

		float target1_1;
		float target2_1;
		float target3_1;
		float target4_1;

		target1_1 = abs(target1.x) + abs(target1.z);
		target2_1 = abs(target2.x) + abs(target2.z);
		target3_1 = abs(target3.x) + abs(target3.z);
		target4_1 = abs(target4.x) + abs(target4.z);

			

	

		m_targetDistance = std::min({ target1_1,target2_1,target3_1,target4_1 });
		
		

		if (m_targetDistance == target1_1)
		{
			m_targetPos = map.GetPointPos().point1;
		}
		else if (m_targetDistance == target2_1)
		{
			m_targetPos = map.GetPointPos().point2;
		}
		else if (m_targetDistance == target3_1)
		{
			m_targetPos = map.GetPointPos().point3;
		}
		else if (m_targetDistance == target4_1)
		{
			m_targetPos = map.GetPointPos().point4;
		}
	}

	if (m_targetMoveDistance >= abs(m_targetDistance))
	{
		m_isNextTargetPosSearch = true;
	}

	m_targetMoveDistance = abs( (m_startPos.x + m_startPos.z)- (m_pos.x + m_pos.z));
}


void Enemy::ChangeAnimNo(const EnemyAnim anim, const float animSpeed, const bool isAnimLoop, const int changeTime)
{
	m_status.animNo = static_cast<int>(anim);
	m_status.animSpeed = animSpeed;
	m_status.isLoop = isAnimLoop;
	ChangeAnimation(m_status.animNo, animSpeed, m_status.isLoop, false, changeTime);
}

void Enemy::ChangeAnimIdle()
{
	// 待機アニメーションに変更する
	if (!m_status.situation.isMoving) {
		ChangeAnimNo(EnemyAnim::Idle, m_animSpeed.Idle, true, m_animChangeTime.Idle);
	}
}
