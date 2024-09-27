#pragma once
#include <map>
#include <vector>
#include <string>
#include <DxLib.h>

class Tutorial
{
private:
	//UI�摜�̗�
	enum class UIGraph
	{
		XboxBotton,
		KeyBord,
		Max,
	};

	//�z�u�|�W�V����
	struct ArrangementPos
	{
		float x = 0.0f;			//�z�u����X���W
		float y = 0.0f;			//�z�u����Y���W
	};

	//�`���[�g���A���̃f�[�^
	struct TutorialData
	{
		int keyType = 0;		//�L�[�̎�ނ�int�^��cast�����l
		bool isPushKey = false;	//���̃L�[�������ꂽ��
		std::string str = {};	//�����Ƃ��ĕ\��������������
	};
	
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Tutorial();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Tutorial();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="pos">�v���C���[�̃|�W�V����</param>
	void Update(const VECTOR& playerPos);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �L�[�^�C�v��\�����Ȃ���
	/// </summary>
	/// <param name="data">�`���[�g���A���̃f�[�^</param>
	/// <returns>true:�\�����Ȃ� false:�\������</returns>
	bool DoNotDiplayKeyType(const TutorialData& data);

private:
	
	int elapsedTime_ = 0;												//�o�ߎ���
	int fontPigumo42_ = -1;												//�t�H���g�n���h��
	int currentDisplayNum_ = 0;											//���݂̕\������ԍ�

	std::string oldTutorialName_ = {};									//1�t���[���O�̃`���[�g���A���Ɏg���Ă������O��ۑ�����
	std::string currentTutorialName_ = {};								//���ꂩ��s����`���[�g���A���̖��O

	std::map<UIGraph, ArrangementPos> UIPos_;							//UI�̉�ʃ|�W�V���� pair�̒��g�bfirst�FX�bsecond�FY
	std::map<std::string, std::vector<TutorialData>> tutorialData_;		//�`���[�g���A���ɕK�v�ȏ���ۊǂ���
};

