#include "DxLib.h"
#include "Enemy.h"

namespace {
	const char* const kModelEnemy = "data/model/book.mv1";
	
	int jumpCount = 2;					// ジャンプ回数
	constexpr float kMove = 0.05f;		// 敵移動量
	constexpr float kGravity = 0.18f;	// 重力
	constexpr float kJumpPow = 0.5f;	// ジャンプ力

	constexpr float kRota = 0.05f;		// 回転量
	constexpr float kRotaMaxY = 1.5f;		// Y軸最大回転量
	constexpr float kRotaMaxZ = 1.7f;		// Z軸最大回転量

	constexpr float kEnemyRota = 1.58f;		// 敵回転量(SceneGameにもあり)
}

Enemy::Enemy(VECTOR pos) :
	m_model(-1),
	m_scale(50),
	m_addScale(0),
	maxRota(0.0f),
	m_gravity(kGravity),
	m_attackNum(0),
	m_isAddMove(false),
	m_isAttack(false),
	m_isFall(false),
	m_isMoveStop(false),
	m_pos(pos),
	m_angle(VGet(0, 0, 0)),
	m_move(VGet(0, 0, 0))
{
	m_modelBase = MV1LoadModel(kModelEnemy);	// 敵ロード
	m_model = MV1DuplicateModel(m_modelBase);	// 敵コピー
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	// 敵モデル回転
	MV1SetRotationXYZ(m_model, m_angle);
	// 敵モデル座標
	MV1SetPosition(m_model, m_pos);
	// 敵モデルサイズ調整
	MV1SetScale(m_model, VGet(m_scale, m_scale + m_addScale, m_scale));
}

void Enemy::Update()
{

	// 敵攻撃前
	if (m_isAttack == true) {

		m_isMoveStop = true;	// 敵の移動を止める
		

		if (jumpCount > 0)		// ジャンプの残り回数が0以上なら処理を行う
		{
			

			m_move = VGet(0, 0, 0);
			m_move = VAdd(m_move, VGet(0, kJumpPow, 0));
			m_pos = VAdd(m_pos, m_move);

			m_pos = VSub(m_pos, VGet(0, m_gravity, 0));	// 重力を掛ける
			m_gravity += 0.01f;							// 重力加速

			// ジャンプ後着地処理
			if (m_pos.y <= 0.99f)
			{
				jumpCount -= 1;			// ジャンプカウントを1減らす
				m_gravity = kGravity;
				if (jumpCount == 0)
				{
					m_isFall = true;
				}
			}
		}
		else // ジャンプを2回行った後の処理
		{
			if(m_isFall==true)
			{
				float rota = kRota;

				/*倒れる処理*/
				if (m_attackNum == 0)	// 一体目の敵
				{
					if (maxRota >= kRotaMaxY)
					{
						m_angle = VAdd(m_angle, VGet(0.0f, -rota, 0.0f));
						if (m_angle.y <= 0.0f)
						{
							m_isFall=false;
						}
					}
					else {	// 1.55迄
						m_angle = VAdd(m_angle, VGet(0.0f, rota, 0.0f));
						maxRota = m_angle.y;
					}
				}
				if (m_attackNum == 1)	// 二体目の敵
				{
					if (maxRota <= -kRotaMaxY)
					{
						m_angle = VAdd(m_angle, VGet(0.0f, rota, 0.0f));
						if (m_angle.y >= 0.0f)
						{
							m_isFall = false;
						}
					}
					else {	// -1.55迄
						m_angle = VAdd(m_angle, VGet(0.0f, -rota, 0.0f));
						maxRota = m_angle.y;
					}
				}
				else if (m_attackNum == 2)	// 三体目の敵
				{
					if (maxRota <= -kRotaMaxZ)
					{
						m_angle = VAdd(m_angle, VGet(0.0f, 0.0f, rota));
						if (m_angle.z >= kEnemyRota)
						{
							m_isFall = false;
						}
					}
					else {
						m_angle = VAdd(m_angle, VGet(0.0f, 0.0f, -rota));
						maxRota -= rota;
					}
				}
				else if(m_attackNum==3){
					if (maxRota >= kRotaMaxZ)
					{
						m_angle = VAdd(m_angle, VGet(0.0f, 0.0f, -rota));
						if (m_angle.z <= kEnemyRota)
						{
							m_isFall = false;
						}
					}
					else {
						m_angle = VAdd(m_angle, VGet(0.0f, 0.0f, +rota));
						maxRota += rota;
					}
				}
			}
			else {
				/*倒れた後の処理*/
				jumpCount = 2;
				m_isAttack = false;
				m_isMoveStop = false;
				//m_angle = VGet(0.0f, 0.0f, 0.0f);
				maxRota = 0.0f;

				if (m_attackNum == 0)
				{
					m_angle = VGet(0.0f, 0.0f, kEnemyRota);
				}
				else if (m_attackNum == 2)
				{
					m_angle = VGet(kEnemyRota, 0.0f, kEnemyRota);
				}

				// 移動する敵だった場合、移動処理を戻す
				if (m_isAddMove == true)
				{
					m_move = VGet(0.0f, 0.0f, kMove);
				}
			}	
		}
	}

	// 敵移動
	if (m_isMoveStop == false)
	{
		if (m_isAddMove == true)
		{
			if (m_pos.z >= 11.2f)
			{
				m_move = VScale(m_move, -1.0f);
			}
			if (m_pos.z <= -10.5f)
			{
				m_move = VScale(m_move, -1.0f);
			}
			m_pos = VAdd(m_pos, m_move);
		}
	}

	MV1SetPosition(m_model, m_pos);
	MV1SetRotationXYZ(m_model, m_angle);
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