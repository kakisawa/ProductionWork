#include "SelectChapterScene.h"
#include "GameMain.h"
#include "SceneManager.h"

#include "../object/ObjectData.h"

#include "../util/game.h"
#include "../util/Model.h"
#include "../UTIL/Easing.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/ModelManager.h"
#include "../util/GraphManager.h"

#include <string>
#include <algorithm>

namespace {
	//�A�j���[�V������ύX����̂ɂ����鎞��
	constexpr int anim_change_time = 10;

	//�e�N�X�`���̔ԍ�
	constexpr int texture_num = 0;

	//�ő�`���v�^�[��
	constexpr int max_chapter_num = 2;

	//�ŏ��`���v�^�[��
	constexpr int min_chapter_num = 0;

	//�t�F�[�h�̍ő�l
	constexpr int max_fade_value = 255;

	//������
	constexpr float total_time = 60.0f;
}

SelectChapterScene::SelectChapterScene(SceneManager& manager) : SceneBase(manager), updateFunc_(&SelectChapterScene::NormalUpdate)
{
}

//�R���X�g���N�^
SelectChapterScene::SelectChapterScene(SceneManager& manager, std::shared_ptr<Model> modelPointer) : SceneBase(manager),updateFunc_(&SelectChapterScene::NormalUpdate)
{
	//���f���̃|�C���^��ݒ肷��
	model_ = modelPointer;

	//���f���̃A�j���[�V������ݒ肷��
	model_->SetAnimation(static_cast<int>(BookAnim::idle), false, false);
}

//�f�X�g���N�^
SelectChapterScene::~SelectChapterScene()
{
}

//������
void SelectChapterScene::Init()
{
}

//�I��
void SelectChapterScene::End()
{
}

//�X�V
void SelectChapterScene::Update()
{
	(this->*updateFunc_)();
}

//�`��
void SelectChapterScene::Draw()
{
	//���f���̕`��
	model_->Draw();

	//��ʑS�̂�^�����ɓh��Ԃ�
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//�`���v�^�[��ύX����
void SelectChapterScene::ChangeChapter()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//�I�΂�Ă���`���v�^�[�̕�������擾����
	std::string str = "Chapter" + std::to_string(selectNum_);

	//�v���C���[�̊J�n�ʒu��ݒ肷��
	file.SetStartName(str);
}

//�ʏ�̍X�V
void SelectChapterScene::NormalUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();
	auto& graph = GraphManager::GetInstance();

	//���f���̍X�V
	model_->Update();

	if (model_->IsAnimEnd()) 
	{
		model_->ChangeAnimation(static_cast<int>(BookAnim::idle), false, false, anim_change_time);
	}
	else 
	{
		return;
	}

	//�`���v�^�[�I��
	if (input.IsTriggered(InputType::Right)) 
	{
		if (selectNum_ < max_chapter_num)
		{
			model_->ChangeAnimation(static_cast<int>(BookAnim::open), false, false, anim_change_time);
		}
		selectNum_ = (std::min)(selectNum_ + 1, max_chapter_num);
	}
	//�`���v�^�[�I��
	if (input.IsTriggered(InputType::Left))
	{
		if (selectNum_ > 0) 
		{
			model_->ChangeAnimation(static_cast<int>(BookAnim::close), false, false, anim_change_time);
		}
		selectNum_ = (std::max)(selectNum_ - 1, min_chapter_num);
	}

	//�摜���̕�����̎擾
	std::string str = "Chapter" + std::to_string(selectNum_ + 1);

	//�}�e���A���̃e�N�X�`����ύX����
	MV1SetTextureGraphHandle(model_->GetModelHandle(), texture_num, graph.GetGraph(str), true);

	//�߂�
	if (input.IsTriggered(InputType::Down) || input.IsTriggered(InputType::Activate))
	{
		manager_.PopFrontScene();
	}

	//����
	if (input.IsTriggered(InputType::Space)) 
	{
		updateFunc_ = &SelectChapterScene::FadeOutUpdate;
	}
}

//�t�F�[�h�A�E�g
void SelectChapterScene::FadeOutUpdate()
{
	fadeValue_ = static_cast <int>(max_fade_value * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));

	if (++fadeTimer_ == fadeInterval_) 
	{
		ChangeChapter();
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
		return;
	}
}
