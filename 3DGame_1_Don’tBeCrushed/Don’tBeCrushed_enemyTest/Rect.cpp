#include "Rect.h"
#include "Game.h"
#include <cassert>

namespace {
	const VECTOR VecInit = VGet(0.0, 0.0, 0.0);
}

Rect::Rect() :
	p1{ VecInit },
	e1{ VecInit }
{
}

Rect::~Rect()
{
}

void Rect::PlayerDraw(unsigned int color, bool isFill)
{
	DrawCube3D(p1.m_leftBottom, p1.m_rightUp, color, 0x000000, isFill);

	

	// プレイヤー座標確認用
	DrawString(0, 800 - 20, "Player", 0xffffff);
	DrawFormatString(0, 800, 0xffffff, "m_leftBottom.x=%.2f,z=%.2f",
		p1.m_leftBottom.x, p1.m_leftBottom.z);
	DrawFormatString(0, 820, 0xffffff, "m_leftUp.x = % .2f, z = % .2f",
		p1.m_leftUp.x, p1.m_leftUp.z);
	DrawFormatString(0, 840, 0xffffff, "m_rightBottom.x=%.2f,z=%.2f",
		p1.m_rightBottom.x, p1.m_rightBottom.z);
	DrawFormatString(0, 860, 0xffffff, "m_rightUp.x=%.2f,z=%.2f",
		p1.m_rightUp.x, p1.m_rightUp.z);
}

void Rect::EnemyDraw(unsigned int color, bool isFill)
{
	DrawCube3D(e1.m_leftBottom, e1.m_rightUp, color, 0x000000, isFill);

	// 敵座標確認用
	DrawString(0, 500 - 20, "Enemy", 0xffffff);
	DrawFormatString(0, 500, 0xffffff, "m_leftBottom.x=%.2f,z=%.2f",
		e1.m_leftBottom.x, e1.m_leftBottom.z);
	DrawFormatString(0, 520, 0xffffff, "m_leftUp.x = % .2f, z = % .2f",
		e1.m_leftUp.x, e1.m_leftUp.z);
	DrawFormatString(0, 540, 0xffffff, "m_rightBottom.x=%.2f,z=%.2f",
		e1.m_rightBottom.x, e1.m_rightBottom.z);
	DrawFormatString(0, 560, 0xffffff, "m_rightUp.x=%.2f,z=%.2f",
		e1.m_rightUp.x, e1.m_rightUp.z);
}

void Rect::SetLB(VECTOR pos, float width, float height)
{
	p1.m_leftBottom = VSub(pos, VGet(width*0.5f, 0, height * 0.5f));	// 左下座標
	p1.m_leftUp = VAdd(p1.m_leftBottom, VGet(0.0f, 0.0f, height));			// 左上座標
	p1.m_rightUp = VAdd(pos, VGet(width * 0.5f, 0, height * 0.5f));	// 右上座標
	p1.m_rightBottom = VAdd(p1.m_leftBottom, VGet(height, 0.0f, 0.0f));	// 右下座標
}

/// <summary>
/// 縦向きの敵の(俯瞰して見た場合の)左下座標から右上座標をセット
/// </summary>
/// <param name="pos">敵座標</param>
/// <param name="width">本のサイズ(背表紙分)</param>
/// <param name="height">本のサイズ(縦の長さ)</param>
/// <param name="depth">本のサイズ(奥行/横幅)</param>
void Rect::SetPortrait(VECTOR pos, float width, float height, float depth, bool flag)
{	
	if (flag==false)
	{
		e1.m_leftUp = VAdd(pos, VGet(width, 0.0f, depth * 0.5f));
		e1.m_leftBottom = VAdd(e1.m_leftUp, VGet(0.0f, 0.0f, -depth));
		e1.m_rightUp = VAdd(e1.m_leftUp, VGet(height, 0.0f, 0.0f));
		e1.m_rightBottom = VAdd(e1.m_leftBottom, VGet(height, 0.0f, 0.0f));
	}
	else
	{

	}
	
}

/// <summary>
/// 横向きの敵の(俯瞰して見た場合の)左下座標から右上座標をセット
/// </summary>
void Rect::SetLandscape(VECTOR pos, float width, float height, float depth)
{
}



//VECTOR Rect::GetWidth() const
//{
//	assert(m_right >= m_left);	// 左右の座標入れ替わりチェック
//	return VSub(m_right, m_left);
//}
//
//VECTOR Rect::GetHeight() const
//{
//	assert(m_bottom >= m_top);	// 上下の座標入れ替わりチェック
//	return VSub(m_bottom, m_top);
//}
