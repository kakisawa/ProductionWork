#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>
#include <string>
#include <DxLib.h>

class UIItemManager;

class ScenePause final : public SceneBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	ScenePause(SceneManager& manager);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~ScenePause();

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

	int selectNum_ = 0;						//�I��ԍ�

	std::shared_ptr<UIItemManager> UI_;		//UI�}�l�[�W���̃X�}�[�g�|�C���^
	std::vector<std::string> menuName_;		//�|�[�Y���j���[�̖��O�p
};

