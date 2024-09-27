#include "UIItemManager.h"
#include "game.h"
#include "Util.h"
#include "UIItem.h"
#include "FontsManager.h"

namespace
{
	//UI�̈ʒu�␳X
	constexpr int correction_pos_x = 20;

	//UI�̈ʒu�␳X
	constexpr int correction_pos_y = 10;

	//UI�̏㉺�ɐU��镝�̕␳
	constexpr int correction_swing = 36;

	//blendMode�֐��̍ő�l
	constexpr float dxlib_max_value = 255.0f;

	//�A���t�@�l�̍ŏ��l
	constexpr float min_alpha_value = 150.0f;

	//�r���{�[�g�\���̃A���t�@�l�̍ŏ��l
	constexpr float bill_board_min_alpha_value = 100.0f;

	//�T�C�Y�̍ŏ��l
	constexpr float min_size_value = 0.7f;
}

//�R���X�g���N�^
UIItemManager::UIItemManager()
{
}

//�f�X�g���N�^
UIItemManager::~UIItemManager()
{
}

//�����ő����Ă����ԍ�����
//�g�k����ύX����Ƃ��̕`��
void UIItemManager::AlphaChangeDraw(const int selectNum, const int fadeAlphaValue)
{
	//�T�C�Y
	float scale = 1.0f;

	//�A���t�@�l
	float alpha = 0;

	//�I�����ꂽ������̃A���t�@�l
	float choiceAlphaValue = dxlib_max_value / dxlib_max_value;

	//�I������Ă��Ȃ�������̃A���t�@�l
	float notChoiceAlphaValue = min_alpha_value / dxlib_max_value;

	//�������`�悷��
	for (int i = 0; i < UIMenu_.size();i++) 
	{
		//�I�����ꂽ�����񂾂����ꍇ
		if (selectNum == i) 
		{
			//�T�C�Y��1�ɂ���
			scale = 1.0f;

			//�A���t�@�l��ύX����
			alpha = choiceAlphaValue * fadeAlphaValue;
		}
		else 
		{
			//�T�C�Y��0.7�ɂ���
			scale = min_size_value;

			//�A���t�@�l��ύX����
			alpha = notChoiceAlphaValue * fadeAlphaValue;
		}
		UIMenu_[i]->AlphaChangeDraw(scale, static_cast<int>(alpha));
	}
}

//�|�W�V��������ύX����UI�̕`��
void UIItemManager::ChangePosDraw(const float centerPosX, const float centerPosY)
{
	static int time = 0;
	float radian = 0.0f;

	time++;

	for (int i = 0; i < UIMenu_.size(); i++) 
	{
		//radian�̎擾
		radian = MathUtil::DegreeToRadian(static_cast<float>(time + (UIMenu_.size() - i) * correction_swing));
		float sin = std::sin(radian);

		//�������g�̂悤�ɓ����悤�ɂ���
		UIMenu_[i]->ChangePosDraw(centerPosX + i * correction_pos_x, centerPosY + sin * correction_pos_y);
	}
}

//�摜��3D��Ԃɕ`�悷��
void UIItemManager::DrawBillBoard(std::map<std::string, VECTOR> drawPos, const int alpha, const float size)
{
	for (int i = 0; i < UIMenu_.size(); i++)
	{
		//�摜�ɏ����ꂽ�������擾
		std::string name = UIMenu_[i]->GetString();

		//�摜��3D��Ԃɕ`�悷��
		UIMenu_[i]->DrawBillboard(drawPos[name], alpha, size);
	}
}

//�A���t�@�l��ύX�����摜��3D��Ԃɕ`�悷��
void UIItemManager::AlphaChangeDrawBillBoard(std::map<std::string, VECTOR> drawPos, const int selectNum, const int fadeValue, const float size)
{
	float alpha = 0;

	float currentNum = dxlib_max_value / dxlib_max_value;
	float notCurrentNum = bill_board_min_alpha_value / dxlib_max_value;

	for (int i = 0; i < UIMenu_.size(); i++)
	{

		if (selectNum == i)
		{
			alpha = currentNum * fadeValue;
		}
		else 
		{
			alpha = notCurrentNum * fadeValue;
		}

		//�摜�ɏ����ꂽ�������擾
		std::string name = UIMenu_[i]->GetString();

		//�摜��3D��Ԃɕ`�悷��
		UIMenu_[i]->DrawBillboard(drawPos[name], static_cast<int>(alpha), size);
	}
}

//UI���j���[�̒ǉ�
void UIItemManager::AddMenu(const float centerPosX, const float centerPosY, const int width, const int height,const std::string& str, const int fontHandle)
{
	//�C���X�^���X��
	UIMenu_.push_back(std::make_shared<UIItem>());

	//���j���[�̒ǉ�
	UIMenu_.back()->CreateUIGraphSetUp(centerPosX,centerPosY, width, height, str, fontHandle);
}

//������𕪊��������j���[�̒ǉ�
void UIItemManager::AddingMenuWithSplitStr(const float centerPosX, const float centerPosY, const int width, const int height,const std::string& str, const int fontHandle)
{
	//�Z�k��
	auto& font = FontsManager::GetInstance();

	//�����̐����擾
	int num = static_cast<int>(str.size());
	float size = 0.0f;

	for (int i = 0; i < num; i++)
	{
		//�ꕶ�����擾
		std::string letter = str.substr(i, 1);

		//�C���X�^���X��
		UIMenu_.push_back(std::make_shared<UIItem>());

		//�摜�̍쐬
		UIMenu_.back()->CreateUIGraphSetUp(centerPosX, centerPosY, width, height, letter, fontHandle);
	}
}

//�쐬���ꂽUI�̐����擾����
int UIItemManager::GetUINum()
{
	return static_cast<int>(UIMenu_.size());
}
