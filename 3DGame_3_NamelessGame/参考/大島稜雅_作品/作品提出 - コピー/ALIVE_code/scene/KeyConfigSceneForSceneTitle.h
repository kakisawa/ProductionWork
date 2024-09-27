#pragma once
#include "SceneBase.h"
#include "../util/KeyData.h"
#include <memory>
#include <map>
#include <string>
#include <DxLib.h>

class UIItemManager;

class KeyConfigSceneForSceneTitle final : public SceneBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">manager���Q�Ƃ��A�V�[���x�[�X�Ɉ����Ƃ��ēn��</param>
	/// <param name="input">input�̏����Q�Ƃ���</param>
	KeyConfigSceneForSceneTitle(SceneManager& manager);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~KeyConfigSceneForSceneTitle();

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
	/// �I�𑀍�
	/// </summary>
	void SelectionOperation();

	/// <summary>
	/// �L�[�̖�����̕`��
	/// </summary>
	void KeyStateDraw();

	/// <summary>
	/// key�̉摜��`��
	/// </summary>
	void KeyGraphDraw();

	/// <summary>
	/// �R���g���[���[���g�p����Ă���ꍇ�̕`��
	/// </summary>
	void ControllerDraw();

	/// <summary>
	/// �ύX������key����͂����邽�߂̃|�b�v�A�b�v�`��
	/// </summary>
	void ChangeKeyPopUpText();

	/// <summary>
	/// �ǂ̓��͑��u�̃L�[��ύX���邩��I������
	/// </summary>
	void SelectChangeKeyUpdate();

	/// <summary>
	/// �ύX����L�[���ǂ̃L�[�ɕύX����̂������肷��
	/// </summary>
	void ChangeKeyborardUpdate();

	/// <summary>
	/// �R���g���[���[�̏ꍇ�̍X�V
	/// </summary>
	void ControllerUpdate();

private:

	//������������Ȃ�
	void FadeInUpdate();
	void NormalUpdate();
	void FadeOutUpdate();

private:

	int keyTypeHandle_[117] = {};									//keyType��`�悷�邽�߂̃O���t���󂯎�邽�߂�handle
	int fontHandleSize21_ = -1;										//�t�H���g16�T�C�Y��ۊǂ���ϐ�
	int fontHandleSize42_ = -1;										//�t�H���g32�T�C�Y��ۊǂ���ϐ�
	int selectNum_ = 0;												//���݂̑I��ԍ�

	//������������������Ȃ�
	static constexpr int fadeInterval_ = 30;						//�t�F�[�h����Ԋu
	int fadeTimer_ = 0;												//�^�C�}�[
	int fadeValue_ = 0;												//�t�F�[�h�̒l
	int fadeColor_ = 0x000000;										//�w�i�̐F

	bool isEditing_ = false;										//�ҏW���t���O

	std::shared_ptr<UIItemManager> KeyUI_;							//UI�}�l�[�W���[�̃X�}�[�g�|�C���^
	std::shared_ptr<UIItemManager> PadUI_;							//UI�}�l�[�W���[�̃X�}�[�g�|�C���^

	std::map<int, Key> keyNum_;										//dxlib�̓��͔ԍ��Ǝ����̉摜�̔ԍ������킹�邽��
	std::map<std::string, VECTOR> keyDrawPos_;						//�L�[�̉摜��`�悷��ꏊ��ۑ�����
	std::map<std::string, VECTOR> padDrawPos_;						//�p�b�h�̉摜��`�悷��ꏊ

	void (KeyConfigSceneForSceneTitle::* updateFunc_)();			//�����o�֐��|�C���^
	void (KeyConfigSceneForSceneTitle::* changeKeyUpdateFunc_)();	//�����o�֐��|�C���^
	void (KeyConfigSceneForSceneTitle::* drawFunc_)();				//�����o�֐��|�C���^
};

