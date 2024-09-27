#pragma once
#include "SceneBase.h"
#include <memory>

class Model;

class SelectChapterScene final : public SceneBase
{
private:
	//�{�̃A�j���[�V����
	enum class BookAnim
	{
		idle = 3,
		open,
		close,
	};

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	SelectChapterScene(SceneManager& manager);

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	SelectChapterScene(SceneManager& manager,std::shared_ptr<Model> modelPointer);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SelectChapterScene();

	/// <summary>
	/// ������
	/// </summary>
	void Init()override;

	/// <summary>
	/// �I��
	/// </summary>
	void End()override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �`���v�^�[��ύX����
	/// </summary>
	void ChangeChapter();

private:
	/// <summary>
	/// �ʏ�̍X�V
	/// </summary>
	void NormalUpdate();

	/// <summary>
	/// �t�F�[�h�A�E�g
	/// </summary>
	void FadeOutUpdate();
private:

	int selectNum_ = 0;								//�I��ԍ�

	float elapsedTime_ = 0;							//�o�ߎ���

	static constexpr int fadeInterval_ = 60;		//�t�F�[�h�C���^�[�o��
	int fadeTimer_ = 0;								//�t�F�[�h����
	int fadeValue_ = 0;								//�t�F�[�h�̒l
	int fadeColor_ = 0x000000;						//�t�F�[�h�̐F

	std::shared_ptr<Model> model_;					//���f���|�C���^

	void(SelectChapterScene::* updateFunc_)();		//�����o�֐��|�C���^
};

