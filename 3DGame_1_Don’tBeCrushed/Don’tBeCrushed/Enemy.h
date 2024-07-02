#pragma once
#include "DxLib.h"
#include "Collision.h"

class Enemy
{
public:
	Enemy(VECTOR pos);
	~Enemy();

	void Init();
	void Update();
	void Draw();
	void End();

	void SetAttackNum(int num) { m_attackNum = num; }
	void SetAddModelScale(float scaleX, float scaleY) { m_addScaleX = scaleX, m_addScaleY = scaleY; }
	void SetAddMove(bool ismove) { m_isAddMove = ismove; }
	void SetAttack(bool isattack) { m_isAttack = isattack; }
	void SetModelAngle(VECTOR angle) { m_angle = angle; }
	void SetMove(VECTOR move) { m_move = move; }

	VECTOR GetPos() { return m_pos; }
	bool GetFall() { return m_isFall; }
	bool GetAttack() { return m_isAttack; }
	float GetRota() { return maxRota; }

private:

	int m_model;		// 敵モデル
	int m_modelBase;	// 敵モデル(ベース)
	int m_attackNum;	// 倒れる方向の番号
	int m_attackMoveNum;// 倒れる

	float m_scale;		// 敵サイズ
	float m_addScaleX;	// 敵サイズ(追加調整X分)
	float m_addScaleY;	// 敵サイズ(追加調整Y分)
	float maxRota;

	float m_gravity;	// 敵にかかる重力

	bool m_isAddMove;	// 敵が移動するかフラグ
	bool m_isAttack;	// 敵の攻撃フラグ
	bool m_isFall;		// 敵が倒れるフラグ
	bool m_isMoveStop;

	// 敵
	VECTOR m_pos;		// 敵座標
	VECTOR m_angle;		// 敵方向
	VECTOR m_move;		// 敵移動量
};

