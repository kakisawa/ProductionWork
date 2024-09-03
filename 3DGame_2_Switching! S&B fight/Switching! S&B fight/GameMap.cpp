#include "DxLib.h"
#include "GameMap.h"

namespace {
	// �}�b�v���f��
	const char* const kMapModel = "data/model/map.mv1";

	const VECTOR kMapScale = VGet(0.01f, 0.01f, 0.01f);
	const VECTOR kMapPosition = VGet(0.0f, 2.0f, 1.0f);

	constexpr int kStageLeftX = -175;	// ���[���WX
	constexpr int kStageRightX = 175;	// �E�[���WX
	constexpr int kMapBackZ = 180;		// ���[���WZ
	constexpr int kMapFrontZ = -106;	// �O�[���WZ

	// �X�e�[�W���W�e��
	const VECTOR kMapLeftBack = VGet(kStageLeftX, 0, kMapBackZ);		// ������W
	const VECTOR kMapLeftFront = VGet(kStageLeftX, 0, kMapFrontZ);	// �������W
	const VECTOR kMapRightBack = VGet(kStageRightX, 0, kMapBackZ);		// �E����W
	const VECTOR kMapRightFront = VGet(kStageRightX, 0, kMapFrontZ);	// �E�����W
}

GameMap::GameMap():
	m_mapLeftBack(kMapLeftBack),
	m_mapRightFront(kMapRightFront)
{
	// �}�b�v
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
	// �}�b�v
	MV1DrawModel(m_model);
}

void GameMap::End()
{
	MV1DeleteModel(m_model);
}
