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
	MV1SetPosition(m_model, m_pos);							// プレイヤーの初期位置	

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
	// モデルの描画
	MV1DrawModel(m_model);
	// 棒モデルの描画
	MV1DrawModel(m_sordModel);
}

void EnemyLeft::End()
{
}

void EnemyLeft::SetModelFramePosition(int ModelHandle, char *FrameName, int SetModelHandle)
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
