#include "SceneGame.h"
#include "SceneGameClear.h"
#include "SceneGameOver.h"
#include "DxLib.h"
#include "../Enemy.h"
#include "../Player.h"
#include "../Stage.h"
#include "../Camera.h"
#include "../Pad.h"
#include "../Game.h"


namespace {
	constexpr int kEnemyNum = 4;					// �G�̐�
	constexpr int kGameTime = 1200;					// ��������(���݂͉�)
	constexpr int kFade = 255;						// �t�F�[�h�ő�l

	constexpr float kEnemyPlaceX = 35.0f;			// �GX���W
	constexpr float kEnemyPlaceZ = 24.0f;			// �GZ���W
	constexpr float kEnemyRota = 1.58f;				// �G��]��
	constexpr float kEnemyAddScaleX = 25.0f;		// �G�ǉ��T�C�Y(�����p)
	constexpr float kEnemyAddScaleY = 50.0f;		// �G�ǉ��T�C�Y(�����p)
	constexpr float kEnemyMove = 0.05f;				// �G�ړ���
	constexpr float kEnemyAttackInterval = 240.0f;	// �G�U���̊Ԋu

	constexpr float kEnemySizeWidth = 5.2f;			// �{�̃T�C�Y(�w�\����)
	constexpr float kEnemySizeHeight = 30.0f;		// �{�̃T�C�Y(�c�̒���)
	constexpr float kEnemySizeDepth = 19.0f;		// �{�̃T�C�Y(���s/����)

}

SceneGame::SceneGame() :
	m_enemyInterval(0),
	m_enemyAttckNum(0),
	m_enemyNum(kEnemyNum),
	m_gameTime(kGameTime), 
	m_fadeAlpha(kFade),
	m_col(false),
	m_isFadeIn(false),
	m_isFadeOut(false),
	m_isSceneEnd(false),
	m_enemyAttackPos(VGet(0.0f, 0.0f, 0.0f))
{
	m_pEnemy.resize(m_enemyNum);

	// �G�̍��W
	m_pEnemy[0] = make_shared<Enemy>(VGet(1.0f, 1.0f, kEnemyPlaceZ));	// ��
	m_pEnemy[1] = make_shared<Enemy>(VGet(1.0f, 1.0f, -kEnemyPlaceZ));	// �O
	m_pEnemy[2] = make_shared<Enemy>(VGet(-kEnemyPlaceX, 1.0f, 2.0f));	// ��
	m_pEnemy[3] = make_shared<Enemy>(VGet(kEnemyPlaceX, 1.0f, 2.0f));	// �E

	// �G���f����]
	m_pEnemy[0]->SetModelAngle(VGet(0.0f, 0.0f, kEnemyRota));		// ��
	m_pEnemy[1]->SetModelAngle(VGet(0.0f, 0.0f, kEnemyRota));		// �O
	m_pEnemy[2]->SetModelAngle(VGet(kEnemyRota, 0.0f, kEnemyRota));	// ��
	m_pEnemy[3]->SetModelAngle(VGet(kEnemyRota, 0.0f, kEnemyRota));	// �E

	// �G�̃��f���T�C�Y����
	m_pEnemy[0]->SetAddModelScale(kEnemyAddScaleX,kEnemyAddScaleY);
	m_pEnemy[1]->SetAddModelScale(kEnemyAddScaleX,kEnemyAddScaleY);
	m_pEnemy[2]->SetAddModelScale(kEnemyAddScaleX*2.7, 0.0f);
	m_pEnemy[3]->SetAddModelScale(kEnemyAddScaleX*2.7, 0.0f);

	// �G�̈ړ���
	m_pEnemy[2]->SetMove(VGet(0, 0, kEnemyMove));
	m_pEnemy[3]->SetMove(VGet(0, 0, kEnemyMove));

	// �G���ړ����邩�ǂ���
	m_pEnemy[2]->SetAddMove(true);
	m_pEnemy[3]->SetAddMove(true);
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	// �J�����̏�����
	m_pCamera->Init_Game();
	// �v���C���[�̏�����
	m_pPlayer->Init(m_pStage);
	// �G�̏�����
	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		m_pEnemy[i]->Init();
	}
	// �X�e�[�W�̏�����
	m_pStage->Init();
}

