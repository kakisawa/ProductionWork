#pragma once
#include "Vec2.h"

/// <summary>
/// UI�̕\���Ɋւ���N���X
/// </summary>
class UI
{
public:
	UI();
	virtual ~UI();
	void Init();
	void Update();
	void DrawCursor(Vec2 pos, int select, float interval, bool isOption = false); // �J�[�\���\��
	void DrawPauseBack();		// �|�[�Y��ʂ̔w�i�\��
	void DrawMenuBg();			// ���j���[�̔w�i�\��
	void DrawButtonText();		// �{�^���e�L�X�g�\��
	void DrawTitleButtonText();	// �^�C�g����ʂł̃{�^���e�L�X�g�\��
	void DrawClearButtonText();	// �N���A��ʂł̃{�^���e�L�X�g�\��
	void DrawClearBgFrame();	// �N���A��ʂ̔w�i�g�\��
	void DrawGameoverBgFrame();	// �Q�[���I�[�o�[��ʂ̔w�i�g�\��


protected:
	int m_buttonHandle;		// �{�^���̉摜

	// �{�^���̎��
	enum ButtonKind
	{
		kAButton,		// A
		kBButton,		// B
		kXButton,		// X
		kYButton,		// Y
		kMenuButton,	// ���k�[
		kViewButton,	// �r���[
		kLStick,		// ���X�e�B�b�N
		kRStick,		// �E�X�e�B�b�N
		kLBButton,		// LB
		kRBButton,		// RB
	};

private:
	bool m_isOption;			// �I�v�V������ʂ��ǂ���
	float m_cursorWidth;		// �J�[�\���̉���
	int m_cursorDisplayTime;	// �J�[�\����\�����鎞��
	int m_cursorAlpha;			// �J�[�\���̃��l
	std::vector<int> m_handle;  // �n���h��

	// �摜�̎��
	enum HandleKind
	{
		kCursor,	// �J�[�\��
		kBg,		// �w�i
		kHandleNum	// �n���h���̎��
	};
};