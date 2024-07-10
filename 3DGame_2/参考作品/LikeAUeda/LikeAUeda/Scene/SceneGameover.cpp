#include "Dxlib.h"
#include "Input.h"
#include "SceneTitle.h"
#include "SceneGameover.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneGameover::SceneGameover()
{
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneGameover::~SceneGameover()
{
}


/// <summary>
/// ������
/// </summary>
void SceneGameover::Init()
{
}


/// <summary>
/// �X�V
/// </summary>
/// <param name="input">���͏��</param>
/// <returns></returns>
std::shared_ptr<SceneBase> SceneGameover::Update(Input& input)
{
	if (input.IsTriggered("OK"))
	{
		return std::make_shared<SceneTitle>();
	}

	return shared_from_this();	// ���g��shared_ptr��Ԃ�
}


/// <summary>
/// �`��
/// </summary>
void SceneGameover::Draw()
{
#ifdef _DEBUG	// �f�o�b�O�\��
	// ���݂̃V�[��
	DrawString(0, 0, "�Q�[���I�[�o�[���", 0xffffff);
#endif
}
