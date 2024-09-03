#include "DxLib.h"
#include "GameMap.h"

namespace {
	// マップモデル
	const char* const kMapModel = "data/model/map.mv1";

	const VECTOR kMapScale = VGet(0.01f, 0.01f, 0.01f);
	const VECTOR kMapPosition = VGet(0.0f, 2.0f, 1.0f);

	constexpr int kStageLeftX = -175;	// 左端座標X
	constexpr int kStageRightX = 175;	// 右端座標X
	constexpr int kMapBackZ = 180;		// 奥端座標Z
	constexpr int kMapFrontZ = -106;	// 前端座標Z

	// ステージ座標各位
	const VECTOR kMapLeftBack = VGet(kStageLeftX, 0, kMapBackZ);		// 左上座標
	const VECTOR kMapLeftFront = VGet(kStageLeftX, 0, kMapFrontZ);	// 左下座標
	const VECTOR kMapRightBack = VGet(kStageRightX, 0, kMapBackZ);		// 右上座標
	const VECTOR kMapRightFront = VGet(kStageRightX, 0, kMapFrontZ);	// 右下座標
}

GameMap::GameMap():
	m_mapLeftBack(kMapLeftBack),
	m_mapRightFront(kMapRightFront)
{
	// マップ
	m_model = MV1LoadModel(kMapModel);
	MV1SetScale(m_model, kMapScale);
	MV1SetPosition(m_model, kMapPosition);
}

GameMap::~GameMap()
{
}

void GameMap::Init()
{
}

void GameMap::Update()
{
}

void GameMap::Draw()
{	
	// マップ
	MV1DrawModel(m_model);
}

void GameMap::End()
{
	MV1DeleteModel(m_model);
}
