#pragma once
#include "SceneBase.h"

/// <summary>
/// �|�[�Y���
/// </summary>
class ScenePause : public SceneBase
{
public:
	ScenePause(std::shared_ptr<SceneBase> pScene);
	virtual ~ScenePause();
	virtual void Init() override;
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw() override;

private:
	std::shared_ptr<SceneBase> m_pPrevScene;	// �O�Ɏ��s���Ă����V�[��

	// �I������
	enum Select
	{
		kBack,			// �Q�[���ɖ߂�
		kOption,		// �I�v�V����
		kStageSelect,	// �X�e�[�W�I���ɖ߂�
		kSelectNum		// �I��
	};
};

