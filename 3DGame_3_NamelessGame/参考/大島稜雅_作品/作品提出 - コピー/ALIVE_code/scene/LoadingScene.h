#pragma once
#include "SceneBase.h"
#include <memory>

class UIItemManager;

class LoadingScene final : public SceneBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	LoadingScene(SceneManager& manager);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~LoadingScene();

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

private:

	int aSyncLoadNum_ = 0;							//�񓯊��ǂݍ��ݒ��̏����̐�

	bool loadingFile_ = false;						//���[�h��  true:�r���@false:����

	std::shared_ptr<UIItemManager> UIManager_;		//ui���j���[�̃}�l�[�W���[�̃X�}�[�g�|�C���^

};

