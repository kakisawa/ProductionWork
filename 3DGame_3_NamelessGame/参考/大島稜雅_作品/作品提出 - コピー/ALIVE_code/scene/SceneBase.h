#pragma once

class SceneManager;

class SceneBase abstract
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	SceneBase(SceneManager& manager) : manager_(manager){}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SceneBase() {};
	
	/// <summary>
	/// ������
	/// </summary>
	virtual void Init() abstract;

	/// <summary>
	/// �I��
	/// </summary>
	virtual void End() abstract;

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() abstract;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() abstract;
protected:
	SceneManager& manager_;			//�V�[���}�l�[�W���[�̎Q��
};

