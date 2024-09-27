#pragma once
#include "SceneBase.h"
#include <memory>

class UIItemManager;

class KeyConfigScene final : public SceneBase
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">manager���Q�Ƃ��A�V�[���x�[�X�Ɉ����Ƃ��ēn��</param>
	/// <param name="input">input�̏����Q�Ƃ���</param>
	KeyConfigScene(SceneManager& manager);
	
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~KeyConfigScene();

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

	int fontHandleSize21_ = -1;							//�t�H���g16�T�C�Y��ۊǂ���ϐ�
	int fontHandleSize42_ = -1;							//�t�H���g32�T�C�Y��ۊǂ���ϐ�
	int makeScreenHandle_ = -1;							//�쐬�����n���h����ۊǂ���ϐ�
	int textColor_ = 0xffffff;							//�J���[
	int selectNum_ = 0;									//���݂̑I��ԍ�

	//������������������Ȃ�
	static constexpr int fadeInterval_ = 30;			//�t�F�[�h����Ԋu
	int fadeTimer_ = 0;									//�^�C�}�[
	int fadeValue_ = 0;									//�t�F�[�h�̒l
	int fadeColor_ = 0x000000;							//�w�i�̐F

	bool isEditing_ = false;							//�ҏW���t���O

	std::shared_ptr<UIItemManager> KeyUI_;				//UI�}�l�[�W���[�̃X�}�[�g�|�C���^
	std::shared_ptr<UIItemManager> PadUI_;				//UI�}�l�[�W���[�̃X�}�[�g�|�C���^

	void (KeyConfigScene::* updateFunc_)();
	void (KeyConfigScene::* changeKeyUpdateFunc_)();	//�����o�֐��|�C���^
	void (KeyConfigScene::* drawFunc_)();				//�����o�֐��|�C���^
};

