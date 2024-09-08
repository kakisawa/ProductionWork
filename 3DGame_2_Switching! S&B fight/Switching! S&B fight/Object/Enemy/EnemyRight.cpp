#include "EnemyRight.h"
#include "EnemyState.h"
#include "../Player/Player.h"
#include "../GameMap.h"
#include "../../Util/Effect.h"
#include <cassert>
#include <math.h>

namespace {
	const char* kModelEnemy = "data/model/EnemyModel/Enemy1.mv1";
	const char* kSord = "data/model/EnemyModel/Blade.mv1";

	const char* kParts = "handIK.r";

	const char* const kUI[4]{
		"data/UI/GameScene/EnemyRight/HPOurGauge.png",	// HPUI(�O��)
		"data/UI/GameScene/EnemyRight/HPInGauge.png",	// HPUI(����)
		"data/UI/GameScene/EnemyRight/NameBg.png",		// ���O�w�iUI
		"data/UI/GameScene/EnemyRight/Shimane.png",		// ���OUI
	};

	// HP�Q�[�W(�O��)�ʒu
	constexpr int kHpGaugeUIPosX = 650;
	constexpr int kHpGaugeUIPosY = 1020;

	// HP�Q�[�W(����)�E���ʒu(Exted�p�E�[���W)
	constexpr int kHpGaugePosX = 852;
	constexpr int kHpGaugePosY = kHpGaugeUIPosY + 42;

	// ���O�w�i�ʒu
	constexpr int kNameBgX = 300;
	constexpr int kNameBgY = 1000;

	// ���O�ʒu
	constexpr int kNameX = kNameBgX + 80;
	constexpr int kNameY = kNameBgY + 10;

	VECTOR kSordSize = VGet(0.01f, 0.01f, 0.01f);
	VECTOR kInitPos = VGet(40.0f, 0.0f, 100.0f);

	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// �x�N�g���̏�����

	constexpr float kInitAngle = -DX_PI_F / 2.0f * 90.0f;	// �v���C���[�̏����p�x*90(�����𔽑΂ɂ���)

	const VECTOR kUpPos = VGet(0.0f, 20.0f, 0.0f);
	const VECTOR kAttackRange = VGet(0.0f, 0.0f, 0.0f);
	constexpr float kColRadius = 5.0;
	constexpr float kAttackColRadius = 0.0;
}

EnemyRight::EnemyRight() :
	EnemyBase(kModelEnemy, kInitPos),
	m_sordModel(-1),
	m_angle(0.0f),
	m_isWalk(false),
	m_upPos(kInitVec),
	m_vecToPlayer(kInitVec),
	m_targetPos(kInitVec)
{
	m_sordModel = MV1LoadModel(kSord);

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
}

EnemyRight::~EnemyRight()
{
	MV1DeleteModel(m_sordModel);
}

void EnemyRight::Init(std::shared_ptr<GameMap> pMap)
{
	m_pEffect->Init();
}

void EnemyRight::Update(const Player& player)
{
	m_pEffect->Update();

	if (m_isAlive) {

		m_pState->Update();

		m_pModel->Update();

		m_targetPos = player.GetPos();

		if (player.GetAttackRight())
		{
			m_hp -= player.GetAddDamage();
			m_hp = max(m_hp, 0);
			if (m_hp <= 0)
			{
				m_isAlive = false;
			}
		}

		Move();

		SetModelFramePosition(m_pModel->GetModel(), "handIK.r", m_sordModel);

		// �����蔻��p�J�v�Z���^�̍��W�X�V
		m_upPos = VAdd(m_pos, kUpPos);
		m_colSphere.UpdateCol(m_pos, m_upPos, kInitPos, kColRadius, kAttackColRadius);
	}
	else
	{
		m_colSphere.Init();
		m_pEffect->PlayDeathEffect(m_pos);
	}
}

void EnemyRight::Draw()
{
	if (m_isAlive)
	{
		// ���f���̕`��
		m_pModel->Draw();
		// �_���f���̕`��
		MV1DrawModel(m_sordModel);
	}
	else
	{
		m_pEffect->Draw();			 // �G�t�F�N�g�`��
	}

#ifdef _DEBUG
	if (m_isAlive)
	{
		m_colSphere.DrawMain(0x00ff00, false);	// �����蔻��`��
	}
#endif
}

void EnemyRight::End()
{
	// UI�摜�̍폜
	for (int i = 0; i < m_uiGraph.size(); i++)
	{
		DeleteGraph(m_uiGraph[i]);
	}
}

void EnemyRight::UIDraw()
{
	// HP�Q�[�W�`��
	DrawExtendGraph(kHpGaugeUIPosX, kHpGaugeUIPosY,
		kHpGaugeUIPosX + (kHpGaugePosX * (m_hp * 0.004f)), kHpGaugePosY, m_uiGraph[1], true);
	DrawGraph(kHpGaugeUIPosX, kHpGaugeUIPosY, m_uiGraph[0], true);

	// �G���`��
	DrawGraph(kNameBgX, kNameBgY, m_uiGraph[2], true);
	DrawGraph(kNameX, kNameY, m_uiGraph[3], true);


#ifdef _DEBUG
	DrawFormatString(0, 700, 0xffffff, "m_pos.x=%.2f:.y=%.2f:.z=%.2f", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(0, 720, 0xffffff, "Getm_pos.x=%.2f:.y=%.2f:.z=%.2f", m_targetPos.x, m_targetPos.y, m_targetPos.z);
	DrawFormatString(0, 740, 0xffffff, "m_vecToPlayer.x=%.2f:.y=%.2f:.z=%.2f", m_vecToPlayer.x, m_vecToPlayer.y, m_vecToPlayer.z);
#endif // DEBUG
}

void EnemyRight::Move()
{
	// �G����ʊO�ɏo�Ȃ��悤���鏈��
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
	MV1SetRotationXYZ(m_pModel->GetModel(), VGet(0.0f, m_angle + DX_PI_F+ kInitAngle, 0.0f));
}

void EnemyRight::SetModelFramePosition(int ModelHandle, const char* FrameName, int SetModelHandle)
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

	MV1SetPosition(m_model, m_pos);
}

void EnemyRight::IdleStateUpdate()
{
	m_pModel->ChangeAnim(m_animData.kIdle, true, false, 0.5f);
}

void EnemyRight::WalkStateUpdate()
{
	m_pModel->ChangeAnim(m_animData.kWalk, true, false, 0.5f);
}
