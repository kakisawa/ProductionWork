#pragma once
#include "DxLib.h"

class Rect
{
public:
	Rect();
	virtual ~Rect();

	// �`��
	void Draw(unsigned int color, bool isFill);

	// (���Ղ��Č����ꍇ��)�������W����E����W���Z�b�g
	void SetLB(VECTOR pos, float width, float height);

	//// �K�v�ȏ����擾����
	//VECTOR GetWidth() const;	// ��`�̕�
	//VECTOR GetHeight() const;	// ��`�̍���

private:
	VECTOR m_leftUp;	// �����X���W
	VECTOR m_leftBottom;	// �����Z���W
	VECTOR m_rightUp;	// �E���X���W
	VECTOR m_rightBottom; // �E���Z���W


	VECTOR m_leftBottom;	// �������W
	VECTOR m_rightUp;// �E����W
};

