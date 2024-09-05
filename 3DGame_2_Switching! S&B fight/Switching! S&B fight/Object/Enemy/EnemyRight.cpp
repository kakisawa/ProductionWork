#include "EnemyRight.h"
#include "EnemyState.h"
#include "../Player/Player.h"

namespace {
	const char* kModelEnemy = "data/model/EnemyModel/Enemy1.mv1";
	const char* kSord = "data/model/EnemyModel/Blade.mv1";

	const char* kParts = "handIK.r";

	const char* const kHpOutGauge = "data/UI/EnemyGauge.png";			// HP�Q�[�W�O��UI
	const char* const kHpInGauge = "data/UI/EnemyRightGauge.png";		// HP�Q�[�W����UI

	constexpr int kHpGaugePosX = 500;
	constexpr int kHpGaugePosY = 890;

	VECTOR kSordSize = VGet(0.01f, 0.01f, 0.01f);
	VECTOR kInitPos = VGet(10.0f, 0.0f, 10.0f);

	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// �x�N�g���̏�����

	const VECTOR kUpPos = VGet(0.0f, 20.0f, 0.0f);
	const VECTOR kAttackRange = VGet(0.0f, 0.0f, 0.0f);
	constexpr float kColRadius = 5.0;
	constexpr float kAttackColRadius = 0.0;
}

EnemyRight::EnemyRight() :
	EnemyBase(kModelEnemy, kInitPos),
	m_sordModel(-1),
	m_upPos(kInitVec)
{
	m_sordModel = MV1LoadModel(kSord);

	m_pState = std::make_shared<EnemyState>();

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
	m_outGauge = LoadGraph(kHpOutGauge);
	m_inGauge = LoadGraph(kHpInGauge);
}

void EnemyRight::Update(const Player& player)
{
	m_pState->Update();

	m_pModel->Update();

	if (player.GetAttackRight())
	{
		m_hp -= player.GetAddDamage();
		m_hp = max(m_hp, 0);
	}


	// move
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
	}


	SetModelFramePosition(m_pModel->GetModel(), "handIK.r", m_sordModel);

	// �����蔻��p�J�v�Z���^�̍��W�X�V
	m_upPos = VAdd(m_pos, kUpPos);
	m_colSphere.UpdateCol(m_pos, m_upPos, kInitPos, kColRadius, kAttackColRadius);


}

void EnemyRight::Draw()
{
	if (m_hp > 0)
	{
		// ���f���̕`��
		m_pModel->Draw();
		// �_���f���̕`��
		MV1DrawModel(m_sordModel);
	}

	DrawExtendGraph(kHpGaugePosX, kHpGaugePosY,
		kHpGaugePosX + (852 * (m_hp * 0.01f)), (kHpGaugePosY + 42),
		m_inGauge, true);
	DrawGraph(kHpGaugePosX, kHpGaugePosY, m_outGauge, true);


#ifdef _DEBUG
	if (m_hp > 0)
	{
		m_colSphere.DrawMain(0x00ff00, false);	// �����蔻��`��
	}
#endif
}

void EnemyRight::End()
{
	DeleteGraph(m_inGauge);
	DeleteGraph(m_outGauge);
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
}

void EnemyRight::IdleStateUpdate()
{
	m_pModel->ChangeAnim(m_animData.kIdle, true, false, 0.5f);
}

void EnemyRight::WalkStateUpdate()
{
	m_pModel->ChangeAnim(m_animData.kWalk, true, false, 0.5f);
}
