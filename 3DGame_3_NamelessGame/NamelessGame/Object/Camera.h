#pragma once
#include "DxLib.h"

class Player;
class Camera
{
public:
	Camera();
	virtual ~Camera() {}

	void Init();

	void Update(const Player& player);
	void FixCameraPos(const Player& player);	// カメラ位置補正

	void RightstickCameraUpdate();	// 右スティックを使用したカメラの更新
	void LeftstickCameraUpdate(const Player& player);	// 左スティックを使用したカメラの更新


	const VECTOR& GetPosition() const { return m_pos; }			// 座標渡し
	const VECTOR& GetTarget()	const { return m_targetPos; }	// 注視点座標渡し

private:
	float m_angleH;	 // 水平角度
	float m_angleV;	 // 垂直角度

	DINPUT_JOYSTATE input;
	DINPUT_JOYSTATE input2;

	VECTOR m_pos;		// カメラ座標
	VECTOR m_targetPos;	// 注視点座標
	VECTOR m_enemyTargetPos;	
};