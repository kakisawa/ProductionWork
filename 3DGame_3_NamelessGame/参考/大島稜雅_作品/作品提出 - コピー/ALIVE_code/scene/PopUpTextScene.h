#pragma once
#include "SceneBase.h"

class InputState;

class PopUpTextScene final : public SceneBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	PopUpTextScene(SceneManager& manager);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~PopUpTextScene();

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �f�X�g���N�^
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
};

