#pragma once
#include "SceneBase.h"
#include <DxLib.h>
#include <memory>
#include <list>
#include <unordered_map>

class Camera;
class Player;
class CheckCollisionModel;
class ObjectManager;
class ShotManager;
class Tutorial;

class GameMain final : public SceneBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	GameMain(SceneManager& manager);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~GameMain();

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �I��
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
private:
	/// <summary>
	/// �S�[�����Ă��邩�𔻒肷��
	/// </summary>
	void GoalCheck();

	//������������Ȃ�
	void FadeInUpdate();
	void NormalUpdate();
	void FadeOutUpdate();

private:
	//������������������Ȃ�
	static constexpr int fadeInterval_ = 60;					//�t�F�[�h����Ԋu
	int fadeTimer_ = fadeInterval_;								//�^�C�}�[
	int fadeValue_ = 255;										//�t�F�[�h�̒l
	int fadeColor_ = 0x000000;									//�w�i�̐F

	//���C�g�̃n���h��
	int lightHandle_ = -1;										//���C�g�̃n���h��

	int skyHandle_ = -1;										//�X�J�C�h�[���̃n���h��
	int makeScreenHandle_ = -1;									//�쐬�����X�N���[���摜
	int totalDeathNum_ = 0;										//�Q�[���J�n����̑����S��

	bool isFilterOn_ = false;									//�t�B���^�[���g�p���邩

	std::shared_ptr<Camera> camera_;							//�J�����̃V�F�A�[�h�|�C���^
	std::shared_ptr<Player> player_;							//�v���C���[�̃V�F�A�[�h�|�C���^
	std::shared_ptr<ObjectManager> objManager_;					//objectManager�̃V�F�A�[�h�|�C���^
	std::shared_ptr<ShotManager> shotManager_;					//shotManager�̃V�F�A�[�h�|�C���^
	std::shared_ptr<Tutorial> tutorial_;						//�`���[�g���A���̃V�F�A�[�h�|�C���^
	
	std::shared_ptr<CheckCollisionModel> checkCollisionModel_;	//�Փ˔�����s���N���X�̃|�C���^

	void (GameMain::* updateFunc_)();							//�����o�֐��|�C���^
};

