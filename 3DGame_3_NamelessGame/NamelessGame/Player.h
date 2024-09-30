#pragma once
#include "DxLib.h"

class Camera;
class Player
{
public:
	Player();
	~Player();

	void Init();
	void Update(const Camera& camera);
	void Draw();

	void Move(const Camera& camera);

	/// <summary>
	/// 回転処理
	/// </summary>
	void Angle();

	VECTOR GetPos() const { return m_pos; }

private:
	int model;

	float m_angle;					// プレイヤー向き角度

	VECTOR m_pos;
	VECTOR m_move;			// 移動量
	VECTOR m_targetDir;		// プレイヤーが向くべき方向のベクトル
};