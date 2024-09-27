#pragma once
#include "SceneStageBase.h"
#include <vector>

class Player;
class Camera;
class Stage;
class EnemyBase;
class CharacterBase;

/// <summary>
/// �X�e�[�W1
/// </summary>
class SceneStage1 : public SceneStageBase
{
public:
	SceneStage1():m_isTuto(false),m_tutoNum(0) {};
	SceneStage1(std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera, std::shared_ptr<Stage> pStage);
	virtual  ~SceneStage1();
	virtual void Init() override;
	virtual std::shared_ptr<SceneBase> Update(Input& input) override;
	virtual void Draw() override;
	void IsSetTuto(bool isTuto) { m_isTuto = isTuto; } // �`���[�g���A����Ԃ�ύX

private:
	void UpdateSound();				// �T�E���h�X�V
	void UpdateTuto(Input& input);	// �`���[�g���A���̕\����Ԃ��X�V����
	void DrawTutorial();			// �`���[�g���A���\��

private:
	int m_tutoNum;					// ���݂̃`���[�g���A����
	bool m_isTuto;					// �`���[�g���A����\�����邩(true:�\������)
	std::string m_tutoText;			// �`���[�g���A�����ɕ\������e�L�X�g
	std::vector<int> m_tutoHandle;	// �`���[�g���A�����ɕ\������摜

	// �`���[�g���A���摜
	enum TutoHandle
	{
		kTuto0,
		kTuto1,
		kTuto2,
		kTuto3,
		kTuto4,
		kTuto5,
		kTutoNum,		// �`���[�g���A���̐�
		kTextBox,		// �e�L�X�g�{�b�N�X
		kTutoHandleNum,	// �摜�̐�
	};
};