#include "EnemyRight.h"

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
}

EnemyRight::~EnemyRight()
{
	MV1DeleteModel(m_sordModel);
}

void EnemyRight::Init()
{
}

void EnemyRight::Update()
{
	SetModelFramePosition(m_pModel->GetModel(), "handIK.r", m_sordModel);
}

void EnemyRight::Draw()
{
	// ���f���̕`��
	m_pModel->Draw();
	// �_���f���̕`��
	MV1DrawModel(m_sordModel);
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
