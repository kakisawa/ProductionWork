#pragma once

class Input;
class Player;
class Stage;

/// <summary>
/// �J�����N���X
/// </summary>
class Camera
{
public:
	Camera();
	~Camera();
	void Init();
	void Update(Input& input, const Player& player);

	VECTOR GetAngle() const { return m_target; }
	VECTOR GetPos() const { return m_pos; }

private:
	void FixCameraPos();	// �J�����ʒu�␳

private:
	VECTOR m_pos;	 // �J�������W
	VECTOR m_target; // �����_���W
	float m_angleH;	 // �����p�x
	float m_angleV;	 // �����p�x
};

