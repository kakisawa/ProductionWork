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

	void GetModelAngle(VECTOR angle) { m_angle = angle; }
	void GetAddModelScale(int scale) { m_addScale = scale; }
	void GetMove(VECTOR move) { m_move = move; }
	
private:

	int m_model;
	int m_modelBase;
	float m_scale;
	float m_addScale;
	
	
	// “G
	VECTOR m_pos;
	VECTOR m_angle;
	VECTOR m_move;
};

