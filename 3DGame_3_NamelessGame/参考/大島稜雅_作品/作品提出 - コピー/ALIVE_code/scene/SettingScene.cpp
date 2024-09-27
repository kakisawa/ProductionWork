#include "SettingScene.h"
#include "SceneManager.h"
#include "ScenePause.h"
#include "KeyConfigScene.h"

#include "../util/game.h"
#include "../util/InputState.h"
#include "../util/SoundManager.h"
#include "../util/FontsManager.h"
#include "../util/UIItemManager.h"

#include <algorithm>

using namespace std;

namespace 
{
	//����
	constexpr int half = 2;

	//��ʂ̉����̕�����
	constexpr int screen_width_division_num = 128;

	//��ʂ̏c�̕�����
	constexpr int screen_height_division_num = 72;

	//UI�z�u�|�W�V����X
	constexpr int UI_pos_x = 320;

	//UI�z�u�|�W�V����Y
	constexpr int UI_pos_y = 100;

	//���̍ŏ��l
	constexpr int min_sound_value = 0;
	
	//���̍ő�l
	constexpr int max_sound_value = 9;

	//����1�������̑傫��
	constexpr int sound_1_momery_size = 25;

	//�t�F�[�h�̍ő�l
	constexpr int max_fade_value = 255;

	//�K�E�X�����Ŏg�p����s�N�Z����
	constexpr int pixel_width = 32;

	//�u�����h���[�h�̃A���t�@�l
	constexpr int blend_mode_alpha_value = 150;

	//����ݒ�Ɩ߂�̕�����
	constexpr int operation_setting_and_back_width_division_num = 64;

	//���[�h�ASE�ABGM�̕�����
	constexpr int mode_SE_BGM_width_division_num = 32;

	//���[�h�̍����̕�����
	constexpr int mode_height_division_num = 24;
	 
	//SE�̍����̕�����
	constexpr int SE_height_division_num = 34;

	//BGM�̍����̕�����
	constexpr int BGM_height_division_num = 46;

	//����ݒ�̍����̕�����
	constexpr int operation_height_division_num = 54;
	
	//�߂�̍����̕�����
	constexpr int back_height_division_num = 58;

	//�~�̉����̕�����
	constexpr int circle_width_division_num = 34;

	//�~�̐�
	constexpr int circle_num = 10;

	//BGM�̍����̕�����
	constexpr int circle_height_division_num = 48;

	//�~�̒��_�̐�
	constexpr int circle_vertex_num = 32;

	//�I������Ă��鍀�ڂ̃A���t�@�l
	constexpr int selected_item_alpha_value = 250;

	//�I������Ă��Ȃ����ڂ̃A���t�@�l
	constexpr int not_selected_item_alpha_value = 150;

	//�~���m�̊Ԋu
	constexpr float circle_distance = 53.5f;

	//�~�̔��a
	constexpr float circle_radius = 14.5f;

	//�~�̊g�k��
	constexpr float circle_expansion_size = 1.3f;
}

//�R���X�g���N�^
SettingScene::SettingScene(SceneManager& manager):SceneBase(manager),updateFunc_(&SettingScene::GaussFadeInUpdate)
{
}

//�f�X�g���N�^
SettingScene::~SettingScene()
{
}

//������
void SettingScene::Init()
{
	//�Z�k��
	auto& sound = SoundManager::GetInstance();
	auto& font = FontsManager::GetInstance();

	//�C���X�^���X��
	UIManager_ = std::make_shared<UIItemManager>();

	//���݂̃{�����[���̎擾
	volumeBGM_ = sound.GetBGMVolume() / sound_1_momery_size;
	volumeSE_ = sound.GetSEVolume() / sound_1_momery_size;

	//UI�摜�̍쐬
	//�t�H���g�̎擾
	int pigumo42Font = font.GetFontHandle("�s�O�� 0042");

	//�t�H���g��K�p����������̃T�C�Y�擾
	int windowFontSize	= font.GetStringSize("���[�h", "�s�O�� 0042");
	int BGMFontSize		= font.GetStringSize("BGM", "�s�O�� 0042");
	int SEFontSize		= font.GetStringSize("SE", "�s�O�� 0042");

	//UI�摜�̍쐬
	UIManager_->AddMenu(Game::screen_width / screen_width_division_num * mode_SE_BGM_width_division_num, Game::screen_height / screen_height_division_num * mode_height_division_num					, UI_pos_x, UI_pos_y, "���[�h", pigumo42Font);
	UIManager_->AddMenu(Game::screen_width / screen_width_division_num * mode_SE_BGM_width_division_num, Game::screen_height / screen_height_division_num * SE_height_division_num						, UI_pos_x, UI_pos_y, "BGM", pigumo42Font);
	UIManager_->AddMenu(Game::screen_width / screen_width_division_num * mode_SE_BGM_width_division_num, Game::screen_height / screen_height_division_num * BGM_height_division_num						, UI_pos_x, UI_pos_y, "SE", pigumo42Font);
	UIManager_->AddMenu(Game::screen_width / screen_width_division_num * operation_setting_and_back_width_division_num, Game::screen_height / screen_height_division_num * operation_height_division_num, UI_pos_x, UI_pos_y, "����ݒ�", pigumo42Font);
	UIManager_->AddMenu(Game::screen_width / screen_width_division_num * operation_setting_and_back_width_division_num, Game::screen_height / screen_height_division_num * back_height_division_num		, UI_pos_x, UI_pos_y, "�߂�", pigumo42Font);

	//�X�N���[���摜�̍쐬
	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);

	if (manager_.GetWindowMode()) 
	{
		windowModeText_ = "��  �E�B���h�E���[�h  ��";
	}
	else {
		windowModeText_ = "��  �t���X�N���[��  ��";
	}
}

