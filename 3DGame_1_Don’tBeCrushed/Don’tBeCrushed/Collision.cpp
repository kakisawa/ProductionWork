#include "Collision.h"
#include "Game.h"
#include <cassert>

namespace {
	const VECTOR VecInit = VGet(0.0, 0.0, 0.0);
}

Collision::Collision() :
	p1{ VecInit },
	e1{ VecInit }
{
}

Collision::~Collision()
{
}

void Collision::PlayerDraw(unsigned int color, bool isFill)
{
	DrawCube3D(p1.m_leftBottom, p1.m_rightUp, color, 0x000000, isFill);

#ifdef DEBUG
	DrawFormatString(0, 420, 0x00ffff, "p1.m_rightBottom.x=%.0f:z=%0.f",
		p1.m_rightBottom.x, p1.m_rightUp.z);
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
#endif // DEBUG
}

void Collision::EnemyDraw(unsigned int color, bool isFill)
{
	DrawCube3D(e1.m_leftBottom, e1.m_rightUp, color, 0x000000, isFill);

#ifdef DEBUG
	DrawFormatString(0, 300, color, "e1.m_leftUp.x=%.0f:z=%0.f", e1.m_leftUp.x, e1.m_leftUp.z);
	DrawFormatString(0, 330, color, "e1.m_leftBottom.x=%.0f:z=%0.f", e1.m_leftBottom.x, e1.m_leftBottom.z);
	DrawFormatString(0, 360, color, "e1.m_rightBottom.x=%.0f:z=%0.f", e1.m_rightBottom.x, e1.m_rightBottom.z);
	DrawFormatString(0, 390, color, "e1.m_rightUp.x=%.0f:z=%0.f", e1.m_rightUp.x, e1.m_rightUp.z);
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
#endif // DEBUG	
}

void Collision::SetLB(VECTOR pos, float width, float height)
{
	p1.m_leftBottom = VSub(pos, VGet(width*0.5f, 0, height * 0.5f));	// 左下座標
	p1.m_leftUp = VAdd(p1.m_leftBottom, VGet(0.0f, 0.0f, height));		// 左上座標
	p1.m_rightUp = VAdd(pos, VGet(width * 0.5f, 0, height * 0.5f));		// 右上座標
	p1.m_rightBottom = VAdd(p1.m_leftBottom, VGet(height, 0.0f, 0.0f));	// 右下座標
}

/// <summary>
/// 縦向きの敵の(俯瞰して見た場合の)左下座標から右上座標をセット
/// </summary>
/// <param name="pos">敵座標</param>
/// <param name="width">本のサイズ(背表紙分)</param>
/// <param name="height">本のサイズ(縦の長さ)</param>
/// <param name="depth">本のサイズ(奥行/横幅)</param>
void Collision::SetPortrait(VECTOR pos, float width, float height, float depth, bool flag)
{	
	if (flag==false)
	{
		e1.m_leftUp = VAdd(pos, VGet(width, 0.0f, depth * 0.5f));
		e1.m_leftBottom = VAdd(e1.m_leftUp, VGet(0.0f, 0.0f, -depth));//
		e1.m_rightUp = VAdd(e1.m_leftUp, VGet(height, 0.0f, 0.0f));//
		e1.m_rightBottom = VAdd(e1.m_leftBottom, VGet(height, 0.0f, 0.0f));
	}
	else
	{
		e1.m_leftUp = VAdd(pos, VGet(-(height + width), 0.0f, depth*0.5f));
		e1.m_leftBottom = VAdd(e1.m_leftUp, VGet(0.0f, 0.0f, -depth));//
		e1.m_rightBottom = VAdd(e1.m_leftBottom, VGet(height, 0.0f, 0.0f));
		e1.m_rightUp = VAdd(e1.m_leftUp, VGet(height, 0.0f, 0.0f));//
		
	}
}

/// <summary>
/// 横向きの敵の(俯瞰して見た場合の)左下座標から右上座標をセット
/// </summary>
/// <param name="pos">敵座標</param>
/// <param name="width">本のサイズ(背表紙分)</param>
/// <param name="height">本のサイズ(縦の長さ)</param>
/// <param name="depth">本のサイズ(奥行/横幅)</param>
void Collision::SetLandscape(VECTOR pos, float width, float height, float depth, bool flag)
{
	if (!flag)
	{
		e1.m_leftUp = VAdd(pos, VGet(-(height*0.70f), 0.0f, -width*0.8f ));
		e1.m_leftBottom = VAdd(e1.m_leftUp, VGet(0.0f, 0.0f, -depth));
		e1.m_rightUp = VAdd(e1.m_leftUp, VGet(height*1.4f, 0.0f, 0.0f));
		e1.m_rightBottom = VAdd(e1.m_leftBottom, VGet(height * 1.4f, 0.0f, 0.0f));//
	}
	else
	{
		e1.m_leftBottom = VAdd(pos, VGet(-(height * 0.7f), 0.0f, width * 0.8f));
		e1.m_leftUp = VAdd(e1.m_leftBottom, VGet(0.0f, 0.0f, depth));
		e1.m_rightBottom = VAdd(e1.m_leftBottom, VGet(height*1.33f, 0.0f, 0.0f));
		e1.m_rightUp = VAdd(e1.m_rightBottom, VGet(0.0f, 0.0f, depth));
	}
}

bool Collision::IsCollision(int num, const Collision& col)
{
	if (num == 0)
	{
		if (col.e1.m_leftBottom.x >= p1.m_rightUp.x)
		{
			return false;
		}
		if (col.e1.m_rightBottom.x <= p1.m_leftUp.x)
		{
			return false;
		}
		if (col.e1.m_leftBottom.z >= p1.m_leftUp.z)
		{
			return false;
		}
	}
	if (num == 1)
	{
		if (col.e1.m_leftUp.x >= p1.m_rightUp.x)
		{
			return false;
		}
		if (col.e1.m_rightUp.x <= p1.m_leftUp.x)
		{
			return false;
		}
		if (col.e1.m_leftUp.z <= p1.m_leftBottom.z)
		{
			return false;
		}
	}
	if (num == 2)
	{
		if (col.e1.m_rightUp.x <= p1.m_leftUp.x)
		{
			return false;
		}
		if (col.e1.m_leftUp.z <= p1.m_leftBottom.z)
		{
			return false;
		}
		if (col.e1.m_leftBottom.z >= p1.m_leftUp.z)
		{
			return false;
		}

	}
	if (num == 3)
	{
		if (col.e1.m_leftUp.x >= p1.m_rightUp.x)
		{
			return false;
		}
		if (col.e1.m_leftUp.z <= p1.m_leftBottom.z)
		{
			return false;
		}
		if (col.e1.m_leftBottom.z >= p1.m_leftUp.z)
		{
			return false;
		}
	}

	return true;
}
