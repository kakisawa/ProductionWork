#include "EnemyLeft.h"
#include "EnemyState.h"
#include "../Player/Player.h"
#include "../../Util/Pad.h"
#include "../GameMap.h"
#include <math.h>
#include <cassert>

namespace {
	const char* kModelEnemy = "data/model/EnemyModel/Enemy2.mv1";
	const char* kSord = "data/model/EnemyModel/Blade.mv1";

	const char* kParts = "handIK.r";

	const char* const kUI[4]{
		"data/UI/GameScene/EnemyLeft/HPOurGauge.png",	// HPUI(�O��)
		"data/UI/GameScene/EnemyLeft/HPInGauge.png",	// HPUI(����)
		"data/UI/GameScene/EnemyLeft/NameBg.png",		// ���O�w�iUI
		"data/UI/GameScene/EnemyLeft/Shimane.png",		// ���OUI
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


	constexpr int	kMaxHp = 100;				// �̗͍ő�l

	VECTOR kSordSize = VGet(0.01f, 0.01f, 0.01f);
	VECTOR kInitPos= VGet(-10.0f, 0.0f, 10.0f);

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
	m_upPos(kInitVec)
{
	m_sordModel = MV1LoadModel(kSord);

	// UI�摜�̓ǂݍ���
	for (int i = 0; i < m_uiGraph.size(); i++)
	{
		m_uiGraph[i] = LoadGraph(kUI[i]);
		assert(m_uiGraph[i] != -1);
	}

	m_pState = std::make_shared<EnemyState>();

	m_pState->AddState([=] { IdleStateUpdate(); }, [=] { IdleStateInit(); }, EnemyState::State::kIdle);
	m_pState->AddState([=] { WalkStateUpdate(); }, [=] { WalkStateInit(); }, EnemyState::State::kWalk);

	//�����X�e�C�g�Z�b�g
	m_pState->SetState(EnemyState::State::kIdle);	//�X�e�C�g�Z�b�g(�ŏ���Idle���)
}

EnemyLeft::~EnemyLeft()
{
	MV1DeleteModel(m_sordModel);
}

void EnemyLeft::Init(std::shared_ptr<GameMap> pMap)
{
	mp.leftBack = pMap->GetMapLeftBack();
	mp.rightFront = pMap->GetMapRightFront();
}

void EnemyLeft::Update(const Player& player)
{
	m_pState->Update();

	m_pModel->Update();
	
	if (player.GetAttackLeft())
	{
		m_hp -= player.GetAddDamage();
		m_hp=max(m_hp, 0);
	}

	Move();

	SetModelFramePosition(m_pModel->GetModel(), kParts, m_sordModel);

	// �����蔻��p�J�v�Z���^�̍��W�X�V
	m_upPos = VAdd(m_pos, kUpPos);
	m_colSphere.UpdateCol(m_pos, m_upPos,kInitPos, kColRadius,kAttackColRadius);
}

void EnemyLeft::Draw()
{
	if (m_hp > 0)
	{
		// ���f���̕`��
		m_pModel->Draw();
		// �_���f���̕`��
		MV1DrawModel(m_sordModel);
	}

#ifdef _DEBUG
	if (m_hp > 0)
	{
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
}

void EnemyLeft::UIDraw()
{
	// HP�Q�[�W�`��
	DrawExtendGraph(kHpGaugeUIPosX, kHpGaugeUIPosY,
		kHpGaugeUIPosX + (kHpGaugePosX * (m_hp * 0.01f)), kHpGaugePosY, m_uiGraph[1], true);
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
