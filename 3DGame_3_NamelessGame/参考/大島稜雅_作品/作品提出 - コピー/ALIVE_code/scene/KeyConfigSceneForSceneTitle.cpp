#include "KeyConfigSceneForSceneTitle.h"
#include "SceneManager.h"
#include "SettingScene.h"
#include "PopUpTextScene.h"
#include "SettingSceneForSceneTitle.h"

#include "../util/game.h"
#include "../util/InputState.h"
#include "../util/FontsManager.h"
#include "../util/DrawFunctions.h"
#include "../util/ExternalFile.h"
#include "../util/GraphManager.h"
#include "../util/UIItemManager.h"

#include <DxLib.h>
#include <algorithm>

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

	//�ύXUI�ƃL�����Z��UI�̐�
	constexpr int change_and_cancel_UI_Num = 2;

	//�|�b�v�A�b�v�e�L�X�g��alphaBlend�̍ő�l
	constexpr int pop_up_text_max_alpha_blend_value = 100;

	//UI�z�u�|�W�V����X
	constexpr int UI_pos_x = 320;

	//UI�z�u�|�W�V����Y
	constexpr int UI_pos_y = 100;

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

	//�摜������X
	constexpr int division_graph_x = 9;

	//�摜������Y
	constexpr int division_graph_y = 13;

	//�摜��������
	constexpr int division_graph_total = division_graph_x * division_graph_y;

	//key�摜�̈ꖇ������̃T�C�Y
	constexpr int graph_chip_size = 59;

	//�I�����ꂽ�摜�̃A���t�@�l
	constexpr float selection_alpha_value = 255;

	//�I�����ꂽ�摜�̃A���t�@�l
	constexpr float not_selection_alpha_value = 150;

	//�摜�̒��S���WX
	constexpr float graph_center_x = 0.5f;

	//�摜�̒��S���WX
	constexpr float graph_center_y = 0.5f;

	//�摜�̌��ԃT�C�Y
	constexpr float graph_gap_size = 5.5f;

	//�摜�̊g�k��
	constexpr float graph_scale_size = 5.0f;

	//�摜�̊g�k��
	constexpr float graph_expansion_scale_size = 5.2f;

	//�����̊g�k��
	constexpr float str_scale_size = 2.5f;

	//�r���{�[�g�̃T�C�Y
	constexpr float bill_board_size = 50.0f;

	//�R���g���[���[�摜�̂̃T�C�Y
	constexpr float controller_graph_size = 90.0f;
}

