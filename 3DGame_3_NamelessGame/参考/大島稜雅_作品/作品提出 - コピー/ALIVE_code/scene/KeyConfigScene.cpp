#include "KeyConfigScene.h"
#include "SceneManager.h"
#include "SettingScene.h"
#include "PopUpTextScene.h"

#include "../util/game.h"
#include "../util/InputState.h"
#include "../util/FontsManager.h"
#include "../util/GraphManager.h"
#include "../util/DrawFunctions.h"
#include "../util/UIItemManager.h"

#include <DxLib.h>
#include <algorithm>
#include <string>

namespace {
	//����
	constexpr int half = 2;

	//��ʉ��̕�����
	constexpr int screen_division_width = 128;

	//��ʏc�̕�����
	constexpr int screen_division_height = 72;

	//�t�F�[�h�̍ő�l
	constexpr int max_fade_value = 255;

	//�L�[�̐�
	constexpr int max_key_num = 256;

	//�K�E�X�����Ŏg�p����s�N�Z����
	constexpr int pixel_width = 32;

	//�ڂ����̃p�����[�^�[
	constexpr int pixel_param = 800;

	//�ύXUI�ƃL�����Z��UI�̐�
	constexpr int change_and_cancel_UI_Num = 2;

	//�w�i��alphaBlend�̍ő�l
	constexpr int background_max_alpha_blend_value = 180;

	//�|�b�v�A�b�v�e�L�X�g��alphaBlend�̍ő�l
	constexpr int pop_up_text_max_alpha_blend_value = 100;

	//���邳�������l
	constexpr int sub_brightness = 180;

	//UI�z�u�|�W�V����X
	constexpr int UI_pos_x = 320;
	
	//UI�z�u�|�W�V����Y
	constexpr int UI_pos_y = 100;

	//�摜UI�z�u�|�W�V����X�̕�����
	constexpr int graph_screen_division_x = 48;

	//�摜UI�z�u�|�W�V����Y�̕�����
	constexpr int graph_screen_division_y = 10;

	//�ύX�|�b�v�A�b�v�摜UI�z�u�|�W�V����Y�̕�����
	constexpr int cange_pop_up_graph_screen_division_y = 11;

	//�L�[�̖������UI�z�u�|�W�V����X�̕�����
	constexpr int key_role_division_x = 32;

	//�L�[�̖������UI�z�u�|�W�V����Y�̕�����
	constexpr int key_role_division_y = 9;

	//�L�[�̖������UI�z�u�|�W�V�����܂�Ԃ�X�̕�����
	constexpr int key_role_division_turn_back_x = 48;

	//�ύX��UI�z�u�|�W�V����Y�̕�����
	constexpr int change_division_y = 57;

	//�L�����Z����UI�z�u�|�W�V����Y�̕�����
	constexpr int cancel_division_y = 60;

	//�L�[��ύX����ۂɏo�Ă���|�b�v�A�b�v�e�L�X�g�̔w�i�̔z�u�|�W�V�������[X�̕�����
	constexpr int pop_up_background_division_left_x = 21;

	//�L�[��ύX����ۂɏo�Ă���|�b�v�A�b�v�e�L�X�g�̔w�i�̔z�u�|�W�V������Y�̕�����
	constexpr int pop_up_background_division_up_y = 14;

	//�L�[��ύX����ۂɏo�Ă���|�b�v�A�b�v�e�L�X�g�̔w�i�̔z�u�|�W�V�����E�[X�̕�����
	constexpr int pop_up_background_division_right_x = 106;

	//�L�[��ύX����ۂɏo�Ă���|�b�v�A�b�v�e�L�X�g�̔w�i�̔z�u�|�W�V������Y�̕�����
	constexpr int pop_up_background_division_down_y = 57;

	//key�摜�̈ꖇ������̃T�C�Y
	constexpr float graph_chip_size = 59.0f;

