#pragma once
#include "DxLib.h"

class Rect
{
public:
	Rect();
	virtual ~Rect();

	// 描画
	void Draw(unsigned int color, bool isFill);

	// (俯瞰して見た場合の)左下座標から右上座標をセット
	void SetLB(VECTOR pos, float width, float height);

	//// 必要な情報を取得する
	//VECTOR GetWidth() const;	// 矩形の幅
	//VECTOR GetHeight() const;	// 矩形の高さ

private:
	VECTOR m_leftUp;	// 左上のX座標
	VECTOR m_leftBottom;	// 左上のZ座標
	VECTOR m_rightUp;	// 右上のX座標
	VECTOR m_rightBottom; // 右上のZ座標


	VECTOR m_leftBottom;	// 左下座標
	VECTOR m_rightUp;// 右上座標
};