//�R���X�g���N�^
KeyConfigSceneForSceneTitle::KeyConfigSceneForSceneTitle(SceneManager& manager) :SceneBase(manager),updateFunc_(&KeyConfigSceneForSceneTitle::FadeInUpdate)
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	if (!input.LastInputDevice()) 
	{
		changeKeyUpdateFunc_ = &KeyConfigSceneForSceneTitle::ControllerUpdate;
		drawFunc_ = &KeyConfigSceneForSceneTitle::ControllerDraw;
	}
	else 
	{
		changeKeyUpdateFunc_ = &KeyConfigSceneForSceneTitle::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigSceneForSceneTitle::KeyStateDraw;
	}

	{
		keyNum_[0] = Key::ESC;
		keyNum_[1] = Key::Key_1;
		keyNum_[2] = Key::Key_2;
		keyNum_[3] = Key::Key_3;
		keyNum_[4] = Key::Key_4;
		keyNum_[5] = Key::Key_5;
		keyNum_[6] = Key::Key_6;
		keyNum_[7] = Key::Key_7;
		keyNum_[8] = Key::Key_8;
		keyNum_[9] = Key::Key_9;
		keyNum_[10] = Key::Key_0;
		keyNum_[11] = Key::Key_equal;

		keyNum_[14] = Key::Back;
		keyNum_[15] = Key::Tab;
		keyNum_[16] = Key::Q;
		keyNum_[17] = Key::W;
		keyNum_[18] = Key::E;
		keyNum_[19] = Key::R;
		keyNum_[20] = Key::T;
		keyNum_[21] = Key::Y;
		keyNum_[22] = Key::U;
		keyNum_[23] = Key::I;
		keyNum_[24] = Key::O;
		keyNum_[25] = Key::P;
		keyNum_[26] = Key::Left_parenthesis_key;
		keyNum_[27] = Key::Right_parenthesis_key;
		keyNum_[28] = Key::Big_enter;
		keyNum_[29] = Key::L_ctrl;
		keyNum_[30] = Key::A;
		keyNum_[31] = Key::S;
		keyNum_[32] = Key::D;
		keyNum_[33] = Key::F;
		keyNum_[34] = Key::G;
		keyNum_[35] = Key::H;
		keyNum_[36] = Key::J;
		keyNum_[37] = Key::K;
		keyNum_[38] = Key::L;
		keyNum_[39] = Key::Semicolon;

		keyNum_[42] = Key::L_shift;
		keyNum_[43] = Key::Backslash;
		keyNum_[44] = Key::Z;
		keyNum_[45] = Key::X;
		keyNum_[46] = Key::C;
		keyNum_[47] = Key::V;
		keyNum_[48] = Key::B;
		keyNum_[49] = Key::N;
		keyNum_[50] = Key::M;
		keyNum_[51] = Key::Comma;
		keyNum_[52] = Key::Period;
		keyNum_[53] = Key::Slash;
		keyNum_[54] = Key::R_shift;
		keyNum_[55] = Key::Asterisk;
		keyNum_[56] = Key::L_alt;
		keyNum_[57] = Key::Space;
		keyNum_[58] = Key::CaspLock;
		keyNum_[59] = Key::F1;
		keyNum_[60] = Key::F2;
		keyNum_[61] = Key::F3;
		keyNum_[62] = Key::F4;
		keyNum_[63] = Key::F5;
		keyNum_[64] = Key::F6;
		keyNum_[65] = Key::F7;
		keyNum_[66] = Key::F8;
		keyNum_[67] = Key::F9;
		keyNum_[68] = Key::F10;
		keyNum_[69] = Key::NumLock;
		keyNum_[70] = Key::ScrollLock;
		keyNum_[71] = Key::Key10_7;
		keyNum_[72] = Key::Key10_8;
		keyNum_[73] = Key::Key10_9;
		keyNum_[74] = Key::Key10_minus;
		keyNum_[75] = Key::Key10_4;
		keyNum_[76] = Key::Key10_5;
		keyNum_[77] = Key::Key10_6;
		keyNum_[78] = Key::Key10_plus;
		keyNum_[79] = Key::Key10_1;
		keyNum_[80] = Key::Key10_2;
		keyNum_[81] = Key::Key10_3;
		keyNum_[82] = Key::Key10_0;
		keyNum_[83] = Key::Key10_period;

		keyNum_[87] = Key::F11;
		keyNum_[88] = Key::F12;
		keyNum_[112] = Key::KANA;
		keyNum_[121] = Key::Conversion;
		keyNum_[123] = Key::NoConversion;
		keyNum_[125] = Key::YEN_mark;
		keyNum_[144] = Key::Caret;
		keyNum_[145] = Key::At_mark;
		keyNum_[146] = Key::Colon;
		keyNum_[148] = Key::ChineseCharacters;
		keyNum_[156] = Key::Key10_enter;
		keyNum_[157] = Key::R_ctrl;
		keyNum_[181] = Key::Key10_slash;
		keyNum_[183] = Key::Print;
		keyNum_[184] = Key::Ralt;
		keyNum_[197] = Key::Pause;
		keyNum_[199] = Key::Home;
		keyNum_[200] = Key::Up_Arrow;
		keyNum_[201] = Key::Page_up;
		keyNum_[203] = Key::Left_Arrow;
		keyNum_[205] = Key::Right_Arrow;
		keyNum_[207] = Key::END;
		keyNum_[208] = Key::Down_Arrow;
		keyNum_[209] = Key::Page_Down;
		keyNum_[210] = Key::Insert;
		keyNum_[211] = Key::Delete;
		keyNum_[219] = Key::L_win;
		keyNum_[220] = Key::Rwin;
		keyNum_[221] = Key::App;
	}
}

//�f�X�g���N�^
KeyConfigSceneForSceneTitle::~KeyConfigSceneForSceneTitle()
{
}

