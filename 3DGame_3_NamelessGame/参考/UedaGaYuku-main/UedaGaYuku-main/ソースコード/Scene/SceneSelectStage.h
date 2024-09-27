#pragma once
#include "SceneBase.h"

/// <summary>
/// �X�e�[�W�I�����
/// </summary>
class SceneSelectStage : public SceneBase
{
public:
	SceneSelectStage();
	virtual ~SceneSelectStage();
	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();

private:
	void DrawIntroductionText();	// �Љ��\������

private:
	// �V�[���̎��
	enum SelectScene
	{
		kStage1,	// �X�e�[�W1
		kStage2,	// �X�e�[�W2
		kOption,	// �I�v�V����
		kEnd,		// �Q�[�����I���
		kSelectNum	// �V�[���̐�
	};
};

