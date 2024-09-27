#include "ScenePause.h"
#include "SceneTitle.h"
#include "DebugScene.h"
#include "SceneManager.h"
#include "SettingScene.h"
#include "KeyConfigScene.h"

#include "../util/Util.h"
#include "../util/game.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/FontsManager.h"
#include "../util/GraphManager.h"
#include "../util/UIItemManager.h"

#include <algorithm>

namespace
{
	//����
	constexpr int half = 2;

	//��ʂ̍����̎O��������
	constexpr int three_part = 3;

	//UI�z�u�|�W�V����X
	constexpr int UI_pos_x = 640;

	//UI�z�u�|�W�V����Y
	constexpr int UI_pos_y = 200;

	//�u�����h���[�h�̃A���t�@�l
	constexpr int blend_alpha_value = 150;

	//UImenu�̃A���t�@�l
	constexpr int UI_menu_alpha_value = 255;

	//�^�C�g���̉摜�̃T�C�Y
	constexpr float title_graph_size = 1.0f;

	//�^�C�g���摜�̊p�x
	constexpr float title_angle = 0.0f;

	//UImenu��Y���W�̏����ʒu
	constexpr float UI_menu_init_pos_y = 100.0f;

	//UImenu�̌��ԃT�C�Y
	constexpr float UI_menu_gap_size = 40.0f;
}

//�R���X�g���N�^
ScenePause::ScenePause(SceneManager& manager):SceneBase(manager)
{
}

//�f�X�g���N�^
ScenePause::~ScenePause()
{
}

//������
void ScenePause::Init()
{
	//�C���X�^���X��
	UI_ = std::make_shared<UIItemManager>();

	//UI������̍쐬
	menuName_.push_back("�߂�");
	menuName_.push_back("�ݒ�");
	menuName_.push_back("�^�C�g����");
#ifdef _DEBUG
	menuName_.push_back("�f�o�b�O�V�[����");
#endif // _DEBUG

	//UI�摜�̍쐬
	int font = FontsManager::GetInstance().GetFontHandle("�s�O�� 0042");
	float y = UI_menu_init_pos_y;
#ifdef _DEBUG
#else
	y += 40;
#endif // _DEBUG

	for (const auto& menu : menuName_)
	{
		UI_->AddMenu(static_cast<float>(Game::screen_width / half), static_cast<float>(Game::screen_height / half + y), UI_pos_x, UI_pos_y, menu.c_str(), font);
		y += UI_menu_gap_size;
	}
}

//�I��
void ScenePause::End()
{
}

//�X�V
void ScenePause::Update()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//���ڑI��
	if (input.IsTriggered(InputType::Up)) 
	{
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}
	if (input.IsTriggered(InputType::Down)) 
	{
		selectNum_ = (std::min)(selectNum_ + 1, static_cast<int>(menuName_.size()) - 1);
	}
	
	if (input.IsTriggered(InputType::Space))
	{
		 switch(selectNum_) 
		 {
		//��O�̃V�[��(���C���V�[��)�֑J��
		case 0:
			manager_.PopFrontScene();
			break;
		//�T�E���h�Z�b�e�B���O�ւ̑J��
		 case 1:
			 manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<SettingScene>(manager_)));
			 break;
		//�^�C�g���V�[���ւ̑J��
		case 2:
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
			break;
		//�f�o�b�O�V�[���ɖ߂�
		case 3:
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<DebugScene>(manager_)));
			break;
		 }
	}

	//��O�̃V�[��(���C���V�[��)�֑J��
	if (input.IsTriggered(InputType::Pause))
	{
		manager_.PopFrontScene();
	}
}

//�`��
void ScenePause::Draw()
{
	//�w�i���̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blend_alpha_value);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

	//�^�C�g���摜�̕`��
	DrawRotaGraph(Game::screen_width / half, Game::screen_height / three_part, title_graph_size, title_angle, GraphManager::GetInstance().GetGraph("title"), true);

	//UI�̕`��
	UI_->AlphaChangeDraw(selectNum_, UI_menu_alpha_value);
}
