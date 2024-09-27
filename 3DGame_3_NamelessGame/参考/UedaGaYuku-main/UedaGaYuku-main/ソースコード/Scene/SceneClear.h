#pragma once
#include "SceneBase.h"
#include <memory>

/// <summary>
/// �N���A�V�[��
/// </summary>
class SceneClear : public SceneBase
{
public:
	SceneClear():m_totalClearTime(0),m_stageKind(0),m_textHandle(-1), m_clearBgModel(-1){};
	SceneClear(int stageKind, std::vector<int> clearTime);
	virtual ~SceneClear();
	void Init();
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();

private:
	int m_totalClearTime;			// �g�[�^���̃N���A�^�C��
	std::vector<int> m_clearTime;	// �N���A�^�C��
	int m_stageKind;				// �N���A�����X�e�[�W�̎��
	int m_textHandle;				// �e�L�X�g�̉摜
	int m_clearBgModel;				// �N���A�w�i��3D���f��
};
