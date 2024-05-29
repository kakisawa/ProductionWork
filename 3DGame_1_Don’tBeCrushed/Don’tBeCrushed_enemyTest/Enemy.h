#pragma once
#include "DxLib.h"

class Enemy
{
public:
	Enemy(VECTOR pos);
	~Enemy();

	void Init();
	void Update();
	void Draw();
	void End();

	void GetAddModelScale(float scale) { m_addScale = scale; }
	void GetAddMove(bool ismove) { m_isAddMove = ismove; }
	void GetModelAngle(VECTOR angle) { m_angle = angle; }
	void GetMove(VECTOR move) { m_move = move; }
	
private:

	int m_model;		// 敵モデル
	int m_modelBase;	// 敵モデル(ベース)
	float m_scale;		// 敵サイズ
	float m_addScale;	// 敵サイズ(追加調整分)
	bool m_isAddMove;	// 敵が移動するか
	
	// 敵
	VECTOR m_pos;		// 敵座標
	VECTOR m_angle;		// 敵方向
	VECTOR m_move;		// 敵移動量
};