	//�摜�̌��ԃT�C�Y
	constexpr float graph_gap_size = 10.0f;

	//�摜�̊g�k��
	constexpr float graph_scale_size = 1.2f;

	//�����̊g�k��
	constexpr float str_scale_size = 2.5f;
}

//�R���X�g���N�^
KeyConfigScene::KeyConfigScene(SceneManager& manager):SceneBase(manager),updateFunc_(&KeyConfigScene::FadeInUpdate)
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�Ō�ɓ��͂��ꂽ���u�ɂ���čX�V�A�`��֐���ύX����
	if (!input.LastInputDevice()) {
		changeKeyUpdateFunc_ = &KeyConfigScene::ControllerUpdate;
		drawFunc_ = &KeyConfigScene::ControllerDraw;
	}
	else {
		changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigScene::KeyStateDraw;
	}
}

//�f�X�g���N�^
KeyConfigScene::~KeyConfigScene()
{
}

//������
void KeyConfigScene::Init()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�C���X�^���X��
	KeyUI_ = std::make_shared<UIItemManager>();
	PadUI_ = std::make_shared<UIItemManager>();

	//�X�N���[���T�C�Y�̃n���h�����쐬
	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);

	//�t�H���g�̍쐬
	fontHandleSize21_ = FontsManager::GetInstance().GetFontHandle("�s�O�� 0021");
	fontHandleSize42_ = FontsManager::GetInstance().GetFontHandle("�s�O�� 0042");
	
	//�L�[�̖����̈�ڂ̍��W
	float namePosX = static_cast<float>(Game::screen_width / screen_division_width * key_role_division_x);
	float namePosY = static_cast<float>(Game::screen_height / screen_division_height * key_role_division_y);

	int nameNo = 0;
	for (const auto& table : input.inputNameTable_) {
		//���j���[�̒ǉ�
		KeyUI_->AddMenu(namePosX, namePosY, UI_pos_x, UI_pos_y, table.second.c_str(), fontHandleSize21_);

		//�|�W�V��������
		namePosY += graph_chip_size + graph_gap_size;

		//inputstate.tempmaptable�̔����𒴂�����܂�Ԃ�
		if (nameNo == input.tempMapTable_.size() / half - 1)
		{
			namePosX += static_cast<float>(Game::screen_width / screen_division_width * key_role_division_turn_back_x);
			namePosY = static_cast<float>(Game::screen_height / screen_division_height * key_role_division_y);
		}

		nameNo++;
	}

	//���j���[�̒ǉ�
	KeyUI_->AddMenu(static_cast<float>(Game::screen_width / half), static_cast<float>(Game::screen_height / screen_division_height * change_division_y), UI_pos_x, UI_pos_y, "�ύX", fontHandleSize42_);
	KeyUI_->AddMenu(static_cast<float>(Game::screen_width / half), static_cast<float>(Game::screen_height / screen_division_height * cancel_division_y), UI_pos_x, UI_pos_y, "�L�����Z��", fontHandleSize42_);
	PadUI_->AddMenu(static_cast<float>(Game::screen_width / half), static_cast<float>(Game::screen_height / screen_division_height * cancel_division_y), UI_pos_x, UI_pos_y, "�߂�", fontHandleSize42_);
}

//�I��
void KeyConfigScene::End()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//���݂̃L�[���͏����O���f�[�^�Ƃ��ď����o��
	input.SavekeyInfo();
	DeleteGraph(makeScreenHandle_);
}

//�X�V
void KeyConfigScene::Update()
{
	(this->*updateFunc_)();
}

//�`��
void KeyConfigScene::Draw()
{
	//�w�i�ɍ��̓���
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, background_max_alpha_blend_value);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�K�E�X�������{��
	GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, pixel_width, max_fade_value - fadeValue_);

	//makeScreen�ō쐬�����n���h����`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawGraph(0, 0, makeScreenHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	(this->*drawFunc_)();
}

