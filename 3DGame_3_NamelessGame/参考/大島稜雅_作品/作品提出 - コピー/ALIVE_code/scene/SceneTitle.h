#pragma once
#include "SceneBase.h"
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <DxLib.h>

class Model;
class Camera;
class LightBulb;
class UIItemManager;
class ObjectManager;

class SceneTitle final : public SceneBase
{
private:
	//�I���ł���V�[��
	enum class SelectScene
	{
		Setting,
		NewGame,
		SelectChapter,
		End,
		None,
		max,
	};

	//�V�[���I���̃f�[�^
	struct SelectSceneData
	{
		//�w������Ɉړ��ł��邩
		bool up		= false;
		bool down	= false;
		bool left	= false;
		bool rignt	= false;

		//�w��̕����̃V�[��UI
		SelectScene upScene		= SelectScene::None;
		SelectScene downScene	= SelectScene::None;
		SelectScene leftScene	= SelectScene::None;
		SelectScene rigntScene	= SelectScene::None;
	};

	//�f�[�^
	SelectSceneData selectData_[static_cast<int>(SelectScene::max)]
	{
		//�w������Ɉړ��ł��邩
		false,
		true,
		false,
		true,
		//�w��̕����̃V�[��UI
		SelectScene::None,
		SelectScene::NewGame,
		SelectScene::None,
		SelectScene::NewGame,

		//�w������Ɉړ��ł��邩
		true,
		false,
		true,
		true,
		//�w��̕����̃V�[��UI
		SelectScene::SelectChapter,
		SelectScene::None,
		SelectScene::Setting,
		SelectScene::End,

		//�w������Ɉړ��ł��邩
		false,
		true,
		true,
		false,
		//�w��̕����̃V�[��UI
		SelectScene::None,
		SelectScene::NewGame,
		SelectScene::NewGame,
		SelectScene::None,

		//�w������Ɉړ��ł��邩
		false,
		true,
		true,
		false,
		//�w��̕����̃V�[��UI
		SelectScene::None,
		SelectScene::NewGame,
		SelectScene::NewGame,
		SelectScene::None,

	};

	//�v�@36byte
	struct CameraInfo 
	{
		VECTOR targetPos;	//�ړI�̏ꏊ		//12byte
		VECTOR targetView;	//����ꏊ			//12byte
		VECTOR upVec;		//�J�����̏����	//12byte
	};
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	SceneTitle(SceneManager& manager);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SceneTitle();

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
	/// �v���C���[�̃��f���̏�����
	/// </summary>
	void PlayerModelInit();

	/// <summary>
	/// UI�̐ݒ�
	/// </summary>
	void UISetting();

	/// <summary>
	/// ���C�g�̐ݒ�
	/// </summary>
	void LightSetting();

	/// <summary>
	/// �J�����̐ݒ�
	/// </summary>
	void CameraPositionDataSetting();

	/// <summary>
	/// �J�����̖ڕW���W�̍X�V
	/// </summary>
	void CameraTargetUpdate();

	/// <summary>
	/// �I���V�[���̍X�V
	/// </summary>
	void SelectSceneUpdate();

	/// <summary>
	/// �t�F�[�h�C���̍X�V
	/// </summary>
	void FadeInUpdate();

	/// <summary>
	/// UI�̍X�V
	/// </summary>
	void UIUpdate();

	/// <summary>
	/// �I�[�v�j���O�̍X�V
	/// </summary>
	void OpeningUpdate();

	/// <summary>
	/// UI�̃t�F�[�h�A�E�g
	/// </summary>
	void UIFadeOutUpdate();

	/// <summary>
	/// �^�C�g���̃t�F�[�h�A�E�g
	/// </summary>
	void SceneTitleFadeOutUpdate();

	/// <summary>
	/// �V�[����؂�ւ���
	/// </summary>
	void SceneChange();

	/// <summary>
	/// �j���[�Q�[�����̓Q�[���I�[�o�[���I�����ꂽ���̕ύX
	/// </summary>
	void NewGameOrGameEndChange();

private:

	static constexpr int fadeInterval_ = 30;				//�C���^�[�o��
	int fadeTimer_ = 0;										//�t�F�[�h���鎞��
	int fadeValue_ = 0;										//�t�F�[�h�̓��ߒl
	int UIfadeValue_ = 0;									//UI�̃t�F�[�h���ߒl
	int fadeColor_ = 0x000000;								//��

	int spotLightHandle_ = -1;								//�X�|�b�g���C�g�̃n���h��
	int directionLightHandle_ = -1;							//�f�B���N�V�������C�g�̃n���h��
	int fontHandle_ = -1;									//�t�H���g�n���h��
	int titleHandle_ = -1;									//�^�C�g���摜�̕ۊǕϐ�

	float outAngle_ = 0.0f;									//�X�|�b�g���C�g�̊O���̊p�x
	float inAngle_ = 0.0f;									//�X�|�b�g���C�g�̓����̊p�x

	SelectScene currentSelectScene_ = SelectScene::NewGame;	//���݂̑I���V�[��
	SelectScene oldSelectScene_ = SelectScene::NewGame;		//�ЂƂO�̑I���V�[��

	VECTOR directionLightDir_ = { 0.0f, 0.0f, 0.0f };		//�f�B���N�V�������C�g�̕���

	std::shared_ptr<Camera> camera_;						//�J�����̃X�p�[�g�|�C���^
	std::shared_ptr<UIItemManager> UI_;						//UI�}�l�[�W���[�̃X�}�[�g�|�C���^
	std::shared_ptr<LightBulb> lightBulb_;					//�d��
	std::shared_ptr<Model> subPlayerModel_;					//�v���C���[�̃X�}�[�g�|�C���^
	std::shared_ptr<Model> mainPlayerModel_;				//�v���C���[�̃X�}�[�g�|�C���^
	std::shared_ptr<ObjectManager> objManager_;				//�I�u�W�F�N�g�}�l�[�W���[�̃X�}�[�g�|�C���^

	std::vector<std::string> menuName_;						//���j���[�̕�����
//	std::vector<int> lightHandle_;							//���C�g�n���h��
//	std::vector<VECTOR> lightDir_;							//���C�g�̃f�B���N�V����(����)
	std::vector<CameraInfo> cameraInfo_;					//�J�����̏��

	std::map<std::string, VECTOR> menuDrawPos_;				//���j���[��`�悷����W

	void (SceneTitle::* updateFunc_)();						//�����o�֐��|�C���^
};

