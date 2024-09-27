#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"
#include "Font.h"
#include "UI.h"
#include <algorithm>

// �萔
namespace
{
	/*�w�i�֘A*/
	constexpr int kBackColor = 0xdddddd;				// �w�i�̐F
	constexpr int kBackBoxColor = 0x494949;				// �l�p�̐F
	constexpr int kBackBoxLTPos = 140;					// �l�p�̍���ʒu
	constexpr int kBackBoxWidth = 490;					// �l�p�̕�
	const Vec2 kBackHandleLTPos = { 750.0f, 200.0f };	// �w�i�摜�̍���ʒu
	const Vec2 kBackHandleRBPos = { 1779.0f, 670.0f };	// �w�i�摜�̉E���ʒu
	constexpr int kMulaPal = 255;						// ��Z�l

	const Vec2 kTextBoxLTPos = { 750.0f, 700.0f };		// �X�e�[�W�I����ʂ̃e�L�X�g�{�b�N�X����ʒu
	const Vec2 kTextBoxRBPos = { 1779.0f, 890.0f };		// �X�e�[�W�I����ʂ̃e�L�X�g�{�b�N�X�E���ʒu

	const Vec2 kPauseBackLTPos = { 600.0f, 200.0f };	// �|�[�Y��ʂ̔w�i�摜����ʒu
	const Vec2 kPauseBackRBPos = { 1320.0f, 880.0f };	// �|�[�Y��ʂ̔w�i�摜�E���ʒu

	const Vec2 kClearTimeBgFrameLTPos = { 170, 420 };	// �N���A���(�N���A�^�C������)�̔w�i�摜����ʒu
	const Vec2 kClearTimeBgFrameRBPos = { 900, 950 };	// �N���A���(�N���A�^�C������)�̔w�i�摜�E���ʒu
	const Vec2 kClearRankBgFrameLTPos = { 950, 420 };	// �N���A���(�����L���O����)�̔w�i�摜����ʒu
	const Vec2 kClearRankBgFrameRBPos = { 1800, 950 };	// �N���A���(�����L���O����)�̔w�i�摜�E���ʒu

	const Vec2 kGameoverBgFrameLTPos = { 620, 580 };	// �Q�[���I�[�o�[��ʂ̔w�i�摜����ʒu
	const Vec2 kGameoverBgFrameRBPos = { 1300, 1000 };	// �Q�[���I�[�o�[��ʂ̔w�i�摜�E���ʒu

	/*�J�[�\���֘A*/
	constexpr float kCursorWidth = 489.0f;				// �J�[�\���̉���
	constexpr float kOptionCursorWidth = 1000.0f;		// �I�v�V������ʂ̃J�[�\���̉���
	constexpr float kCursorHeight = 90.0f;				// �J�[�\���̏c��
	constexpr float kCursorSpeed = 60.0f;				// �J�[�\���̉����̐L�т��
	constexpr int kTextDisplayTime = 2;					// �J�[�\���̕\���Ԋu
	constexpr int kTextDisplayAnimTime = 240;			// �J�[�\���A�j���[�V�����̎���
	constexpr int kMaxAlpha = 255;						// �ő�A���t�@�l
	constexpr int kMinAlpha = 40;						// �ŏ��A���t�@�l

