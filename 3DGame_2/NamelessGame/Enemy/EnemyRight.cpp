#include "EnemyRight.h"
#include "EnemyState.h"

namespace {
	const char* kModelEnemy = "data/model/EnemyModel/Enemy1.mv1";
	const char* kSord = "data/model/EnemyModel/Blade.mv1";

	VECTOR kSordSize = VGet(0.01f, 0.01f, 0.01f);
	VECTOR kInitPos = VGet(10.0f, 0.0f, 10.0f);
}

EnemyRight::EnemyRight():
	EnemyBase(kModelEnemy, kInitPos),
	m_sordModel(-1)
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

void EnemyRight::Init()
{

}

void EnemyRight::Update(const Player& player)
{
	m_pState->Update();

	m_pModel->Update();

	SetModelFramePosition(m_pModel->GetModel(), "handIK.r", m_sordModel);
}

void EnemyRight::Draw()
{
	// ���f���̕`��
	m_pModel->Draw();
	// �_���f���̕`��
	MV1DrawModel(m_sordModel);

	DrawFormatString(0, 140, 0xffffff, "EnemyRight:m_hp=%d", m_hp);

}

void EnemyRight::End()
{
}

void EnemyRight::SetModelFramePosition(int ModelHandle, char* FrameName, int SetModelHandle)
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
