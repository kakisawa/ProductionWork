#include "DxLib.h"
#include "Stage.h"
#include "Game.h"

namespace {
	const char* const kBg = "data/floor2.png";

	constexpr int kStageLeftX = -30;	// ���[���WX
	constexpr int kStageRightX = 30;	// �E�[���WX
	constexpr int kStageUpZ = -20;		// ��[���WZ
	constexpr int kStageDownZ = 20;		// ���[���WZ

	// �X�e�[�W���W�e��
	const VECTOR kStageLeftUp = VGet(kStageLeftX, 0, kStageUpZ);		// ������W
	const VECTOR kStageLeftDown = VGet(kStageLeftX, 0, kStageDownZ);	// �������W
	const VECTOR kStageRightUp = VGet(kStageRightX, 0, kStageUpZ);		// �E����W
	const VECTOR kStageRightDown = VGet(kStageRightX, 0, kStageDownZ);	// �E�����W
}

Stage::Stage() :
	bgGraph(-1),
	m_stageUpperLeft(kStageLeftUp),
	m_stageLowerRight(kStageRightDown)
{
}

Stage::~Stage()
{
}

void Stage::Init()
{
	bgGraph = LoadGraph(kBg);	// �w�i�摜���[�h
}

void Stage::Update()
{
}

void Stage::Draw()
{
	// �w�i�摜�`��
	DrawExtendGraph(0, 0, kScreenWidth, kScreenHeight, bgGraph, false);

	// �X�e�[�W�`��
	for (int x = kStageLeftX; x <= kStageRightX; x += 10)
	{
		DrawLine3D(VGet(static_cast<float>(x), 0, kStageUpZ), VGet(static_cast<float>(x), 0, 20), 0xffffff);
	}
	for (int z = kStageUpZ; z <= kStageDownZ; z += 10)
	{
		DrawLine3D(VGet(kStageLeftX, 0, static_cast<float>(z)), VGet(kStageRightX, 0, static_cast<float>(z)), 0xffffff);
	}

#ifdef DEBUG
	// �X�e�[�W�l���`��(�f�o�b�O)
	DrawSphere3D(kStageLeftUp, 2, 32, true, 0xff00ff, 0xff00ff);		// ����
	DrawSphere3D(kStageLeftDown, 2, 32, true, 0xff00ff, 0xff00ff);	// ����
	DrawSphere3D(kStageRightUp, 2, 32, true, 0xff00ff, 0xff00ff);	// �E��
	DrawSphere3D(kStageRightDown, 2, 32, true, 0xff00ff, 0xff00ff);	// �E��

	// X+-,Z+-�̕�����������₷���悤�ɕ\����ǉ�����
	VECTOR dispPos = ConvWorldPosToScreenPos(VGet(2, 0, 0));
	if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
	{
		DrawStringF(dispPos.x, dispPos.y, "X+", 0xffffff);
	}
	dispPos = ConvWorldPosToScreenPos(VGet(-2, 0, 0));
	if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
	{
		DrawStringF(dispPos.x, dispPos.y, "X-", 0xffffff);
	}

	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, 2));
	if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
	{
		DrawStringF(dispPos.x, dispPos.y, "Z+", 0xffffff);
	}
	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, -2));
	if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
	{
		DrawStringF(dispPos.x, dispPos.y, "Z-", 0xffffff);
	}
#endif // DEBUG

}
