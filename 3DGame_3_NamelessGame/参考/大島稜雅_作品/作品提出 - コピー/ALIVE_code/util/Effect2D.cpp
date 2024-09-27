#include "Effect2D.h"
#include <algorithm>

namespace
{
	//������
	constexpr int total_time = 60;

	//���S���W
	constexpr float center_pos_x = 0.5f;
	constexpr float center_pos_y = 0.5f;
}

//�R���X�g���N�^
Effect2D::Effect2D(const std::vector<int>& handle,const float size,const VECTOR& pos)
{
	//�n���h���̎擾
	handle_ = handle;

	//���݂��Ă���t���O�����Ă�
	isEnabled_ = true;

	//�`��T�C�Y
	drawSize_ = size;

	//�`��ꏊ
	drawPos_ = pos;
}

//�f�X�g���N�^
Effect2D::~Effect2D()
{
}

//�X�V
void Effect2D::Update()
{
	//�摜�̍X�V
	currentNum_ = (std::min)(currentNum_ + 1, total_time);

	//���݂̔ԍ����摜�̖����Ɠ����ɂȂ����瑶�݂�����
	if (currentNum_ == handle_.size() - 1) {
		isEnabled_ = false;
	}
}

//�摜��3D��Ԃɕ`�悷��
void Effect2D::DrawBillboard()
{
	DrawBillboard3D(drawPos_, center_pos_x, center_pos_y, drawSize_, 0.0f, handle_[currentNum_], true);
}