//�I��
void SettingScene::End()
{
	DeleteGraph(makeScreenHandle_);
}

//�X�V
void SettingScene::Update()
{
	(this->*updateFunc_)();
}

//�`��
void SettingScene::Draw()
{
	//�������߂������̔w�i��`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blend_mode_alpha_value);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�쐬������ʃT�C�Y�̉摜��
	//�ȍ~�̓��e����������
	SetDrawScreen(makeScreenHandle_);
	ClearDrawScreen();

	//UI������̕`��
	UIManager_->AlphaChangeDraw(selectNum_,fadeValue_);

	int alpha[static_cast<int>(UpdateItem::Max)] = {};

	for (int i = 0; i < static_cast<int>(UpdateItem::Max); i++)
	{
		if (selectNum_ == i)
		{
			alpha[i] = selected_item_alpha_value;
		}
		else
		{
			alpha[i] = not_selected_item_alpha_value;
		}
	}

	//�E�B���h�E���[�h�̕`��
	DrawWindowMode(alpha[static_cast<int>(UpdateItem::WindowMode)]);

	//BGM�����o�[�̕`��
	DrawBGMBar(alpha[static_cast<int>(UpdateItem::BGM)]);

	//SE�����o�[�̕`��
	DrawSEBar(alpha[static_cast<int>(UpdateItem::SE)]);

	//�ʏ�ɖ߂�
	SetDrawScreen(DX_SCREEN_BACK);

	//�t�F�[�h�̎��A�A���t�@�l��ύX����ƂƂ��ɃK�E�X�ڂ������|����
	GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, pixel_width, max_fade_value - fadeValue_);

	//�`��摜�̃A���t�@�l��ύX����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawGraph(0, 0, makeScreenHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//�E�B���h�E���[�h�̕`��
void SettingScene::DrawWindowMode(int alphaValue)
{
	//�Z�k��
	auto& font = FontsManager::GetInstance();

	//���݂̉�ʃ��[�h��\��
	int pigumo42 = font.GetFontHandle("�s�O�� 0042");

	//�t�H���g��K�p����������̉����̃T�C�Y���擾
	int windowModeFontSize = font.GetStringSize(windowModeText_.c_str(), "�s�O�� 0042");

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaValue);
	DrawStringToHandle(Game::screen_width / half - windowModeFontSize / half, Game::screen_height / screen_height_division_num * mode_height_division_num, windowModeText_.c_str(), 0xffffff, pigumo42);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//SE�����o�[�̕`��
