#pragma once
#include "SceneBase.h"

/// <summary>
/// �^�C�g�����
/// </summary>
class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	virtual  ~SceneTitle();
	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();

private:
	void UpdateDisplay();

private:
	int m_time;					// ���݂̎���
	int m_textDisplayTime;		// �e�L�X�g��\�����鎞��
	float m_titleLogoScale;		// �^�C�g�����S�̊g�嗦
	float m_titleLogoRot;		// �^�C�g�����S�̉�]��
	int m_textAlpha;			// �e�L�X�g�̃A���t�@�l
	int m_opStartTime;			// �I�[�v�j���O������Đ�����܂ł̎���
	std::vector<int> m_handle;  // �n���h��

	// �n���h���̎��
	enum HandleKind
	{
		kTitleLogo,		// �^�C�g�����S
		kTitleLogoBack,	// �^�C�g�����S�̔w�i
		kPressText,		// "PRESSANYBUTTON"�̃e�L�X�g
		kOpMovie,		// OP����
		kHandleNum		// �n���h���̐�
	};
};