shared_ptr<SceneBase> SceneGame::Update()
{
	Pad::Update();		// Pad�̍X�V

	m_gameTime--;		// �������ԃJ�E���g
	m_enemyInterval++;	// �G���|��Ă���C���^�[�o��

	if (m_enemyInterval >= kEnemyAttackInterval)	// �S�b�o�����烉���_���œG��|��
	{	
		// �G��|������
		m_enemyAttckNum= GetRand(3);				// �ǂ̓G���|��Ă��邩�����_���Ŋl������
		m_pEnemy[m_enemyAttckNum]->SetAttackNum(m_enemyAttckNum);	// �|���G�̔ԍ���G�ɓn�� 
		m_pEnemy[m_enemyAttckNum]->SetAttack(true);					// �|���G�̃t���O��true�ɂ���
		
		if (m_enemyAttckNum < 2)
		{
			int flag = false;
			m_enemyAttackPos = m_pEnemy[m_enemyAttckNum]->GetPos();	// �|��Ă���G�̍��W���l������

			if (m_enemyAttckNum == 0) {
				flag = false;
			}
			else if (m_enemyAttckNum == 1) {
				flag = true;
			}

			// �|��Ă���G�̓����蔻���ݒ肷��
			m_colRect.SetLandscape(m_enemyAttackPos, kEnemySizeWidth, kEnemySizeHeight, kEnemySizeDepth, flag);	
		}

		if (m_enemyAttckNum > 1)
		{
			int flag = false;
			m_enemyAttackPos = m_pEnemy[m_enemyAttckNum]->GetPos();		// �|��Ă���G�̍��W���l������

			if (m_enemyAttckNum == 2) {
				flag = false;
			}
			else if (m_enemyAttckNum == 3) {
				flag = true;
			}

			// �|��Ă���G�̓����蔻���ݒ肷��
			m_colRect.SetPortrait(m_enemyAttackPos, kEnemySizeWidth, kEnemySizeHeight, kEnemySizeDepth, flag);
		}
		m_enemyInterval = 0;
	}

	// �G�ƃv���C���[�̓����蔻��
	if (m_pEnemy[m_enemyAttckNum]->GetFall())
	{
		// �����蔻�菈��
		Collision playerRect = m_pPlayer->GetColRect();
		Collision enemyRect = m_colRect;
		if (playerRect.IsCollision(m_enemyAttckNum, m_colRect)){
			m_col = true;
			m_isFadeOut = true;
		}
	}
	else {
		m_col = false;
	}

	if (m_gameTime <= 0)
	{
		m_isFadeOut = true;
	}

	// �v���C���[�̍X�V
	m_pPlayer->Update();
	// �G�̍X�V
	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		m_pEnemy[i]->Update();
	}

	// �t�F�[�h�C��
	if (!m_isFadeIn)
	{
		m_fadeAlpha -= 8;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
			m_isFadeIn = true;			// �t�F�[�h�C���t���O��true�ɂ���
		}
	}

	// �t�F�[�h�A�E�g
	if (m_isFadeOut)
	{
		// �t�F�[�h�A�E�g������
		if (m_isSceneEnd)
		{
			if (!m_col)
			{
				return make_shared<SceneGameClear>();	// �Q�[���N���A�V�[���֌�����
			}
			if (m_col)
			{
				return make_shared<SceneGameOver>();	// �Q�[���I�[�o�[�V�[���֌�����
			}
		}

		m_fadeAlpha += 8;
		if (m_fadeAlpha >= 255)
		{
			m_fadeAlpha = 255;
			m_isSceneEnd = true;
		}
	}

#ifdef _DEBUG
	// �f�o�b�O�p
	if (Pad::IsTrigger(PAD_INPUT_1))	// �Q�[���I�[�o�[��ʂ֑J��
	{
		m_col = true;
		m_isFadeOut = true;
	}
	if (Pad::IsTrigger(PAD_INPUT_2))	// �Q�[���N���A��ʂ֑J��
	{
		m_isFadeOut = true;
	}
#endif // DEBUG
	return shared_from_this();
}

void SceneGame::Draw()
{
	m_pStage->Draw();	// �X�e�[�W�`��

	// �G�����蔻��`��
	if (m_pEnemy[m_enemyAttckNum]->GetAttack()){
		m_colRect.EnemyDraw(0x000000, true);
	}

	// �G�`��
	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		m_pEnemy[i]->Draw();
	}
	// �v���C���[�`��
	m_pPlayer->Draw();

	DrawString(0, 0, "SceneGame", 0xffffff);
	DrawFormatString(0, 20, 0xffffff, "��������=%d", m_gameTime/60);

	// �t�F�[�h�C���E�t�F�[�h�A�E�g�`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// �������ŕ\���J�n
	DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, 0x00000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);			// �s�����ɖ߂��Ă���
#ifdef _DEBUG

#ifdef DEBUG
	DrawFormatString(0, 0, 0xffffff, "m_enemyAttckNum=%d", m_enemyAttckNum);
	DrawFormatString(0, 70, 0xffffff, "m_enemyInterval=%d", m_enemyInterval);
	DrawFormatString(0, 200, 0xffffff,"ifPos.x=%.2f:z=%.2f", m_enemyAttackPos.x, m_enemyAttackPos.z);
	DrawFormatString(0, 360, 0xffffff, "col=%d", col);
#endif // DEBUG

}

void SceneGame::End()
{
	m_pPlayer->End();

	for (int i = 0; i < m_pEnemy.size(); i++)
	{
		m_pEnemy[i]->End();
	}
	m_pStage->End();
}