//������
void KeyConfigSceneForSceneTitle::Init()
{
	//�L�[�O���t�𕪊����ēǂݍ���
	LoadDivGraph("data/graph/key.png", division_graph_total,division_graph_x, division_graph_y,graph_chip_size, graph_chip_size,keyTypeHandle_);

	//�t�H���g�̍쐬
	fontHandleSize21_ = FontsManager::GetInstance().GetFontHandle("�s�O�� 0021");
	fontHandleSize42_ = FontsManager::GetInstance().GetFontHandle("�s�O�� 0042");

	//�C���X�^���X��
	KeyUI_ = std::make_shared<UIItemManager>();
	PadUI_ = std::make_shared<UIItemManager>();

	//�Z�k��
	auto& input = InputState::GetInstance();
	auto& file = ExternalFile::GetInstance();

	//�L�[�̖����̈�ڂ̍��W
	float namePosX = static_cast<float>(Game::screen_width / screen_division_width * key_role_division_x);
	float namePosY = static_cast<float>(Game::screen_height / screen_division_height * key_role_division_y);

	VECTOR pos = file.GetUIPos("keyConfPos");
	int inputSize = static_cast<int>(input.inputNameTable_.size());

	int no = 0;
	for (const auto& input : input.inputNameTable_)
	{
		keyDrawPos_[input.second] = pos;
		pos.y -= graph_gap_size;
		if (no == inputSize / half)
		{
			pos = file.GetUIPos("keyConfTurnBackPos");
		}
		no++;
	}

	keyDrawPos_["�ύX"] = file.GetUIPos("advancedSettingUIPos");
	keyDrawPos_["�L�����Z��"] = file.GetUIPos("backUIPos");

	padDrawPos_["�߂�"] = file.GetUIPos("backUIPos");

	int nameNo = 0;
	for (const auto& table : input.inputNameTable_)
	{
		//���j���[�̒ǉ�
		KeyUI_->AddMenu(namePosX, namePosY, UI_pos_x, UI_pos_y, table.second.c_str(), fontHandleSize21_);
	
		//�|�W�V��������
		namePosY += graph_chip_size + graph_gap_size;
	
		//inputstate.tempmaptable�̔����𒴂�����܂�Ԃ�
		if (nameNo == inputSize / half) 
		{
			namePosX += static_cast<float>(Game::screen_width / screen_division_width * key_role_division_turn_back_x);
			namePosY =  static_cast<float>(Game::screen_height / screen_division_height * key_role_division_y);
		}
		nameNo++;
	}

	//���j���[�̒ǉ�
	KeyUI_->AddMenu(static_cast<float>(Game::screen_width / half), static_cast<float>(Game::screen_height / screen_division_height * change_division_y), UI_pos_x, UI_pos_y, "�ύX", fontHandleSize21_);
	KeyUI_->AddMenu(static_cast<float>(Game::screen_width / half), static_cast<float>(Game::screen_height / screen_division_height * cancel_division_y), UI_pos_x, UI_pos_y, "�L�����Z��", fontHandleSize21_);
	PadUI_->AddMenu(static_cast<float>(Game::screen_width / half), static_cast<float>(Game::screen_height / screen_division_height * cancel_division_y), UI_pos_x, UI_pos_y, "�߂�", fontHandleSize21_);
}

//�I��
void KeyConfigSceneForSceneTitle::End()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//���݂̃L�[���͏����O���f�[�^�Ƃ��ď����o��
	input.SavekeyInfo();

	for (const auto& graph : keyTypeHandle_)
	{
		DeleteGraph(graph);
	}
}

//�����o�֐��|�C���^�̍X�V
void KeyConfigSceneForSceneTitle::Update()
{
	(this->*updateFunc_)();
}

//�`��
void KeyConfigSceneForSceneTitle::Draw()
{
	(this->*drawFunc_)();
}