	/*�{�^���̉摜�ƃe�L�X�g�̈ʒu*/
	const Vec2 kButtonPos = { 1500, 1000 };				// �{�^���\���ʒu
	const Vec2 kButtonTextPos = { 1530, 970 };			// �e�L�X�g�̈ʒu
	const Vec2 kTitleButtonPos = { 1605, 1005 };		// �^�C�g����ʃ{�^���ʒu
	const Vec2 kTitleButtonTextPos = { 1630, 980 };		// �^�C�g����ʃ{�^���e�L�X�g�ʒu
	const Vec2 kClearButtonPos = { 1450, 1000 };		// �N���A���{�^���\���ʒu
	const Vec2 kClearButtonTextPos = { 1480, 975 };		// �N���A���̃e�L�X�g�ʒu
	constexpr float kButtonWidth = 180.0f;				// �\������{�^���̕�
	constexpr int kButtonSize = 32;						// �{�^���摜�̃T�C�Y
	constexpr float kButtonScale = 1.5f;				// �{�^���̊g�嗦
	constexpr float kButtonTextWidth = 170.0f;			// �e�L�X�g�̕�
	constexpr int kTextColor = 0xffffff;				// �e�L�X�g�̐F
	constexpr int kTitleTextColor = 0x000000;			// �^�C�g�������̃e�L�X�g�̐F
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
UI::UI():
	m_isOption(false),
	m_cursorWidth(0.0f),
	m_cursorAlpha(kMaxAlpha)
{
	m_buttonHandle = LoadGraph("data/UI/button.png");
	m_handle.resize(HandleKind::kHandleNum);
	m_handle[HandleKind::kCursor] = LoadGraph("data/UI/cursor.png");
	m_handle[HandleKind::kBg] = LoadGraph("data/UI/back.png");
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
UI::~UI()
{
	DeleteGraph(m_buttonHandle);
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
}


/// <summary>
/// ������
/// </summary>
void UI::Init()
{
	m_cursorWidth = 0.0f;
}


/// <summary>
/// �X�V
/// </summary>
void UI::Update()
{
	// �J�[�\���̉������X�V
	// �I�v�V������ʂ̏ꍇ�̂݃J�[�\���̒�����ς���
	m_cursorWidth += kCursorSpeed;
	if (m_isOption)
	{
		m_cursorWidth = std::min(m_cursorWidth, kOptionCursorWidth);
	}
	else
	{
		m_cursorWidth = std::min(m_cursorWidth, kCursorWidth);
	}

	// �J�[�\���̃��l���X�V
	m_cursorDisplayTime += kTextDisplayTime;
	m_cursorDisplayTime %= kTextDisplayAnimTime;
	// MEMO:sin�g���g����0�`1�͈̔͂ɂ���
	float sinAlpha = 0.5f + 0.5f * sinf(static_cast<float>(m_cursorDisplayTime) / kTextDisplayAnimTime * DX_PI_F);
	m_cursorAlpha = kMinAlpha + static_cast<int>((kMaxAlpha - kMinAlpha) * sinAlpha);
}


/// <summary>
///  �J�[�\���\��
/// </summary>
/// <param name="pos">�\���ʒu</param>
/// <param name="select">�I�����</param>
/// <param name="interval">�\���Ԋu</param>
void UI::DrawCursor(Vec2 pos, int select, float interval, bool isOption)
{
	m_isOption = isOption;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_cursorAlpha);
	DrawExtendGraphF(pos.x, pos.y + interval * select,
		pos.x + m_cursorWidth, pos.y + interval * select + kCursorHeight,
		m_handle[HandleKind::kCursor], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


/// <summary>
/// �|�[�Y��ʂ̔w�i�\��
/// </summary>
void UI::DrawPauseBack()
{
	// �w�i�摜�\��
	DrawExtendGraphF(kPauseBackLTPos.x, kPauseBackLTPos.y, kPauseBackRBPos.x, kPauseBackRBPos.y, m_handle[HandleKind::kBg], true);
}


/// <summary>
/// ���j���[�̔w�i�\��
/// </summary>
void UI::DrawMenuBg()
{
	// �w�i�`��
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, kBackColor, true);
	// �w�i�̎l�p�����\��
	DrawBox(kBackBoxLTPos, 0, kBackBoxLTPos + kBackBoxWidth, Game::kScreenHeight, kBackBoxColor, true);
	// �w�i�摜�\��
	DrawExtendGraphF(kBackHandleLTPos.x, kBackHandleLTPos.y, kBackHandleRBPos.x, kBackHandleRBPos.y, m_handle[HandleKind::kBg], true);
	// �e�L�X�g�{�b�N�X�\��
	DrawExtendGraphF(kTextBoxLTPos.x, kTextBoxLTPos.y, kTextBoxRBPos.x, kTextBoxRBPos.y, m_handle[HandleKind::kBg], true);
}


/// <summary>
/// �{�^���̉摜�ƃe�L�X�g��\������
/// </summary>
void UI::DrawButtonText()
{
	// �e�L�X�g�\��
	DrawStringFToHandle(kButtonTextPos.x, kButtonTextPos.y,
		"����", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kButtonText)]);
	DrawStringFToHandle(kButtonTextPos.x + kButtonTextWidth, kButtonTextPos.y,
		"���ǂ�", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kButtonText)]);

	// �{�^���摜�\��
	DrawRectRotaGraphF(kButtonPos.x, kButtonPos.y, kButtonSize * ButtonKind::kAButton, 0, kButtonSize, kButtonSize, kButtonScale, 0.0f, m_buttonHandle, true);
	DrawRectRotaGraphF(kButtonPos.x + kButtonWidth, kButtonPos.y, kButtonSize * ButtonKind::kBButton, 0, kButtonSize, kButtonSize, kButtonScale, 0.0f, m_buttonHandle, true);
}


