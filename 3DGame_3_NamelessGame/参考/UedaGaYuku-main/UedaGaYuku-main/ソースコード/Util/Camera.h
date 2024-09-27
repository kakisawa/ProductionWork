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
	void StartProduction();	// �X�^�[�g���̃J�������o

	VECTOR GetAngle() const { return m_target; }
	VECTOR GetPos() const { return m_pos; }

private:
	void FixCameraPos();	// �J�����ʒu�␳

private:
	VECTOR m_pos;					// �J�������W
	VECTOR m_target;				// �����_���W
	float m_angleH;					// �����p�x
	float m_angleV;					// �����p�x
	int m_lightHandle;				// ���C�g�n���h��
	int m_startProductionTime;		// �X�^�[�g���o����
	DINPUT_JOYSTATE AnalogInput; 	// �p�b�h�A�i���O���擾
};

