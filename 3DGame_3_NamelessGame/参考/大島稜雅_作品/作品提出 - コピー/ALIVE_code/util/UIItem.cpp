#include "UIItem.h"
#include "FontsManager.h"
#include <cassert>

namespace
{
	//������𔼕��ɂ���
	constexpr int str_half = 2;

	//��ʂ𔼕��ɂ���
	constexpr int screen_half = 2;

	//������̊p�x
	constexpr float str_angle = 0.0f;

	//���S�ʒu
	constexpr float center_pos_x = 0.5f;
	constexpr float center_pos_y = 0.5f;

	//�\���T�C�Y
	constexpr float bill_board_size = 1.0f;
}

//�R���X�g���N�^
UIItem::UIItem()
{
}

//�f�X�g���N�^
UIItem::~UIItem()
{
}

//�T�C�Y�����A�A���t�@�l�̕ύX�������UI�`��
void UIItem::AlphaChangeDraw(const float scale, const int alpha)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraphF(centerPosX_, centerPosY_, scale, str_angle, makeScreenHandle_, true, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//�|�W�V�������ς��UI�̕`��
void UIItem::ChangePosDraw(const float centerPosX, const  float centerPosY)
{
	DrawRotaGraphF(centerPosX, centerPosY, bill_board_size, str_angle, makeScreenHandle_, true);
}

//�摜��3D��Ԃɕ`�悷��
void UIItem::DrawBillboard(const VECTOR& drawPos, const int alpha, const  float size)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBillboard3D(drawPos, center_pos_x, center_pos_y, size, str_angle, makeScreenHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//UI�摜���쐬���邽�߂̈ʒu��񓙂�ݒ肷��
void UIItem::CreateUIGraphSetUp(const float centerPosX, const float centerPosY, const int width, const int height, const std::string& str, const int fontHandle)
{
	makeScreenWidth_ = width;
	makeScreenHeight_ = height;
	centerPosX_ = centerPosX;
	centerPosY_ = centerPosY;

	UIString_ = str;

	makeScreenHandle_ = MakeScreen(makeScreenWidth_, makeScreenHeight_, true);
	CreateUIGraph(fontHandle);
}

//UI�摜�̍쐬
void UIItem::CreateUIGraph(const int fontHandle)
{
	assert(fontHandle != -1);

	//�Z�k��
	auto& fontManager = FontsManager::GetInstance();

	//�`��X�N���[���̕ύX
	SetDrawScreen(makeScreenHandle_);

	//������̉��̒������擾
	int strWidth = fontManager.GetStringSize(UIString_.c_str(), fontManager.GetFontName(fontHandle).c_str());

	//�t�H���g��K�p���������̕`��
	DrawStringToHandle(makeScreenWidth_ / screen_half - strWidth / str_half, makeScreenHeight_ / screen_half, UIString_.c_str(), 0xffffff, fontHandle);

	SetDrawScreen(DX_SCREEN_BACK);
}
