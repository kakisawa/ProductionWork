#pragma once
#include "SceneBase.h"
#include <map>
#include <memory>
#include <string>
#include <DxLib.h>

class InputState;
class UIItemManager;

class SettingSceneForSceneTitle final : public SceneBase
{
private:
	//�A�b�v�f�[�g���s������
	enum class UpdateItem
	{
		WindowMode,
		BGM,
		SE,

		Max,
	};
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	SettingSceneForSceneTitle(SceneManager& manager);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SettingSceneForSceneTitle();

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �I��
	/// </summary>
	void End();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �E�B���h�E���[�h�̕`��
	/// </summary>
	/// <param name="alphaValue">�A���t�@�l</param>
	void DrawWindowMode(int alphaValue);

	/// <summary>
	/// SE�����o�[�̕`��
	/// </summary>
	/// <param name="alphaValue">�A���t�@�l</param>
	void DrawSEBar(int alphaValue);

	/// <summary>
	/// BGM�����o�[�̕`��
	/// </summary>
	/// <param name="alphaValue">�A���t�@�l</param>
	void DrawBGMBar(int alphaValue);

	/// <summary>
	/// �ʏ펞�̍X�V
	/// </summary>
	void NormalUpdate();

	/// <summary>
	/// �K�E�X�������{�����t�F�[�h�C��
	/// </summary>
	void GaussFadeInUpdate();

	/// <summary>
	/// �K�E�X�������{�����t�F�[�h�A�E�g
	/// </summary>
	void GaussFadeOutUpdate();

	/// <summary>
	/// BGM�̉��ʂ�ύX����
	/// </summary>
	void BGMUpdate();

	/// <summary>
	/// SE�̉��ʂ�ύX����
	/// </summary>
	void SEUpdate();

	/// <summary>
	/// �E�B���h�E���[�h��ύX����
	/// </summary>
	void ChangeWindowUpdate();

	/// <summary>
	/// updateFunc�̒��g��ύX����
	/// </summary>
	void ChangeUpdateFunc();

	/// <summary>
	/// �V�[����؂�ւ���
	/// </summary>
	void SceneChange();
private:

	int makeScreenHandle_ = -1;									//�쐬�����X�N���[���摜
	int volumeBGM_ = 0;											//BGM�̃{�����[��
	int volumeSE_ = 0;											//SE�̃{�����[��
	int selectNum_ = 0;											//�I��ԍ�

	bool windowMode_ = true;									//�E�B���h�E���[�h

	//������������������Ȃ�
	static constexpr int fadeInterval_ = 30;					//�C���^�[�o��
	int fadeTimer_ = 0;											//�t�F�[�h���鎞��
	int fadeValue_ = 0;											//�t�F�[�h�̓��ߒl
	int fadeColor_ = 0x000000;									//�w�i

	std::string windowModeText_ = "��  �E�B���h�E���[�h  ��";	//�E�B���h�E���[�h�̕\���e�L�X�g
	std::shared_ptr<UIItemManager> UIManager_;					//UI�}�l�[�W���[�̃|�C���^
	std::map<std::string, VECTOR> menuDrawPos_;					//���j���[��`�悷����W

	void(SettingSceneForSceneTitle::* updateFunc_)();			//�����o�֐��|�C���^
};

