#include "EnemyLeft.h"
#include "EnemyState.h"
#include "../Player/Player.h"
#include "../../Util/Pad.h"
#include "../GameMap.h"
#include "../../Util/Effect.h"
#include "../../Manager/SoundManager.h"
#include <math.h>
#include <cassert>

namespace {
	const char* kModelEnemy = "data/model/EnemyModel/Enemy2.mv1";
	const char* kSord = "data/model/EnemyModel/Blade.mv1";

	const char* kParts = "handIK.r";

	const char* const kUI[4]{
		"data/Image/GameScene/UI/EnemyLeft/HPOurGauge.png",	// HPUI(�O��)
		"data/Image/GameScene/UI/EnemyLeft/HPInGauge.png",	// HPUI(����)
		"data/Image/GameScene/UI/EnemyLeft/NameBg.png",		// ���O�w�iUI
		"data/Image/GameScene/UI/EnemyLeft/Shimane.png",	// ���OUI
	};

	// HP�Q�[�W(�O��)�ʒu
	constexpr int kHpGaugeUIPosX = 650;
	constexpr int kHpGaugeUIPosY = 950;

	// HP�Q�[�W(����)�E���ʒu(Exted�p�E�[���W)
	constexpr int kHpGaugePosX = 852;
	constexpr int kHpGaugePosY = kHpGaugeUIPosY + 42;

	// ���O�w�i�ʒu
	constexpr int kNameBgX = 300;
	constexpr int kNameBgY = 930;

	// ���O�ʒu
	constexpr int kNameX = kNameBgX + 80;
	constexpr int kNameY = kNameBgY + 10;

	constexpr float kInitAngle = -DX_PI_F / 2.0f * 90.0f;	// �v���C���[�̏����p�x*90(�����𔽑΂ɂ���)

	constexpr int	kMaxHp = 100;				// �̗͍ő�l

	VECTOR kSordSize = VGet(0.01f, 0.01f, 0.01f);
	VECTOR kInitPos= VGet(-40.0f, 0.0f, 100.0f);

	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// �x�N�g���̏�����
	
	const VECTOR kUpPos = VGet(0.0f, 20.0f, 0.0f);
	const VECTOR kAttackRange = VGet(0.0f, 0.0f, 0.0f);
	constexpr float kColRadius = 5.0;
	constexpr float kAttackColRadius = 0.0;
	
}

EnemyLeft::EnemyLeft():
	EnemyBase(kModelEnemy,kInitPos),
	m_sordModel(-1),
	m_isWalk(false),
	m_isEffect(false),
	m_angle(0.0f),
	m_upPos(kInitVec),
	m_vecToPlayer(kInitVec),
	m_targetPos(kInitVec)
{
	m_sordModel = MV1LoadModel(kSord);
	m_pos = m_pModel->GetPos();

	// UI�摜�̓ǂݍ���
	for (int i = 0; i < m_uiGraph.size(); i++)
	{
		m_uiGraph[i] = LoadGraph(kUI[i]);
		assert(m_uiGraph[i] != -1);
	}

	m_isAlive = true;

	m_pState = std::make_shared<EnemyState>();
	m_pEffect = std::make_shared<Effect>();

	m_pState->AddState([=] { IdleStateUpdate(); }, [=] { IdleStateInit(); }, EnemyState::State::kIdle);
	m_pState->AddState([=] { WalkStateUpdate(); }, [=] { WalkStateInit(); }, EnemyState::State::kWalk);

	//�����X�e�C�g�Z�b�g
	m_pState->SetState(EnemyState::State::kIdle);	//�X�e�C�g�Z�b�g(�ŏ���Idle���)

	m_pSound = new SoundManager;
	m_pSound->Init();
	m_pSound->LoadSE(SoundManager::SE_Type::kExtinction);
}

EnemyLeft::~EnemyLeft()
{
	MV1DeleteModel(m_sordModel);
	m_pEffect->ClearEffect();
}

void EnemyLeft::Init(std::shared_ptr<GameMap> pMap)
{
	mp.leftBack = pMap->GetMapLeftBack();
	mp.rightFront = pMap->GetMapRightFront();

	m_pEffect->Init();
}

