#pragma once
#include "DxLib.h"

class Camera
{
public:
	Camera();
	~Camera() {}

	void Init_Title(VECTOR pos);
	void Init_Game();

	void AddCameraPos(VECTOR pos) { cameraAddPos = pos; }

	void Update();

private:
	VECTOR m_pos;		// �J�������W
	VECTOR m_playerPos;
	VECTOR cameraPos;
	VECTOR cameraAddPos;
};

