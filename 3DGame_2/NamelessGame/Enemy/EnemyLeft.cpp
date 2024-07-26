#include "EnemyLeft.h"
#include <math.h>

namespace {
	const char* kModelEnemy = "data/model/EnemyModel/Enemy2.mv1";
	const char* kSord = "data/model/EnemyModel/Blade.mv1";

	VECTOR kSordSize = VGet(0.01f, 0.01f, 0.01f);
}

EnemyLeft::EnemyLeft():
	EnemyBase(kModelEnemy),
	m_sordModel(-1)
{
	m_pos = VGet(-10.0f, 0.0f, 10.0f);
	MV1SetPosition(m_model, m_pos);							// �v���C���[�̏����ʒu	

	m_sordModel = MV1LoadModel(kSord);
}

EnemyLeft::~EnemyLeft()
{
	MV1DeleteModel(m_sordModel);
}

void EnemyLeft::Init()
{
}

void EnemyLeft::Update()
{
	SetModelFramePosition(m_model, "handIK.r", m_sordModel);
}

void EnemyLeft::Draw()
{
	// ���f���̕`��
	MV1DrawModel(m_model);
	// �_���f���̕`��
	MV1DrawModel(m_sordModel);
}

void EnemyLeft::End()
{
}

void EnemyLeft::SetModelFramePosition(int ModelHandle, char *FrameName, int SetModelHandle)
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
