#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>
#include <DxLib.h>

class Model;
class Camera;
class ObjectManager;

class GameEnd final : public SceneBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	GameEnd(SceneManager& manager);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~GameEnd();

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
	/// ���f���̐ݒ�
	/// </summary>
	void ModelSetting();

	/// <summary>
	/// ���̂̍X�V
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	void CorpseUpdate(const VECTOR& playerPos);

	/// <summary>
	/// �z���C�g�{�[�h�̍X�V
	/// </summary>
	void WhiteBoardUpdate();

	void FadeInUpdate();
	void NormalUpdate();
	void FadeOutUpdate();
private:

	int pigumo21FontHandle_ = -1;										//�s�O��21�̃t�H���g�n���h��
	int pigumo42FontHandle_ = -1;										//�s�O��42�̃t�H���g�n���h��
	int divisionNum_ = 0;												//������
	int corpseNum_ = 0;													//���삷�鎀�̂̔ԍ�
	int currentDivisionNum_ = 1;										//���݂̕�����
	int lightHandle_ = -1;												//���C�g�n���h��

	static constexpr int fadeInterval_ = 60;							//�t�F�[�h����Ԋu
	int fadeTimer_ = fadeInterval_;										//�^�C�}�[
	int fadeValue_ = 255;												//�t�F�[�h�̒l
	int fadeColor_ = 0x000000;											//�w�i�̐F

	
	bool isPutting_ = false;											//�u���Ă���
	bool isTurn_ = false;												//��]��
	bool isWentToRightSide_ = true;										//�E�[�ɍs������
	bool boardIsMoving_ = true;											//�{�[�h�������Ă��邩
	bool isResultDisplaying_ = false;									//���ʂ�\����

	VECTOR moveVec_ = { 1.0f,0.0f,0.0f };								//�ړ��x�N�g��

	std::shared_ptr<Camera> camera_;									//�J�����̃|�C���^
	std::shared_ptr<Model> playerModel_;								//�v���C���[���f���̃|�C���^
	std::shared_ptr<Model> boardModel_;									//�z���C�g�{�[�h���f���̃|�C���^
	std::shared_ptr<ObjectManager> objManager_;							//�I�u�W�F�N�g�}�l�[�W���[�̃|�C���^

	std::vector<int> barGraphHeight_;									//�_�O���t�̍���
	std::vector<std::pair<bool,std::shared_ptr<Model>>> corpseModel_;	//���̃��f���̃|�C���^

	void (GameEnd::* updateFunc_)();

};

