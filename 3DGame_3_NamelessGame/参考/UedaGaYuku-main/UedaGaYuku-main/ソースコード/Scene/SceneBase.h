#pragma once
#include <memory>

class Input;
class UI;
class Ranking;

/// <summary>
/// �V�[�����N���X
/// </summary>
class SceneBase : public std::enable_shared_from_this<SceneBase>
{
public:
	SceneBase();
	virtual ~SceneBase();
	virtual void Init() = 0;
	virtual std::shared_ptr<SceneBase> Update(Input& input) = 0;
	virtual void Draw() = 0;

protected:
	void UpdateSelect(Input& input, int selectNum);	// �I����Ԃ��X�V
	virtual void FadeIn(int fadeFrame);				// �t�F�[�h�C���̏���
	virtual void FadeOut(int fadeFrame);			// �t�F�[�h�A�E�g�̏���
	void DrawFade();								// �t�F�[�h�C���A�E�g�̕`��

protected:
	std::shared_ptr<Ranking> m_pRank;
	std::shared_ptr<UI> m_pUI;	// UI�̃|�C���^
	int m_select;				// ���݂̑I�����
	int m_fadeAlpha;			// �t�F�[�h�̃��l
	int m_isFadeOut;			// �t�F�[�h�����ǂ���(true:�t�F�[�h��)

	// �X�e�[�W�̎��
	enum StageKind
	{
		kStage1 = 1,	// �X�e�[�W1
		kStage2 = 2,	// �X�e�[�W2
	};

#ifdef _DEBUG
	// �f�o�b�O��
	enum class DebugState
	{
		Normal,	// �ʏ�
		Pause	// �|�[�Y
	};
	DebugState m_debugState = DebugState::Normal;
#endif
};

