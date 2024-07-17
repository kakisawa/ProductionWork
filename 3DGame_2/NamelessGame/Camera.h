#pragma once
#include "DxLib.h"

class Player;
class Camera
{
public:
	Camera();
	~Camera() {}

	void Init();

	void Update(const Player& player);
	void FixCameraPos();	// カメラ位置補正

	const VECTOR& GetPosition() const { return m_pos; }
	const VECTOR& GetTarget()	const { return m_targetPos; }

private:
	float m_angleH;	 // 水平角度
	float m_angleV;	 // 垂直角度

	DINPUT_JOYSTATE input;

	VECTOR m_pos;		// カメラ座標
	VECTOR m_targetPos;	// 注視点座標
};