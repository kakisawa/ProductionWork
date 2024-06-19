#pragma once
#include "DxLib.h"

class Rect
{
public:
	Rect();
	virtual ~Rect();

	// 描画
	void PlayerDraw(unsigned int color, bool isFill);
	void EnemyDraw(unsigned int color, bool isFill);

	// (俯瞰して見た場合の)左下座標から右上座標をセット
	void SetLB(VECTOR pos, float width, float height);

	// 縦向きの敵の(俯瞰して見た場合の)左下座標から右上座標をセット
	void SetPortrait(VECTOR pos, float width, float height,float depth, bool flag);
	// 横向きの敵の(俯瞰して見た場合の)左下座標から右上座標をセット
	void SetLandscape(VECTOR pos, float width, float height,float depth, bool flag);

	//// 必要な情報を取得する
	//VECTOR GetWidth() const;	// 矩形の幅
	//VECTOR GetHeight() const;	// 矩形の高さ

private:

	struct Player {
		VECTOR m_leftUp;		// 左上のX座標
		VECTOR m_leftBottom;	// 左上のZ座標
		VECTOR m_rightUp;		// 右上のX座標
		VECTOR m_rightBottom;	// 右上のZ座標
	}p1;

	struct Enmey {
		VECTOR m_leftUp;		// 左上のX座標
		VECTOR m_leftBottom;	// 左上のZ座標
		VECTOR m_rightUp;		// 右上のX座標
		VECTOR m_rightBottom;	// 右上のZ座標
	}e1;

	
};

