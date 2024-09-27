#include "SceneManager.h"
#include "SceneBase.h"
#include "DxLib.h"
#include "util/game.h"

namespace
{
	//updateBar��`�悷���̍��W�̕␳�l
	constexpr int update_bar_up_correction = 16;

	//box�����`�悷���̍��W�̕␳�l
	constexpr int draw_box_up_correction = 48;

	//box�����`�悷�鉺�̍��W�̕␳�l
	constexpr int draw_box_down_correction = 32;

	//UIUpdateBox�\���ʒuX
	constexpr int UI_update_box_draw_pos_x = 0;

	//UIDrawBox�\���ʒuX
	constexpr int UI_draw_box_draw_pos_x = 64;
	
	//UIbox�T�C�Y
	constexpr int draw_box_size = 16;

	//�f�o�b�OUI��
}

//�V�[���J��
void SceneManager::ChangeScene(const std::shared_ptr<SceneBase>& scene)
{
	//������V�[�������ׂč폜����
	while (!scenes_.empty())
	{
		//�擪�̃V�[�����폜����
		scenes_.front()->End();
		scenes_.pop_front();
	}
	//�擪�Ɉ����œ����V�[����ǉ�����
	scenes_.push_front(std::shared_ptr<SceneBase>(scene));
	scenes_.front()->Init();
}

//���C���̃V�[����ۂ����܂ܕʂ̃V�[����O��ʂɕ\������
void SceneManager::PushFrontScene(const std::shared_ptr<SceneBase>& scene)
{
	//�擪�Ɉ����œ����V�[����ǉ�����
	scenes_.push_front(scene);
	scenes_.front()->Init();
}

//�V�[��������ւ���
void SceneManager::SwapScene(const std::shared_ptr<SceneBase>& scene)
{
	//�擪�̃V�[�����폜����
	while(scenes_.size() > 1) {
		scenes_.front()->End();
		scenes_.pop_front();
	}
	//�擪�Ɉ����œ����V�[����ǉ�����
	scenes_.push_front(std::shared_ptr<SceneBase>(scene));
	scenes_.front()->Init();
}

//�ЂƂO�̃V�[���ɖ߂�
void SceneManager::PopFrontScene()
{
	//�擪�̃V�[�����폜����
	if (scenes_.size() > 1) {
		scenes_.front()->End();
		scenes_.pop_front();
	}
}

//�X�V
void SceneManager::Update()
{
	LONGLONG start = GetNowHiPerformanceCount();
	
	//�擪�ɂ���V�[���̍X�V
	scenes_.front()->Update();

	debugUpdateTime_ = static_cast<int>(GetNowHiPerformanceCount() - start);
}

//�`��
void SceneManager::Draw()
{

	LONGLONG start = GetNowHiPerformanceCount();

	//�V�[�����ׂĂ̕`��
	for (int i = static_cast<int>(scenes_.size() - 1); i >= 0; --i) 
	{
		scenes_[i]->Draw();
	}

	debugDrawTime_ = static_cast<int>(GetNowHiPerformanceCount() - start);

#ifdef _DEBUG
	float rate = static_cast<float>(debugUpdateTime_ + debugDrawTime_) / 16666.6f;
	int width = static_cast<int>(Game::screen_width * rate);
	DrawBox(UI_update_box_draw_pos_x, Game::screen_height - update_bar_up_correction, width, Game::screen_height, 0xff0000, true);

	rate = static_cast<float>(debugUpdateTime_) / 16666.6f;
	width = static_cast<int>(Game::screen_width * rate);
	DrawBox(UI_update_box_draw_pos_x, Game::screen_height - update_bar_up_correction, width, Game::screen_height, 0x0000ff, true);

	DrawBox(UI_update_box_draw_pos_x, Game::screen_height - draw_box_up_correction, UI_update_box_draw_pos_x + draw_box_size, Game::screen_height - draw_box_down_correction, 0x0000ff, true);
	DrawBox(UI_draw_box_draw_pos_x, Game::screen_height - draw_box_up_correction, UI_draw_box_draw_pos_x + draw_box_size, Game::screen_height - draw_box_down_correction, 0xff0000, true);

	DrawString( 0, Game::screen_height - draw_box_down_correction, "update", 0xffffff);
	DrawString(64, Game::screen_height - draw_box_down_correction, "draw", 0xffffff);
#endif
}

//�E�B���h�E���[�h��ύX����
void SceneManager::SetChangeWindowMode(const bool windowMode)
{
	windowMode_ = windowMode;
	DxLib::ChangeWindowMode(windowMode_);
}