void SettingScene::DrawSEBar(int alphaValue)
{
	//�~��`�悷��ʒu
	float circlePosX = Game::screen_width / screen_width_division_num * circle_width_division_num;

	//�g�k��
	float scale = 1.0f;

	//�F
	int color = 0xb1b3b6;

	//SE�o�[
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaValue);

	//���ʃo�[�摜�̕`��
	for (int i = 0; i < circle_num; i++)
	{
		scale = 1.0f;
		color = 0xb1b3b6;

		if (volumeSE_ == i)
		{
			scale = circle_expansion_size;
			color = 0xff0000;
		}

		DrawCircleAA(circlePosX + i * circle_distance + circle_distance, Game::screen_height / screen_height_division_num * circle_height_division_num, circle_radius * scale, circle_vertex_num, color, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//BGM�����o�[�̕`��
void SettingScene::DrawBGMBar(int alphaValue)
{
	//�~��`�悷��ʒu
	float circlePosX = Game::screen_width / screen_width_division_num * circle_width_division_num;

	//�g�k��
	float scale = 1.0f;

	//�F
	int color = 0xb1b3b6;

	//BGM�o�[
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaValue);

	//���ʃo�[�摜�̕`��
	for (int i = 0; i < circle_num; i++)
	{
		scale = 1.0f;
		color = 0xb1b3b6;

		if (volumeBGM_ == i)
		{
			scale = circle_expansion_size;
			color = 0xff0000;
		}

		DrawCircleAA(circlePosX + i * circle_distance + circle_distance, Game::screen_height / half, circle_radius * scale, circle_vertex_num, color, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//�ʏ펞�̍X�V
void SettingScene::NormalUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	if (input.IsTriggered(InputType::Up))
	{
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}
	else if (input.IsTriggered(InputType::Down))
	{
		selectNum_ = (std::min)(selectNum_ + 1, UIManager_->GetUINum() - 1);
	}

	ChangeUpdateFunc();

	//�V�[���؂�ւ�
	if (input.IsTriggered(InputType::Pause)) 
	{
		nextScene_ = std::make_shared<ScenePause>(manager_);
		updateFunc_ = &SettingScene::GaussFadeOutUpdate;
	}
}

//�K�E�X�������{�����t�F�[�h�C��
void SettingScene::GaussFadeInUpdate()
{
	fadeValue_ = static_cast <int>(max_fade_value * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_)
	{
		updateFunc_ = &SettingScene::NormalUpdate;
		fadeValue_ = max_fade_value;
	}
}

//�K�E�X�������{�����t�F�[�h�A�E�g
void SettingScene::GaussFadeOutUpdate()
{
	fadeValue_ = static_cast <int>(max_fade_value * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0)
	{
		manager_.SwapScene(std::shared_ptr<SceneBase>(nextScene_));
		fadeValue_ = 0;
		return;
	}
}

//BGM�̉��ʂ�ύX����
void SettingScene::BGMUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();
	auto& sound = SoundManager::GetInstance();

	//BGM���ʒ���
	if (input.IsTriggered(InputType::Left))
	{
		volumeBGM_ = (max)(volumeBGM_ - 1, min_sound_value);
		sound.PlayBGM("checkSoundBGM");
	}
	if (input.IsTriggered(InputType::Right))
	{
		volumeBGM_ = (min)(volumeBGM_ + 1, max_sound_value);
		sound.PlayBGM("checkSoundBGM");
	}

	//���ʂ̕ύX
	SoundManager::GetInstance().SetBGMVolume(volumeBGM_ * sound_1_momery_size);
}

//SE�̉��ʂ�ύX����
void SettingScene::SEUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();
	auto& sound = SoundManager::GetInstance();

	//SE���ʒ���
	if (input.IsTriggered(InputType::Left)) 
	{
		volumeSE_ = (max)(volumeSE_ - 1, min_sound_value);
		sound.PlaySE("checkSoundSE");
	}
	if (input.IsTriggered(InputType::Right))
	{
		volumeSE_ = (min)(volumeSE_ + 1, max_sound_value);
		sound.PlaySE("checkSoundSE");
	}

	//���ʂ̕ύX
	SoundManager::GetInstance().SetSEVolume(volumeSE_ * sound_1_momery_size);
}

//�E�B���h�E���[�h��ύX����
void SettingScene::ChangeWindowUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	if (input.IsTriggered(InputType::Left))
	{
		windowModeText_ = "��  �E�B���h�E���[�h  ��";
		manager_.SetChangeWindowMode(true);
	}
	if (input.IsTriggered(InputType::Right))
	{
		windowModeText_ = "��  �t���X�N���[��  ��";
		manager_.SetChangeWindowMode(false);
	}
}

//updateFunc�̒��g��ύX����
void SettingScene::ChangeUpdateFunc()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	switch (selectNum_) {
	case 0:
		ChangeWindowUpdate();
		break;
	case 1:
		BGMUpdate();
		break;
	case 2:
		SEUpdate();
		break;
	case 3:
		if (input.IsTriggered(InputType::Space))
		{
			nextScene_ = std::make_shared<KeyConfigScene>(manager_);
			updateFunc_ = &SettingScene::GaussFadeOutUpdate;
		}
		break;
	case 4:
		if (input.IsTriggered(InputType::Space))
		{
			nextScene_ = std::make_shared<ScenePause>(manager_);
			updateFunc_ = &SettingScene::GaussFadeOutUpdate;
		}
		break;
	}
}