//�I�𑀍�
void KeyConfigScene::SelectionOperation()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//���݂̃L�[�̐����擾����
	const int keyNum = static_cast<int>(input.inputNameTable_.size() + change_and_cancel_UI_Num);

	//�I�𑀍�
	if (input.IsTriggered(InputType::Up))
	{
		if (selectNum_ == (keyNum - 1) / half)
		{
			selectNum_ += keyNum / half;
		}
		else {
			selectNum_ = ((selectNum_ - 1) + keyNum) % keyNum;
		}
	}
	else if (input.IsTriggered(InputType::Down))
	{
		if (selectNum_ + 1 == (keyNum - 1) / half)
		{
			selectNum_ += keyNum / half;
		}
		else
		{
			selectNum_ = (selectNum_ + 1) % keyNum;
		}
	}
	else if (input.IsTriggered(InputType::Left) || input.IsTriggered(InputType::Right))
	{
		if (selectNum_ < keyNum - change_and_cancel_UI_Num)
		{
			selectNum_ = (selectNum_ + keyNum / half) % (keyNum - 1);
		}
	}
}

//�L�[�̖�����̕`��
void KeyConfigScene::KeyStateDraw()
{
	//�������݃X�N���[���̕ύX
	SetDrawScreen(makeScreenHandle_);

	//�X�N���[���̃N���A
	ClearDrawScreen();

	//�L�[�̖�����`�悷��
	KeyUI_->AlphaChangeDraw(selectNum_,fadeValue_);

	//�L�[�ɑΉ�����摜��`�悷��
	KeyGraphDraw();

	//�������݃X�N���[���𗠂̃X�N���[���ɕύX����
	SetDrawScreen(DX_SCREEN_BACK);
}

//�L�[�̉摜��`�悷��
void KeyConfigScene::KeyGraphDraw()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�|�W�V�����ݒ�
	float graphPosX = static_cast<float>(Game::screen_width / screen_division_width * graph_screen_division_x);
	float graphPosY = static_cast<float>(Game::screen_height / screen_division_height * graph_screen_division_y);

	//for���ŉ��Ԗڂ����擾����
	int keyCount = 0;

	//���邳�������l
	int subBrightness = 0;

	//�摜�̊g�k��
	float graphScale = 1.0f;

	for (const auto& key : input.tempMapTable_) {
		
		if (key.first == InputType::Creative) {
			continue;
		}

		//���݃J�[�\���������Ă���ꍇ
		//�T�C�Y�Ɩ��邳�������l��ύX����
		if (keyCount == selectNum_) {
			graphScale = graph_scale_size;
			subBrightness = 0;
		}
		else {
			graphScale = 1.0f;
			subBrightness = sub_brightness;
		}

		//keyType�̕`��
		input.DrawKeyGraph(static_cast<int>(key.first), graphPosX, graphPosY, graphScale);

		//�Â������摜���摜�̏�ɏ悹��
		SetDrawBlendMode(DX_BLENDMODE_SUB, subBrightness);

		DrawBoxAA(graphPosX - graph_chip_size, graphPosY - graph_chip_size / half - graph_gap_size / half,
				  graphPosX + graph_chip_size , graphPosY + graph_chip_size,
				  fadeColor_, true);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//�����𒴂�����܂�Ԃ�����
		graphPosY += graph_chip_size + graph_gap_size;
		if (keyCount == input.tempMapTable_.size() / half - 1)
		{
			graphPosY = static_cast<float>(Game::screen_height / screen_division_height * graph_screen_division_y);
			graphPosX += static_cast<float>(Game::screen_width / screen_division_width * graph_screen_division_x);
		}

		keyCount++;
	}
}

