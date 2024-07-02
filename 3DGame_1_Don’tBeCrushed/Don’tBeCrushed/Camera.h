#pragma once
#include "DxLib.h"

class Camera
{
public:
	Camera();
	~Camera() {}

	void Init_Title(VECTOR pos);
	void Init_Game();

	void AddCameraAngle(float angle) { m_cameraAngle = angle; }

	void Update();

private:
	float m_cameraAngle;

	VECTOR m_pos;		// ÉJÉÅÉâç¿ïW
	VECTOR m_playerPos;
	VECTOR cameraPos;
};

