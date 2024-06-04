#include "Stage.h"
#include "Game.h"
#include "DxLib.h"

namespace {
	const char* const kBg = "data/floor2.png";
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

	for (int x = -30; x <= 30; x += 10)
	{
		DrawLine3D(VGet(static_cast<float>(x), 0, -20), VGet(static_cast<float>(x), 0, 20), 0xffffff);
	}
	for (int z = -20; z <= 20; z += 10)
	{
		DrawLine3D(VGet(-30, 0, static_cast<float>(z)), VGet(30, 0, static_cast<float>(z)), 0xffffff);
	}

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
