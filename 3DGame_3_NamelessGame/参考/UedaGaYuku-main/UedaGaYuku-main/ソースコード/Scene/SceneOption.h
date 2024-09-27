#pragma once
#include "SceneBase.h"

/// <summary>
/// �I�v�V�������
/// </summary>
class SceneOption : public SceneBase
{
public:
	SceneOption():m_afterSelect(0), m_isSound(false),m_isWindow(false){};
	SceneOption(std::shared_ptr<SceneBase> pScene);
	virtual ~SceneOption();
	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();

private:
	void UpdateSound(Input& input);				// �T�E���h�X�V
	void UpdateWindowMode(Input& input);		// ��ʃT�C�Y�̕ύX
	void DrawSound();							// �T�E���h�����\��
	void DrawWindowMode();						// ��ʃT�C�Y�����\��
	void DrawCredit();							// �N���W�b�g�\�L

private:
	std::shared_ptr<SceneBase> m_pPrevScene;	// �O�Ɏ��s���Ă����V�[��
	int m_afterSelect;							// �I����̏��
	bool m_isSound;								// �T�E���h���I�𒆂�(true:�I��)
	bool m_isWindow;							// ��ʃT�C�Y���I�𒆂�(true:�I��)
	std::vector<int> m_handle;					// �T�E���h�o�[�Ɏg�p����摜

	// �I����
	enum Select
	{
		kSound,			// �T�E���h
		kWindow,		// ��ʃT�C�Y
		kCredit,		// �N���W�b�g�\�L
		kSelectNum		// �I��
	};

	// �T�E���h�I����
	enum SelectSound
	{
		kBGM,			// BGM
		kSE,			// SE
		kSelectSoundNum	// �I��
	};

	// �E�B���h�E���[�h�I����
	enum SelectWindow
	{
		kFullScreen,	// �t���X�N���[��
		kWindowMode,	// �E�B���h�E���[�h
		kSelectWinNum	// �I��
	};
};

