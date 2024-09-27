#include "DebugScene.h"
#include "GameMain.h"
#include "GameEnd.h"
#include "ScenePause.h"
#include "SceneTitle.h"
#include "LoadingScene.h"
#include "SceneManager.h"
#include "SettingScene.h"
#include "KeyConfigScene.h"

#include "../util/InputState.h"
#include "../util/ExternalFile.h"

#include <algorithm>

namespace
{
	//������̕␳�z�u�ꏊY
	constexpr int correction_pos_Y = 32;

	//�������\������X���W
	constexpr int str_draw_pos_x = 100;

	//�������\������Y���W
	constexpr int str_draw_pos_y = 150;
}

//�R���X�g���N�^
DebugScene::DebugScene(SceneManager& manager):SceneBase(manager)
{
}

//�f�X�g���N�^
DebugScene::~DebugScene()
{
}

//������
void DebugScene::Init()
{
	sceneName_.push_back("main");
	sceneName_.push_back("title");
	sceneName_.push_back("testScene");
	sceneName_.push_back("end");
	sceneName_.push_back("pause");
	sceneName_.push_back("exit");
}

//�I��
void DebugScene::End()
{
}

//�X�V
void DebugScene::Update()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�I��
	if (input.IsTriggered(InputType::Up)) {
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}
	if (input.IsTriggered(InputType::Down)) {
		selectNum_ = (std::min)(selectNum_ + 1, static_cast<int>(sceneName_.size()) - 1);
	}

	//�V�[���̐؂�ւ�
	if (input.IsTriggered(InputType::Space)) {
		switch (selectNum_) {
		case 0:
			//�Q�[���v���C�V�[��
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
			break;
		case 1:
			//�V�[���^�C�g��
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
			break;
		case 2:
			//�f�o�b�O�������V�[��
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<KeyConfigScene>(manager_)));
			break;
		case 3:
			//�I�����
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameEnd>(manager_)));
			break;
		case 4:
			//�V�[���|�[�Y
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
			break;
		case 5:
			//�I��
			manager_.SetEndFlag(true);
			break;
		}
	}
}

//�`��
void DebugScene::Draw()
{
	int x = str_draw_pos_x;
	int y = str_draw_pos_y;

	//������̕`��
	for (const auto& scene : sceneName_) {
		DrawFormatString(0, y, 0xffffff, "%s", scene.c_str());
		y += correction_pos_Y;
	}

	y = str_draw_pos_y;
	//�I��ԍ��ɂ���ĕς����̕`��
	DrawString(x, selectNum_ * correction_pos_Y + y, "��", 0xff0000);
}
