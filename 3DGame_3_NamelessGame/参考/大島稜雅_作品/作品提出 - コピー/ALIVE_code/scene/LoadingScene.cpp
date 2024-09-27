#include "LoadingScene.h"
#include "DebugScene.h"
#include "SceneTitle.h"
#include "SceneManager.h"

#include "../util/game.h"
#include "../util/ModelManager.h"
#include "../util/FontsManager.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/GraphManager.h"
#include "../util/EffectManager.h"
#include "../util/UIItemManager.h"

namespace
{
	//������\���̍��W�␳X
	constexpr float correction_x = 230.0f;

	//������\���̍��W�␳X
	constexpr float correction_y = 80.0f;
}

//�R���X�g���N�^
LoadingScene::LoadingScene(SceneManager& manager):SceneBase(manager)
{
}

//�f�X�g���N�^
LoadingScene::~LoadingScene()
{
}

//������
void LoadingScene::Init()
{
	//�Z�k��
	auto& font = FontsManager::GetInstance();

	//�C���X�^���X��
	UIManager_ = std::make_shared<UIItemManager>();

	//UI�摜�쐬
	UIManager_->AddingMenuWithSplitStr(Game::screen_width - correction_x, Game::screen_height - correction_y, Game::screen_width, Game::screen_height, "Loading...", font.GetFontHandle("�s�O�� 0042"));

	//�񓯊������̊J�n
	SetUseASyncLoadFlag(true);

	//�O���t�@�C���̃��[�h
	ExternalFile::GetInstance().LoadFile();
	
	//���f���t�@�C���p�X��ǂݍ���
	ModelManager::GetInstance().LoadModelFilePath();

	//���f���̃��[�h
	ModelManager::GetInstance().LoadModel();
	
	//�T�E���h�t�@�C���̃��[�h
	SoundManager::GetInstance().LoadSound();
	
	//�摜�̃t�@�C���p�X��ǂݍ���
	GraphManager::GetInstance().LoadGraphFilePath();

	//�摜�̃��[�h
	GraphManager::GetInstance().Load();

	//�����摜�̃t�@�C���p�X��ǂݍ���
	EffectManager::GetInstance().LoadDivGraphFilePath();

	//�G�b�t�F�N�g�摜�̃��[�h
	EffectManager::GetInstance().Load();

	//�񓯊��������I������
	SetUseASyncLoadFlag(false);

	loadingFile_ = true;
}

//�I��
void LoadingScene::End()
{
}

//�X�V
void LoadingScene::Update()
{
	//�񓯊��ǂݍ��ݒ��̐����擾
	aSyncLoadNum_ = GetASyncLoadNum();

	if (loadingFile_)
	{
		if (aSyncLoadNum_ < 1) 
		{
			loadingFile_ = false;

			//�f�[�^�̃��[�h
			ExternalFile::GetInstance().LoadArrangementData();
		}
	}
	else {
		if (aSyncLoadNum_ < 1) 
		{
#ifdef _DEBUG
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<DebugScene>(manager_)));
#else
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
#endif
		}
	}
}

//�`��
void LoadingScene::Draw()
{
	UIManager_->ChangePosDraw(Game::screen_width - correction_x, Game::screen_height - correction_y);
}
