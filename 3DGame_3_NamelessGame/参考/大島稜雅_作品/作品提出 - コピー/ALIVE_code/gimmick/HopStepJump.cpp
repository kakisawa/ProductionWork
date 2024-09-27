#include "HopStepJump.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/Easing.h"

#include <string>
#include <stdio.h>
#include <algorithm>

namespace 
{
	//������l
	constexpr int add_select_num = 1;

	//�I��ԍ��̍ŏ��l
	constexpr int min_select_num = 0;

	//�����鎞��
	constexpr float add_time = 1.0f;

	//������
	constexpr float total_time = 180.0f;

	//���Ԃ̍ŏ��l
	constexpr float min_time = 0.0f;

	//�A���t�@�l��ύX���鎞��
	constexpr float alpha_change_time = 60.0f;

	//�A���t�@�l�̍ő�l
	constexpr float max_alpha_value = 1.0f;

	//�A���t�@�l�̍ŏ��l
	constexpr float min_alpha_value = 0.0f;
}

//�R���X�g���N�^
HopStepJump::HopStepJump(const int handle, const Material materialType, const ObjectInfo& objInfo):GimmickBase(handle,materialType,objInfo)
{
	//�}�e���A���̐��̎擾
	materialNum_ = MV1GetMaterialNum(handle);

	//�����蔻��̐ݒ�
	model_->SetUseCollision(true, true);

	//�����蔻��̐ݒ�
	isCollCheck_ = true;

	//���Ԃ̐ݒ�
	elapseddTime_ = total_time;

	//���݂̔ԍ�
	currentNum_ = StrUtil::GetNumberFromString(objInfo.name, ".");
}

//�f�X�g���N�^
HopStepJump::~HopStepJump()
{
}

//�X�V
void HopStepJump::Update(Player& player)
{
	//�^�C�}�[
	elapseddTime_ = (std::max)(elapseddTime_ - add_time, min_time);

	//���݂̔ԍ���ύX����
	ChangeCurrentNum();

	//�A���t�@�l��ύX����
	ChangeAlphaValue();

	//�}�e���A���̐F��ς���
	ChangeMaterialColor();

	//���݂̔ԍ��𕶎���ɕς���
	std::string num = std::to_string(currentNum_);

	//�����蔻��p�t���[���̕ύX
	model_->SetCollFrame(num.c_str());
}

//�`��
void HopStepJump::Draw()
{
	MV1DrawFrame(model_->GetModelHandle(), model_->GetColFrameIndex());
}

//���݂̔ԍ���ύX����
void HopStepJump::ChangeCurrentNum()
{
	//���݂̔ԍ���ύX����
	if (elapseddTime_ == min_time)
	{
		currentNum_ = (std::min)(currentNum_ + add_select_num, materialNum_);
		elapseddTime_ = total_time;
	}

	//�}�e���A���̐��𒴂�����0�ɂ���
	if (currentNum_ == materialNum_)
	{
		currentNum_ = min_select_num;
	}
}

//�A���t�@�l��ύX����
void HopStepJump::ChangeAlphaValue()
{
	//�A���t�@�l��ύX����
	if (elapseddTime_ >= total_time - alpha_change_time)
	{
		alphaValue_ = (std::min)(alphaValue_ + (max_alpha_value / alpha_change_time), max_alpha_value);
	}
	else if (elapseddTime_ <= alpha_change_time)
	{
		alphaValue_ = (std::max)(alphaValue_ - (max_alpha_value / alpha_change_time), min_alpha_value);
	}
}

//�}�e���A���̐F��ς���
void HopStepJump::ChangeMaterialColor()
{
	COLOR_F color = {};

	//���݂̔ԍ��ȊO�̃}�e���A���̃J���[�̃A���t�@�l��0�ɂ���
	for (int i = 0; i < materialNum_; i++)
	{
		//�}�e���A���̐F���擾����
		color = MV1GetMaterialDifColor(model_->GetModelHandle(), i);

		//�}�e���A���̃A���t�@�l��ύX����
		if (currentNum_ == i)
		{
			color.a = alphaValue_;
		}
		else
		{
			color.a = min_alpha_value;
		}

		//�}�e���A���̐F��ݒ肷��
		MV1SetMaterialDifColor(model_->GetModelHandle(), i, color);
	}
}
