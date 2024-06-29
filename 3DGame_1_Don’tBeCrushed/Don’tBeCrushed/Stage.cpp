#include "DxLib.h"
#include "Stage.h"
#include "Game.h"

namespace {
	const char* const kBg = "data/floor2.png";

	constexpr int kStageLeftX = -30;	// 左端座標X
	constexpr int kStageRightX = 30;	// 右端座標X
	constexpr int kStageUpZ = -20;		// 上端座標Z
	constexpr int kStageDownZ = 20;		// 下端座標Z

	// ステージ座標各位
	const VECTOR kStageLeftUp = VGet(kStageLeftX, 0, kStageUpZ);		// 左上座標
	const VECTOR kStageLeftDown = VGet(kStageLeftX, 0, kStageDownZ);	// 左下座標
	const VECTOR kStageRightUp = VGet(kStageRightX, 0, kStageUpZ);		// 右上座標
	const VECTOR kStageRightDown = VGet(kStageRightX, 0, kStageDownZ);	// 右下座標
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
	bgGraph = LoadGraph(kBg);	// 背景画像ロード
}

void Stage::Update()
{
}

void Stage::Draw()
{
	// 背景画像描画
	DrawExtendGraph(0, 0, kScreenWidth, kScreenHeight, bgGraph, false);

	// ステージ描画
	for (int x = kStageLeftX; x <= kStageRightX; x += 10)
	{
		DrawLine3D(VGet(static_cast<float>(x), 0, kStageUpZ), VGet(static_cast<float>(x), 0, 20), 0xffffff);
	}
	for (int z = kStageUpZ; z <= kStageDownZ; z += 10)
	{
		DrawLine3D(VGet(kStageLeftX, 0, static_cast<float>(z)), VGet(kStageRightX, 0, static_cast<float>(z)), 0xffffff);
	}

#ifdef DEBUG
	// ステージ四隅描画(デバッグ)
	DrawSphere3D(kStageLeftUp, 2, 32, true, 0xff00ff, 0xff00ff);		// 左上
	DrawSphere3D(kStageLeftDown, 2, 32, true, 0xff00ff, 0xff00ff);	// 左下
	DrawSphere3D(kStageRightUp, 2, 32, true, 0xff00ff, 0xff00ff);	// 右上
	DrawSphere3D(kStageRightDown, 2, 32, true, 0xff00ff, 0xff00ff);	// 右下

	// X+-,Z+-の方向が分かりやすいように表示を追加する
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
