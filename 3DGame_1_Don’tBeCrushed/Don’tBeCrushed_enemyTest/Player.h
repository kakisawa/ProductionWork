#pragma once
#include "DxLib.h"
#include "Rect.h"

class Player
{
public:
	Player();
	~Player();

	void Init();
	void Update();
	void Draw();
	void End();

private:
	int m_playerModel;		// プレイヤーモデル
	
	float m_playerScele;	// プレイヤーサイズ
	float m_speed;			// 移動速度

	VECTOR m_pos;			// プレイヤー位置
	VECTOR m_move;			// 移動量

	// 当たり判定矩形
	Rect m_colRect;
};