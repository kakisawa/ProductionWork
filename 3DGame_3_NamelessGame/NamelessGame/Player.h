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
	/// ��]����
	/// </summary>
	void Angle();

	VECTOR GetPos() const { return m_pos; }

private:
	int model;

	float m_angle;					// �v���C���[�����p�x

	VECTOR m_pos;
	VECTOR m_move;			// �ړ���
	VECTOR m_targetDir;		// �v���C���[�������ׂ������̃x�N�g��
};