//�I�𑀍�
void KeyConfigSceneForSceneTitle::SelectionOperation()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//���݂̃L�[�̐����擾����
	const int keyNum = static_cast<int>(input.inputNameTable_.size() + change_and_cancel_UI_Num);

	if (input.IsTriggered(InputType::Up))
	{
		if (selectNum_ == (keyNum - 1) / half)
		{
			selectNum_ += keyNum / half;
		}
		else
		{
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
void KeyConfigSceneForSceneTitle::KeyStateDraw()
{
	KeyUI_->AlphaChangeDrawBillBoard(keyDrawPos_, selectNum_, fadeValue_, bill_board_size);

	//�L�[�ɑΉ�����摜��`�悷��
	KeyGraphDraw();
}

//key�̉摜��`��
void KeyConfigSceneForSceneTitle::KeyGraphDraw()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//dxlib���̊e�L�[�ԍ�����肷��
	int keyNum = 0;

	//�A���t�@�l
	float alphaValue = 0;

	//�摜�̊g�k��
	float graphScale = graph_scale_size;

	float choosingValue = selection_alpha_value / selection_alpha_value;
	float notChoosingValue = not_selection_alpha_value / selection_alpha_value;

	VECTOR pos = ExternalFile::GetInstance().GetUIPos("keyGraphPos");

	int no = 0;
	for (const auto& key : input.tempMapTable_)
	{
		if (key.first == InputType::Creative) 
		{
			continue;
		}

		//key�ԍ����擾����
		keyNum = static_cast<int>(keyNum_[key.second.begin()->id]);

		//���݃J�[�\���������Ă���ꍇ
		//�T�C�Y�Ɩ��邳�������l��ύX����
		if (no == selectNum_)
		{
			graphScale = graph_expansion_scale_size;
			alphaValue = choosingValue * fadeValue_;
		}
		else
		{
			graphScale = graph_scale_size;
			alphaValue = notChoosingValue * fadeValue_;
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alphaValue));
		DrawBillboard3D(pos, graph_center_x, graph_center_y, graphScale, 0.0f, keyTypeHandle_[keyNum], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		pos.y -= graph_gap_size;

		if (no == input.tempMapTable_.size() / half - 1)
		{
			pos = ExternalFile::GetInstance().GetUIPos("keyGraphTurnBackPos");
		}
		no++;
	}
}

//�R���g���[���[���g�p����Ă���ꍇ�̕`��
void KeyConfigSceneForSceneTitle::ControllerDraw()
{
	//UI�z�u�|�W�V����
	VECTOR pos = ExternalFile::GetInstance().GetUIPos("controllerUIPos");

	//�R���g���[���[�摜�̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBillboard3D(pos, graph_center_x, graph_center_y, controller_graph_size, 0.0f, GraphManager::GetInstance().GetGraph("controller"), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//UI�̕`��
	PadUI_->AlphaChangeDrawBillBoard(padDrawPos_, selectNum_, fadeValue_, bill_board_size);
}

//�ύX������key����͂����邽�߂̃|�b�v�A�b�v�`��
void KeyConfigSceneForSceneTitle::ChangeKeyPopUpText()
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
	std::string text = input.inputNameTable_.find(static_cast<InputType>(selectNum_))->second.c_str();

	//������̉���
	int strWidth = GetDrawStringWidthToHandle(text.c_str(), static_cast<int>(text.size()), fontHandleSize21_);

	//�I�������L�[�̖��O���o��
	DrawStringToHandle(Game::screen_width / screen_division_width * pop_up_background_division_left_x + strWidth / half,
					   Game::screen_height / screen_division_height * cange_pop_up_graph_screen_division_y,
					   text.c_str(), 0xffffff, fontHandleSize21_);

	//�L�[�摜�̕`��
	input.DrawKeyGraph(selectNum_,
					   static_cast<float>(Game::screen_width / screen_division_width * pop_up_background_division_left_x + strWidth * str_scale_size),
					   static_cast<float>(Game::screen_height / screen_division_height * pop_up_background_division_up_y - graph_chip_size / half),
					   1.0f);

	//������
	text = "�ύX�������L�[����͂��Ă�������";
	//������̉���
	strWidth = GetDrawStringWidthToHandle(text.c_str(), static_cast<int>(text.size()), fontHandleSize42_);
	//������̕`��
	DrawStringToHandle(Game::screen_width / half - strWidth / half, Game::screen_height / half, text.c_str(), 0xffffff, fontHandleSize42_);

}

