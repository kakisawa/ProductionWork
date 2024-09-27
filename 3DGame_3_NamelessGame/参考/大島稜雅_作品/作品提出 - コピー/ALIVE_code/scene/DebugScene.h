#pragma once
#include "SceneBase.h"
#include <list>
#include <memory>
#include <vector>
#include <string>

class DebugScene final : public SceneBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	DebugScene(SceneManager& manager);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~DebugScene();

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

	int selectNum_ = 0;					//�I��ԍ�

	std::list<std::string> sceneName_;	//�V�[���̖��O
};

