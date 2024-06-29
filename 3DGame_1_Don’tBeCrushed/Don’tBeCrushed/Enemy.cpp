#include "DxLib.h"
#include "Enemy.h"

namespace {
	const char* const kModelEnemy = "data/model/book.mv1";
	
	int jumpCount = 2;					// �W�����v��
	constexpr float kMove = 0.05f;		// �G�ړ���
	constexpr float kGravity = 0.18f;	// �d��
	constexpr float kJumpPow = 0.5f;	// �W�����v��

	constexpr float kRota = 0.05f;		// ��]��
	constexpr float kRotaMaxY = 1.5f;		// Y���ő��]��
	constexpr float kRotaMaxZ = 1.7f;		// Z���ő��]��

	constexpr float kEnemyRota = 1.58f;		// �G��]��(SceneGame�ɂ�����)
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

	// �G�U���O
	if (m_isAttack == true) {

		m_isMoveStop = true;	// �G�̈ړ����~�߂�
		

		if (jumpCount > 0)		// �W�����v�̎c��񐔂�0�ȏ�Ȃ珈�����s��
		{
			

			m_move = VGet(0, 0, 0);
			m_move = VAdd(m_move, VGet(0, kJumpPow, 0));
			m_pos = VAdd(m_pos, m_move);

			m_pos = VSub(m_pos, VGet(0, m_gravity, 0));	// �d�͂��|����
			m_gravity += 0.01f;							// �d�͉���

			// �W�����v�㒅�n����
			if (m_pos.y <= 0.99f)
			{
				jumpCount -= 1;			// �W�����v�J�E���g��1���炷
				m_gravity = kGravity;
				if (jumpCount == 0)
				{
					m_isFall = true;
				}
			}
		}
		else // �W�����v��2��s������̏���
		{
			if(m_isFall==true)
			{
				float rota = kRota;

				/*�|��鏈��*/
				if (m_attackNum == 0)	// ��̖ڂ̓G
				{
					if (maxRota >= kRotaMaxY)
					{
						m_angle = VAdd(m_angle, VGet(0.0f, -rota, 0.0f));
						if (m_angle.y <= 0.0f)
						{
							m_isFall=false;
						}
					}
					else {	// 1.55��
						m_angle = VAdd(m_angle, VGet(0.0f, rota, 0.0f));
						maxRota = m_angle.y;
					}
				}
				if (m_attackNum == 1)	// ��̖ڂ̓G
				{
					if (maxRota <= -kRotaMaxY)
					{
						m_angle = VAdd(m_angle, VGet(0.0f, rota, 0.0f));
						if (m_angle.y >= 0.0f)
						{
							m_isFall = false;
						}
					}
					else {	// -1.55��
						m_angle = VAdd(m_angle, VGet(0.0f, -rota, 0.0f));
						maxRota = m_angle.y;
					}
				}
				else if (m_attackNum == 2)	// �O�̖ڂ̓G
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
				/*�|�ꂽ��̏���*/
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

				// �ړ�����G�������ꍇ�A�ړ�������߂�
				if (m_isAddMove == true)
				{
					m_move = VGet(0.0f, 0.0f, kMove);
				}
			}	
		}
	}

	// �G�ړ�
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