void EnemyLeft::Update(const Player& player)
{
	if (m_isAlive)
	{
		m_pState->Update();

		m_pModel->Update();

		m_targetPos = player.GetPos();

		if (player.GetAttackLeft())
		{
			m_hp -= player.GetAddDamage();
			m_hp = max(m_hp, 0);
			if (m_hp <= 0)
			{
				m_isEffect = true;
				m_isAlive = false;
			}
		}

		Move();

		SetModelFramePosition(m_pModel->GetModel(), kParts, m_sordModel);

		// �����蔻��p�J�v�Z���^�̍��W�X�V
		m_upPos = VAdd(m_pos, kUpPos);
		m_colSphere.UpdateCol(m_pos, m_upPos, kInitPos, kColRadius, kAttackColRadius);
	}
	else
	{
		m_colSphere.Init();

		if (m_isEffect)
		{
			m_pSound->PlaySE(SoundManager::SE_Type::kExtinction, DX_PLAYTYPE_BACK);
			m_pEffect->PlayDeathEffect(m_pos);
			m_isEffect = false;
		}
	}

	m_pEffect->Update();
}

void EnemyLeft::Draw()
{
	if (m_isAlive)
	{
		// ���f���̕`��
		m_pModel->Draw();
		// �_���f���̕`��
		MV1DrawModel(m_sordModel);
	}

	m_pEffect->Draw();			 // �G�t�F�N�g�`��

	
#ifdef _DEBUG
	DrawFormatString(0, 800, 0xffffff, "m_pos=%.2f:=%.2f:=%.2f", m_pos.x, m_pos.y, m_pos.z);
	if (m_isAlive)
	{
		DrawFormatString(0, 600, 0xffffff, "m_hp=%d", m_hp);
		m_colSphere.DrawMain(0x00ff00, false);	// �����蔻��`��
	}
#endif
}

void EnemyLeft::End()
{
	// UI�摜�̍폜
	for (int i = 0; i < m_uiGraph.size(); i++)
	{
		DeleteGraph(m_uiGraph[i]);
	}

	m_pSound->ReleaseSound();

	delete m_pSound;
	m_pSound = nullptr;
}

void EnemyLeft::Move()
{
	//�G����ʊO�ɏo�Ȃ��悤���鏈��
	if (m_pos.x < mp.leftBack.x)
	{
		m_pos.x -= m_move.x;		// ��
	}
	if (m_pos.x > mp.rightFront.x)
	{
		m_pos.x -= m_move.x;		// �E
	}
	if (m_pos.z < mp.rightFront.z)
	{
		m_pos.z -= m_move.z;		// �O
	}
	if (m_pos.z > mp.leftBack.z)
	{
		m_pos.z -= m_move.z;		// ��
	}

	m_vecToPlayer = VSub(m_pos, m_targetPos);
	// atan2 ���g�p���Ċp�x���擾
	m_angle = atan2(m_vecToPlayer.x, m_vecToPlayer.z);

	// atan2 �Ŏ擾�����p�x�ɂR�c���f���𐳖ʂɌ������邽�߂̕␳�l( DX_PI_F )��
		// �������l���R�c���f���� Y����]�l�Ƃ��Đݒ�
	MV1SetRotationXYZ(m_pModel->GetModel(), VGet(0.0f, m_angle + DX_PI_F + kInitAngle, 0.0f));
}

void EnemyLeft::UIDraw()
{
	// HP�Q�[�W�`��
	DrawExtendGraph(kHpGaugeUIPosX, kHpGaugeUIPosY,
		kHpGaugeUIPosX + (kHpGaugePosX * (m_hp * 0.004f)), kHpGaugePosY, m_uiGraph[1], true);
	DrawGraph(kHpGaugeUIPosX, kHpGaugeUIPosY, m_uiGraph[0], true);

	// �G���`��
	DrawGraph(kNameBgX, kNameBgY, m_uiGraph[2], true);
	DrawGraph(kNameX, kNameY, m_uiGraph[3], true);
}

void EnemyLeft::SetModelFramePosition(int ModelHandle, const char *FrameName, int SetModelHandle)
{
	MATRIX FrameMatrix;
	int FrameIndex;

	// �t���[��������t���[���ԍ����擾����
	FrameIndex = MV1SearchFrame(ModelHandle, FrameName);

	// �t���[���̌��݂̃��[���h�ł̏�Ԃ������s����擾����
	FrameMatrix = MV1GetFrameLocalWorldMatrix(ModelHandle, FrameIndex);

	// �Z�b�g���郂�f���̏�Ԃ������s����t���[���̏�Ԃ������s��Ɠ����ɂ���
	// MV1SetMatrix(SetModelHandle, FrameMatrix);
	MV1SetMatrix(SetModelHandle, MMult(MGetScale(kSordSize), FrameMatrix));
}

void EnemyLeft::IdleStateUpdate()
{
	m_pModel->ChangeAnim(m_animData.kIdle, true, false, 0.5f);
}

void EnemyLeft::WalkStateUpdate()
{
	m_pModel->ChangeAnim(m_animData.kWalk, true, false, 0.5f);
}
