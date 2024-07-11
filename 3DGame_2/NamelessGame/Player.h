#pragma once
#include "DxLib.h"

class Player
{
public:
	Player();
	~Player();

	void Init();
	void Update();
	void Draw();
	void End();

	VECTOR GetPos()const { return m_pos; }

private:
	int m_model;		// プレイヤーモデル

	float m_playerScele;	// プレイヤーサイズ
	
	VECTOR m_pos;			// プレイヤー位置
	VECTOR m_move;			// 移動量
};