//�R���g���[���[�摜�̕`��
void KeyConfigScene::ControllerDraw()
{
	//�������݃X�N���[���̕ύX
	SetDrawScreen(makeScreenHandle_);

	//�X�N���[���̃N���A
	ClearDrawScreen();

	//UI�̕`��
	PadUI_->AlphaChangeDraw(selectNum_, fadeValue_);

	//��������摜�̕`��
	DrawGraph(0, 0, GraphManager::GetInstance().GetGraph("controller"), true);

	//�������݃X�N���[���𗠂̃X�N���[���ɕύX����
	SetDrawScreen(DX_SCREEN_BACK);
}

//�ύX������key����͂����邽�߂̃|�b�v�A�b�v�`��
void KeyConfigScene::ChangeKeyPopUpText()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�w�i�̍��`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, pop_up_text_max_alpha_blend_value);
	DrawBox(Game::screen_width / screen_division_width * pop_up_background_division_left_x, Game::screen_height / screen_division_height * pop_up_background_division_up_y,
			Game::screen_width / screen_division_width * pop_up_background_division_right_x, Game::screen_height / screen_division_height * pop_up_background_division_down_y,
			0x7d7d7d, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//������
	std::string text = input.inputNameTable_.find(static_cast<InputType>(selectNum_))->second;

	//������̉���
	int strWidth = GetDrawStringWidthToHandle(text.c_str(), static_cast<int>(text.size()), fontHandleSize21_);

	//�I�������L�[�̖��O���o��
	DrawStringToHandle(Game::screen_width / screen_division_width * pop_up_background_division_left_x + strWidth / half ,
					   Game::screen_height / screen_division_height * cange_pop_up_graph_screen_division_y,
					   text.c_str(), 0xffffff, fontHandleSize21_);

	//int�^�ɃL���X�g����graphChipSize
	int castGraphChipSize = static_cast<int>(graph_chip_size);

	//�L�[�摜�̕`��
	input.DrawKeyGraph( selectNum_, 
						static_cast<float>(Game::screen_width / screen_division_width * pop_up_background_division_left_x + strWidth * str_scale_size),
						static_cast<float>(Game::screen_height / screen_division_height * pop_up_background_division_up_y - graph_chip_size / half),
						1.0f);

	//������
	text = "�ύX�������L�[����͂��Ă�������";
	//������̉���
	strWidth = GetDrawStringWidthToHandle(text.c_str(), static_cast<int>(strlen(text.c_str())), fontHandleSize42_);
	//������̕`��
	DrawStringToHandle(Game::screen_width / half - strWidth / half, Game::screen_height / half, text.c_str(), 0xffffff, fontHandleSize42_);

}

//�ǂ̓��͑��u�̃L�[��ύX���邩��I������
void KeyConfigScene::SelectChangeKeyUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�I�𑀍�
	SelectionOperation();

	//�L�[�̕ύX��ۑ�����
	if (selectNum_ == input.inputNameTable_.size())
	{
		if (input.IsTriggered(InputType::Space)) 
		{
			//���̃L�[�������ۂɎQ�Ƃ��Ă���L�[���ɑ������
			input.CommitChangedInputInfo();

			//�V�[�����|�b�v�A�b�v�e�L�X�g��`�悷��V�[���ɕύX����
			manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<PopUpTextScene>(manager_)));		
			return;
		}
	}

	//���L�[�����������ă|�[�Y�V�[���ɖ߂�
	if (selectNum_ == input.inputNameTable_.size() + 1) 
	{
		if (input.IsTriggered(InputType::Space)) 
		{
			input.ResetInputInfo();
			updateFunc_ = &KeyConfigScene::FadeOutUpdate;
			return;
		}
	}
	
	//�ǂ̃L�[��ύX���邩��������
	if (input.IsTriggered(InputType::Space))
	{
		isEditing_ = !isEditing_;
		drawFunc_ = &KeyConfigScene::ChangeKeyPopUpText;
		changeKeyUpdateFunc_ = &KeyConfigScene::ChangeKeyborardUpdate;
		GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, pixel_width, pixel_param);
		return;
	}

	//�ЂƂO�̃V�[���ɖ߂�
	if (input.IsTriggered(InputType::Pause)) 
	{
		updateFunc_ = &KeyConfigScene::FadeOutUpdate;
		input.RollbackChangedInputInfo();
		return;
	}

	//�R���g���[���[�̓��͂��������Ƃ���
	//�R���g���[���[�p�̕`��ƍX�V�p�N���X�ɕύX����
	if (!input.LastInputDevice()) 
	{
		changeKeyUpdateFunc_ = &KeyConfigScene::ControllerUpdate;
		drawFunc_ = &KeyConfigScene::ControllerDraw;
	}
}

