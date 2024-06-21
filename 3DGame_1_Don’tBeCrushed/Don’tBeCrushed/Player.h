#pragma once
#include "DxLib.h"
#include "Rect.h"
#include <memory>

class Stage;
class Player
{
public:
	Player();
	~Player();

	void Init(std::shared_ptr<Stage> pStage);
	void Update();
	void Draw();
	void End();

private:
	int m_playerModel;		// プレイヤーモデル

	struct StagePlace
	{
		VECTOR leftUp;		// ステージ左上座標
		VECTOR rightDown;	// ステージ右下座標
	}sp;
	
	float m_playerScele;	// プレイヤーサイズ
	float m_speed;			// 移動速度

	VECTOR m_pos;			// プレイヤー位置
	VECTOR m_move;			// 移動量

	Rect m_colRect;			// 当たり判定矩形
};