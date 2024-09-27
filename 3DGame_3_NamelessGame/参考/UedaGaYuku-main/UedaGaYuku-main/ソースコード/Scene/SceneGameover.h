#pragma once
#include "SceneBase.h"

class SceneStageBase;

/// <summary>
/// �Q�[���I�[�o�[�V�[��
/// </summary>
class SceneGameover : public SceneBase
{
public:
	SceneGameover():m_textHandle(-1), m_backHandle(-1) {};
	SceneGameover(std::shared_ptr<SceneBase> pScene);
	virtual ~SceneGameover();
	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();

private:
	std::shared_ptr<SceneBase> m_pPrevScene;	// �O�Ɏ��s���Ă����V�[��
	int m_textHandle;	// �e�L�X�g�̉摜
	int m_backHandle;	// �w�i�̉摜

	// �I������
	enum Select
	{
		kRetry,			// ���g���C
		kStageSelect,	// �X�e�[�W�I��
		kTitle,			// �^�C�g��
		kSelectNum,		// �I��
	};
};

