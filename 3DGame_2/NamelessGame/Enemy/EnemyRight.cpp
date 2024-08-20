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
	// モデルの描画
	m_pModel->Draw();
	// 棒モデルの描画
	MV1DrawModel(m_sordModel);
}

void EnemyRight::End()
{
}

void EnemyRight::SetModelFramePosition(int ModelHandle, char* FrameName, int SetModelHandle)
{
	MATRIX FrameMatrix;
	int FrameIndex;

	// フレーム名からフレーム番号を取得する
	FrameIndex = MV1SearchFrame(ModelHandle, FrameName);

	// フレームの現在のワールドでの状態を示す行列を取得する
	FrameMatrix = MV1GetFrameLocalWorldMatrix(ModelHandle, FrameIndex);

	// セットするモデルの状態を示す行列をフレームの状態を示す行列と同じにする
	// MV1SetMatrix(SetModelHandle, FrameMatrix);
	MV1SetMatrix(SetModelHandle, MMult(MGetScale(kSordSize), FrameMatrix));
}