//�ύX����L�[���ǂ̃L�[�ɕύX����̂������肷��
void KeyConfigScene::ChangeKeyborardUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�L�[�{�[�h�ƃp�b�h�̓��͂𓾂�
	char keyState[max_key_num];
	GetHitKeyStateAll(keyState);

	int idx = 0;
	InputType currentType = InputType::Max;

	//��t���[���O�̓��͏�񂪎c���Ă����烊�^�[������
	for (const auto& key : input.lastInput_)
	{
		if (key == true) return;
	}

	//�����o�֐��|�C���^��ύX����L�[��I������֐��ɕύX����
	if (input.IsTriggered(InputType::Pause)) 
	{
		isEditing_ = !isEditing_;
		input.UndoSelectKey(static_cast<InputType>(selectNum_), InputCategory::keybd);
		drawFunc_ = &KeyConfigScene::KeyStateDraw;
		changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
		return;
	}

	//���݂̑I��inputNameTable���擾����
	for (const auto& name : input.inputNameTable_) 
	{
		if (selectNum_ == idx) 
		{
			currentType = name.first;
			break;
		}
		idx++;
	}

	//�L�[�{�[�h�̓��͂�ύX����
	for (int i = 0; i < max_key_num; i++)
	{
		if (keyState[i]) 
		{
			isEditing_ = !isEditing_;
			input.RewriteInputInfo(currentType, InputCategory::keybd, i);
			drawFunc_ = &KeyConfigScene::KeyStateDraw;
			changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
			break;
		}
	}
}

//�R���g���[���[�̏ꍇ�̍X�V
void KeyConfigScene::ControllerUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�I�������Ȃ�����0�ɂ���
	selectNum_ = 0;

	//���L�[�����������ă|�[�Y�V�[���ɖ߂�
	if (input.IsTriggered(InputType::Space))
	{
		input.ResetInputInfo();
		updateFunc_ = &KeyConfigScene::FadeOutUpdate;
		return;
	}

	//�ЂƂO�̃V�[���ɖ߂�
	if (input.IsTriggered(InputType::Pause))
	{
		updateFunc_ = &KeyConfigScene::FadeOutUpdate;
		input.RollbackChangedInputInfo();
		return;
	}

	//�L�[�{�[�h�̓��͂��������Ƃ���
	//�L�[�{�[�h�p�̕`��ƍX�V�p�N���X�ɕύX����
	if (input.LastInputDevice()) 
	{
		changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigScene::KeyStateDraw;
	}

}

//�t�F�[�h�C���̍X�V
void KeyConfigScene::FadeInUpdate()
{
	float timer = static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_);
	fadeValue_ = static_cast <int>(max_fade_value * timer);
	if (++fadeTimer_ == fadeInterval_) 
	{
		updateFunc_ = &KeyConfigScene::NormalUpdate;
		fadeValue_ = max_fade_value;
	}
}

//�ʏ펞�̍X�V
void KeyConfigScene::NormalUpdate()
{
	(this->*changeKeyUpdateFunc_)();
}

//�t�F�[�h�A�E�g�̍X�V
void KeyConfigScene::FadeOutUpdate()
{
	fadeValue_ = static_cast <int>(max_fade_value * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) 
	{
		manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<SettingScene>(manager_)));
		fadeValue_ = 0;
		return;
	}
}
