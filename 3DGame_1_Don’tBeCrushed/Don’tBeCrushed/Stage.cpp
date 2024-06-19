#include "Stage.h"
#include "Game.h"
#include "DxLib.h"

namespace {
	const char* const kBg = "data/floor2.png";

	constexpr int kStageLeftX = -30;
	constexpr int kStageRightX = 30;
	constexpr int kStageUpZ = 20;
	constexpr int kStageDownZ = -20;
}

Stage::Stage()
{
}

Stage::~Stage()
{
}

void Stage::Init()
{
	bgGraph = LoadGraph(kBg);
}

void Stage::Update()
{
}

void Stage::Draw()
{
	DrawExtendGraph(0, 0, kScreenWidth, kScreenHeight, bgGraph, false);

	for (int x = kStageLeftX; x <= kStageRightX; x += 10)
	{
		DrawLine3D(VGet(static_cast<float>(x), 0, kStageDownZ), VGet(static_cast<float>(x), 0, 20), 0xffffff);
	}
	for (int z = kStageDownZ; z <= kStageUpZ; z += 10)
	{
		DrawLine3D(VGet(kStageLeftX, 0, static_cast<float>(z)), VGet(kStageRightX, 0, static_cast<float>(z)), 0xffffff);
	}

	DrawSphere3D(VGet(kStageLeftX, 0, kStageUpZ), 2, 32, true, 0xff00ff, 0xff00ff);		// ¶ã
	DrawSphere3D(VGet(kStageLeftX, 0, kStageDownZ), 2, 32, true, 0xff00ff, 0xff00ff);	// ¶‰º
	DrawSphere3D(VGet(kStageRightX, 0, kStageUpZ), 2, 32, true, 0xff00ff, 0xff00ff);	// ‰Eã
	DrawSphere3D(VGet(kStageRightX, 0, kStageDownZ), 2, 32, true, 0xff00ff, 0xff00ff);	// ‰E‰º










	// X+-,Z+-‚Ì•ûŒü‚ª•ª‚©‚è‚â‚·‚¢‚æ‚¤‚É•\Ž¦‚ð’Ç‰Á‚·‚é
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
}
