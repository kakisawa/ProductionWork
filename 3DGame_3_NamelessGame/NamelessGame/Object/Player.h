#pragma once
#include "DxLib.h"
#include <string>

class Camera;
class Player
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Player();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Player();

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="camera">�J�����Q��</param>
	void Update(const Camera& camera);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="camera">�J�����Q��</param>
	void Move(const Camera& camera);

	void leftStick(VECTOR moveUp,VECTOR moveLeft);

	/// <summary>
	/// ��]����
	/// </summary>
	void Angle();

	VECTOR GetPos() const { return m_pos; }	// ���W�n��

	// �v���C���[�f�[�^
	struct CharaData
	{
		int maxHp;		// �ő�HP
		float initPosX;	// ���������WX
		float initPosY; // ���������WY
		float initPosZ; // ���������WZ
		float modelSize;// ���f���T�C�Y
		float walkSpeed;// �������x
		float rotaSpeed;// ��]���x
	}m_chara;

private:
	int model;				// ���f��
	float m_angle;			// �v���C���[�����p�x


	int inputX, inputY;



	VECTOR m_pos;			// ���W
	VECTOR m_move;			// �ړ���
	VECTOR m_targetDir;		// �v���C���[�������ׂ������̃x�N�g��

	DINPUT_JOYSTATE input;
};