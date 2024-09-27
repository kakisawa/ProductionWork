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

	const VECTOR& GetPosition() const { return m_pos; }
	const VECTOR& GetTarget()	const { return m_targetPos; }

private:
	float m_angleH;	 // �����p�x
	float m_angleV;	 // �����p�x

	DINPUT_JOYSTATE input;

	VECTOR m_pos;		// �J�������W
	VECTOR m_targetPos;	// �����_���W
};

