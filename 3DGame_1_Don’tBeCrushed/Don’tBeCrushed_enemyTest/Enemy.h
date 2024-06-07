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

	void SetAddModelScale(float scale) { m_addScale = scale; }
	void SetAddMove(bool ismove) { m_isAddMove = ismove; }
	void SetAttack(bool isattack) { m_isAttack = isattack; }
	void SetModelAngle(VECTOR angle) { m_angle = angle; }
	void SetMove(VECTOR move) { m_move = move; }
	VECTOR GetPos() { return m_pos; }
	
	
private:

	int m_model;		// 敵モデル
	int m_modelBase;	// 敵モデル(ベース)
	float m_scale;		// 敵サイズ
	float m_addScale;	// 敵サイズ(追加調整分)

	float m_gravity;		// 敵にかかる重力

	bool m_isAddMove;	// 敵が移動するかフラグ
	bool m_isAttack;	// 敵の攻撃フラグ
	bool m_isMoveStop;
	
	// 敵
	VECTOR m_pos;		// 敵座標
	VECTOR m_angle;		// 敵方向
	VECTOR m_move;		// 敵移動量
};

