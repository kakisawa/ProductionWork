#include "Rect.h"
#include "Game.h"
#include <cassert>

namespace {
	const VECTOR VecInit = VGet(0.0, 0.0, 0.0);
}

Rect::Rect() :
	m_leftB(VecInit),
	m_rightU(VecInit),
	m_leftUp(VecInit),
	//m_leftBottom(VecInit),
	//m_rightUp(VecInit),
	m_rightBottom(VecInit)
{
}

Rect::~Rect()
{
}

void Rect::Draw(unsigned int color, bool isFill)
{
	DrawCube3D(m_leftB, m_rightU, color, 0x000000, isFill);
}

void Rect::SetLB(VECTOR pos, float width, float height)
{
	m_leftUp=VAdd(m_leftB, VGet(0.0f, 0.0f, height));

	m_leftB = VSub(pos, VGet(width * 0.5f, 0, height * 0.5f));
	m_rightU = VAdd(pos, VGet(width * 0.5f, 0, height * 0.5f));

	m_rightBottom;

	//m_leftBottom;
	//m_rightUp;
	
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
