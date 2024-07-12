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
	void FixCameraPos();	// �J�����ʒu�␳

private:
	float m_angleH;	 // �����p�x
	float m_angleV;	 // �����p�x

	DINPUT_JOYSTATE input;

	VECTOR m_pos;		// �J�������W
	VECTOR m_targetPos;	// �J�����^�[�Q�b�g���W
};