//�ύX�������L�[��I������
void KeyConfigSceneForSceneTitle::SelectChangeKeyUpdate()
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
			updateFunc_ = &KeyConfigSceneForSceneTitle::FadeOutUpdate;
			return;
		}
	}

	//�ǂ̃L�[��ύX���邩��������
	if (input.IsTriggered(InputType::Space)) 
	{
		isEditing_ = !isEditing_;
		drawFunc_ = &KeyConfigSceneForSceneTitle::ChangeKeyPopUpText;
		changeKeyUpdateFunc_ = &KeyConfigSceneForSceneTitle::ChangeKeyborardUpdate;
		return;
	}

	//�ЂƂO�̃V�[���ɖ߂�
	if (input.IsTriggered(InputType::Pause))
	{
		updateFunc_ = &KeyConfigSceneForSceneTitle::FadeOutUpdate;
		input.RollbackChangedInputInfo();
		return;
	}

	//�R���g���[���[�̓��͂��������Ƃ���
	//�R���g���[���[�p�̕`��ƍX�V�p�N���X�ɕύX����
	if (!input.LastInputDevice()) 
	{
		changeKeyUpdateFunc_ = &KeyConfigSceneForSceneTitle::ControllerUpdate;
		drawFunc_ = &KeyConfigSceneForSceneTitle::ControllerDraw;
	}
}

//�ύX�������L�[���ǂ̃L�[�ɕύX����̂�
void KeyConfigSceneForSceneTitle::ChangeKeyborardUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

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
		drawFunc_ = &KeyConfigSceneForSceneTitle::KeyStateDraw;
		changeKeyUpdateFunc_ = &KeyConfigSceneForSceneTitle::SelectChangeKeyUpdate;
		return;
	}

	//�L�[�{�[�h�ƃp�b�h�̓��͂𓾂�
	char keyState[max_key_num];
	auto padState = GetJoypadInputState(DX_INPUT_PAD1);
	GetHitKeyStateAll(keyState);

	int idx = 0;
	InputType currentType = InputType::Max;

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
			drawFunc_ = &KeyConfigSceneForSceneTitle::KeyStateDraw;
			changeKeyUpdateFunc_ = &KeyConfigSceneForSceneTitle::SelectChangeKeyUpdate;
			break;
		}
	}
	//�p�b�h�̓��͂�ύX����
	if (padState != 0)
	{
		isEditing_ = !isEditing_;
		input.RewriteInputInfo(currentType, InputCategory::pad, padState);
		drawFunc_ = &KeyConfigSceneForSceneTitle::KeyStateDraw;
		changeKeyUpdateFunc_ = &KeyConfigSceneForSceneTitle::SelectChangeKeyUpdate;
	}
}

//�R���g���[���[�̏ꍇ�̍X�V
void KeyConfigSceneForSceneTitle::ControllerUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	selectNum_ = 0;

	//���L�[�����������ă|�[�Y�V�[���ɖ߂�
	if (input.IsTriggered(InputType::Space))
	{
		input.ResetInputInfo();
		updateFunc_ = &KeyConfigSceneForSceneTitle::FadeOutUpdate;
		return;
	}

	//�ЂƂO�̃V�[���ɖ߂�
	if (input.IsTriggered(InputType::Pause)) 
	{
		updateFunc_ = &KeyConfigSceneForSceneTitle::FadeOutUpdate;
		input.RollbackChangedInputInfo();
		return;
	}

	//�L�[�{�[�h�̓��͂��������Ƃ���
	//�L�[�{�[�h�p�̕`��ƍX�V�p�N���X�ɕύX����
	if (input.LastInputDevice())
	{
		drawFunc_ = &KeyConfigSceneForSceneTitle::KeyStateDraw;
		changeKeyUpdateFunc_ = &KeyConfigSceneForSceneTitle::SelectChangeKeyUpdate;
	}
}

//�t�F�[�h�C���̍X�V
void KeyConfigSceneForSceneTitle::FadeInUpdate()
{
	float timer = static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_);
	fadeValue_ = static_cast <int>(max_fade_value * timer);
	if (++fadeTimer_ == fadeInterval_)
	{
		updateFunc_ = &KeyConfigSceneForSceneTitle::NormalUpdate;
		fadeValue_ = max_fade_value;
	}
}

//�ʏ펞�̍X�V
void KeyConfigSceneForSceneTitle::NormalUpdate()
{
	(this->*changeKeyUpdateFunc_)();
}

//�t�F�[�h�A�E�g�̍X�V
void KeyConfigSceneForSceneTitle::FadeOutUpdate()
{
	fadeValue_ = static_cast <int>(max_fade_value * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0)
	{
		manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<SettingSceneForSceneTitle>(manager_)));
		fadeValue_ = 0;
		return;
	}
}