/// <summary>
/// �^�C�g����ʂŃ{�^���̉摜�ƃe�L�X�g��\������
/// </summary>
void UI::DrawTitleButtonText()
{
	// �G�b�W�t���e�L�X�g�\��
	DrawFormatString2FToHandle(kTitleButtonTextPos.x, kTitleButtonTextPos.y, kTitleTextColor, kTextColor, 
		Font::m_fontHandle[static_cast<int>(Font::FontId::kTitleButtonText)], "�ŃX�L�b�v");

	// �{�^���摜�\��
	DrawRectRotaGraphF(kTitleButtonPos.x, kTitleButtonPos.y, kButtonSize * ButtonKind::kAButton, 0, kButtonSize, kButtonSize, kButtonScale, 0.0f, m_buttonHandle, true);
}


/// <summary>
/// �N���A��ʂŃ{�^���̉摜�ƃe�L�X�g�\������
/// </summary>
void UI::DrawClearButtonText()
{
	// �e�L�X�g�\��
	DrawStringFToHandle(kClearButtonTextPos.x, kClearButtonTextPos.y,
		"�X�e�[�W�I���ɂ��ǂ�", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kClearButtonText)]);

	// �{�^���摜�\��
	DrawRectRotaGraphF(kClearButtonPos.x, kClearButtonPos.y, kButtonSize * ButtonKind::kAButton, 0, kButtonSize, kButtonSize, kButtonScale, 0.0f, m_buttonHandle, true);
}


/// <summary>
/// �N���A��ʂ̔w�i�g�\��
/// </summary>
void UI::DrawClearBgFrame()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, kMulaPal);
	DrawExtendGraphF(kClearTimeBgFrameLTPos.x, kClearTimeBgFrameLTPos.y, kClearTimeBgFrameRBPos.x, kClearTimeBgFrameRBPos.y, m_handle[HandleKind::kBg], true);
	DrawExtendGraphF(kClearRankBgFrameLTPos.x, kClearRankBgFrameLTPos.y, kClearRankBgFrameRBPos.x, kClearRankBgFrameRBPos.y, m_handle[HandleKind::kBg], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


/// <summary>
/// �Q�[���I�[�o�[�̔w�i�g�\��
/// </summary>
void UI::DrawGameoverBgFrame()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, kMulaPal);
	DrawExtendGraphF(kGameoverBgFrameLTPos.x, kGameoverBgFrameLTPos.y, kGameoverBgFrameRBPos.x, kGameoverBgFrameRBPos.y, m_handle[HandleKind::kBg], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
