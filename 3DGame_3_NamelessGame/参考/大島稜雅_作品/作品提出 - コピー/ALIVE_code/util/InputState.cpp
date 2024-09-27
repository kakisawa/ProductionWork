#include "InputState.h"
#include "Util.h"
#include "DrawFunctions.h"
#include "GraphManager.h"

#include <DxLib.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include <cassert>
#include <nlohmann/json.hpp>

namespace
{
	//���͑��u�̃{�^���摜�̃`�b�v�T�C�Y
	constexpr int input_graph_chip_size = 59;
}

using json = nlohmann::json;

//�R���X�g���N�^
InputState::InputState()
{
	//�|�[�Y
	defaultMapTable_[InputType::Pause] =		{ {InputCategory::keybd,KEY_INPUT_TAB},
												{InputCategory::pad,PAD_INPUT_8} };
	//��
	defaultMapTable_[InputType::Up] =			{ {InputCategory::keybd,KEY_INPUT_W},
												{InputCategory::pad,PAD_INPUT_UP} };
	//��
	defaultMapTable_[InputType::Down] =			{ {InputCategory::keybd,KEY_INPUT_S},
												{InputCategory::pad,PAD_INPUT_DOWN} };
	//��
	defaultMapTable_[InputType::Left] =			{ {InputCategory::keybd,KEY_INPUT_A},
												{InputCategory::pad,PAD_INPUT_LEFT} };
	//�E
	defaultMapTable_[InputType::Right] =		{ {InputCategory::keybd,KEY_INPUT_D},
												{InputCategory::pad,PAD_INPUT_RIGHT} };
	//����
	defaultMapTable_[InputType::UpArrow] =		{ {InputCategory::keybd,KEY_INPUT_UP}};

	//�����
	defaultMapTable_[InputType::DownArrow] =	{ {InputCategory::keybd,KEY_INPUT_DOWN}};

	//�����
	defaultMapTable_[InputType::LeftArrow] =	{ {InputCategory::keybd,KEY_INPUT_LEFT}};

	//�E���
	defaultMapTable_[InputType::RightArrow] =	{ {InputCategory::keybd,KEY_INPUT_RIGHT}};

	//Space
	defaultMapTable_[InputType::Space] =		{ {InputCategory::keybd,KEY_INPUT_SPACE},
												{InputCategory::pad,PAD_INPUT_1} };
	//���S
	defaultMapTable_[InputType::Death] =		{ {InputCategory::keybd,KEY_INPUT_Z},
												{InputCategory::pad,PAD_INPUT_4} };
	//shift
	defaultMapTable_[InputType::Dush] =			{ {InputCategory::keybd,KEY_INPUT_LSHIFT},
												{InputCategory::pad,PAD_INPUT_3} };
	//activate
	defaultMapTable_[InputType::Activate] =		{ {InputCategory::keybd,KEY_INPUT_F},
												{InputCategory::pad,PAD_INPUT_2} };
	//creative
	defaultMapTable_[InputType::Creative] =		{ {InputCategory::keybd,KEY_INPUT_Q},
												{InputCategory::pad,PAD_INPUT_5} };

	inputMapTable_ = defaultMapTable_;

	//loadKeyInfo();
	LoadKeyInfo("keyInfo");

	//�ꎞ�}�b�v�e�[�u���ɃR�s�[
	tempMapTable_ = inputMapTable_;

	//���̓^�C�v�̖��O�̃e�[�u�������
	inputNameTable_[InputType::Pause]		= "�|�[�Y";
	inputNameTable_[InputType::Up]			= "��ֈړ�";
	inputNameTable_[InputType::Down]		= "���ֈړ�";
	inputNameTable_[InputType::Left]		= "���ֈړ�";
	inputNameTable_[InputType::Right]		= "�E�ֈړ�";
	inputNameTable_[InputType::UpArrow]		= "�������";
	inputNameTable_[InputType::DownArrow]	= "��������";
	inputNameTable_[InputType::LeftArrow]	= "��������";
	inputNameTable_[InputType::RightArrow]	= "�E������";
	inputNameTable_[InputType::Space]		= "����/�W�����v";
	inputNameTable_[InputType::Death]		= "���S";
	inputNameTable_[InputType::Dush]		= "����";
	inputNameTable_[InputType::Activate]	= "�A�N�V����";

	currentInput_.resize(static_cast<int>(InputType::Max));
	lastInput_.resize(static_cast<int>(InputType::Max));

	{
		keyNum_[  0] = Key::ESC;
		keyNum_[  1] = Key::Key_1;
		keyNum_[  2] = Key::Key_2;
		keyNum_[  3] = Key::Key_3;
		keyNum_[  4] = Key::Key_4;
		keyNum_[  5] = Key::Key_5;
		keyNum_[  6] = Key::Key_6;
		keyNum_[  7] = Key::Key_7;
		keyNum_[  8] = Key::Key_8;
		keyNum_[  9] = Key::Key_9;
		keyNum_[ 10] = Key::Key_0;
		keyNum_[ 11] = Key::Key_equal;

		keyNum_[ 14] = Key::Back;
		keyNum_[ 15] = Key::Tab;
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

	{
		padNum_[static_cast<int>(InputType::Up)]		= XboxBotton::Up;
		padNum_[static_cast<int>(InputType::Down)]		= XboxBotton::Down;
		padNum_[static_cast<int>(InputType::Left)]		= XboxBotton::Left;
		padNum_[static_cast<int>(InputType::Right)]		= XboxBotton::Right;
		padNum_[static_cast<int>(InputType::Space)]		= XboxBotton::A;
		padNum_[static_cast<int>(InputType::Activate)]	= XboxBotton::B;
		padNum_[static_cast<int>(InputType::Dush)]		= XboxBotton::X;
		padNum_[static_cast<int>(InputType::Death)]		= XboxBotton::Y;
	}
}

//�f�X�g���N�^
InputState::~InputState()
{
}

//�������s�\���͊֐�
bool InputState::IsTriggered(const InputType type) const
{
	return IsPressed(type) && !lastInput_[static_cast<int>(type)];
}

//�������\���͊֐�
bool InputState::IsPressed(const InputType type) const
{
	return currentInput_[static_cast<int>(type)];
}

//�X�V
void InputState::Update()
{

	lastInput_ = currentInput_;//���O�̓��͏����L�����Ă���

	char keystate[256];
	GetHitKeyStateAll(keystate);//�S�L�[���擾

	int padState = GetJoypadInputState(DX_INPUT_PAD1);//�p�b�h1�R���̏����擾����

	//�}�b�v�̑S�������[�v����
	for (const auto& keymap : inputMapTable_) 
	{			
		//���͏��z������[�v����
		for (const auto& input : keymap.second) 
		{		
			//���̒��g�́Akeybd,KEY_INPUT_RETURN�Ȃǂ̃Z�b�g(InputInfo)�������Ă�
			//keymap.second�ɂ́A���̓��͏��Z�b�gInputInfo�������Ă���B
			//keymap.first�ɂ́A�Ή�����Q�[�����͖���"InputType"�Ȃǂ������Ă���

			if (input.cat == InputCategory::keybd)
			{
				currentInput_[static_cast<int>(keymap.first)] = keystate[input.id];
				if (currentInput_[static_cast<int>(keymap.first)])
				{
					currentInputDevice_ = true;
				}
			}
			else if (input.cat == InputCategory::pad) 
			{
				currentInput_[static_cast<int>(keymap.first)] = padState & input.id;
				if (currentInput_[static_cast<int>(keymap.first)])
				{
					currentInputDevice_ = false;
				}
			}
			//2�̓��͂̂����ǂꂩ��true������������u���͂���Ă�v
			//�Ƃ݂Ȃ��āAbreak����B
			if (currentInput_[static_cast<int>(keymap.first)])
			{
				break;
			}
		}
	}
}

//�ύX���̃L�[�R���t�B�O��tempMapTable�ɏ�������
void InputState::RewriteInputInfo(InputType type, InputCategory cat, const int id)
{
	//���͎��(���蓖�Đ�)���Ȃ���΁A�����Ȃ̂Ŗ������܂��B
	if (tempMapTable_.count(type) == 0)
	{
		return;
	}

	bool isRewrite = false;

	for (auto& info : tempMapTable_[type])
	{
		if (info.cat == cat)//�J�e�S�����q�b�g������
		{
			info.id = id;//ID���㏑������
			isRewrite = true;
			break;
		}
	}

	//�����J�e�S�������݂��Ȃ�������A�����Œǉ����Ă���
	if (!isRewrite)
	{
		tempMapTable_[type].push_back({ cat,id });
	}
}

//�L�[�̕ύX�����s����
void InputState::CommitChangedInputInfo()
{
	inputMapTable_ = tempMapTable_;
}

//�ύX�O�̃L�[�R���t�B�O�ɖ߂�
void InputState::RollbackChangedInputInfo()
{
	tempMapTable_ = inputMapTable_;
}

//�L�[�R���t�B�O������������
void InputState::ResetInputInfo()
{
	inputMapTable_ = defaultMapTable_;
	tempMapTable_ = defaultMapTable_;
}

//�ύX�����L�[��߂�
void InputState::UndoSelectKey(const InputType type, const InputCategory cat)
{
	for (auto& info : tempMapTable_[type]) 
	{
		if (info.cat == cat) 
		{
			info.id = inputMapTable_[type].begin()->id;
			break;
		}
	}
}

//�L�[�R���t�B�O���O���t�@�C���Ƃ��ĕۑ�����
void InputState::SavekeyInfo()
{
	//���ߑł������o���Ȃ��̂��l�b�N
	json keyInfo_[static_cast<int>(InputType::Max)];

	//�z��ԍ���i�߂邽�߂̕ϐ�(�o����Ȃ珑�������Ȃ�)
	int i = 0;
	for (const auto& input : inputMapTable_)
	{
		//inputMapTable_(map�^)�̃L�[�����Ƃɒl���擾����
		//vector�̃T�C�Y��l�̃T�C�Y���p�ӂ���
		std::vector<InputInfo> rewrite(input.second.size());
		rewrite = input.second;

		//���������̓��͑��u���������ꍇ�͎蓮�Ńv���O�����𑝂₳�Ȃ��ƍs���Ȃ�������
		//�e���͑��u���ƑΉ�����id���擾
		//�L�[�{�[�h
		std::unordered_map<int, int> keybord;
		keybord[static_cast<int>(rewrite.front().cat)] = static_cast<int>(rewrite.front().id);
		//�p�b�h
		std::unordered_map<int, int> pad;
		pad[static_cast<int>(rewrite.back().cat)] = static_cast<int>(rewrite.back().id);

		//�e���͑��u��id�A�T�C�Y�AinputType���f�[�^�ɂ܂Ƃ߂�
		keyInfo_[i] = 
		{
			{"inputType",static_cast<int>(input.first)},
			{"inputSize",static_cast<int>(input.second.size())},
			{"keybord",keybord},
			{"pad",pad},
		};
		i++;
	}

	//�o�͂����f�[�^���܂Ƃ߂�
	json keySize = 
	{
		{"name","keyInfo"},
		{"keyTypeNum",inputMapTable_.size()},
		{"keyInfo",keyInfo_}
	};

	//�����o���t�@�C������"name"�ɂ���(�ʂɖ����Ă�����)
	std::string filename = keySize["name"];
	std::string extension = ".json";
	filename += extension;
	filename = "data/jsonFile/" + filename;

	//�o��
	std::ofstream writeingFile;
	//�t�@�C�����J��
	writeingFile.open(filename, std::ios::out);
	//�܂Ƃ߂��f�[�^�����ɂ܂Ƃ߂�
	writeingFile << keySize.dump() << std::endl;
	//����
	writeingFile.close();
}

//�L�[�R���t�B�O��ǂݍ���
void InputState::LoadKeyInfo(const std::string& filename)
{

	//�������������g������
	inputMapTable_.clear();

	//�ǂݍ��ރt�@�C���̃p�X�𐶐�
	std::string path = "data/jsonFile/";
	path += filename;
	path += ".json";

	//�ǂݍ��ނ���
	std::ifstream ifs(path.c_str());
	assert(ifs);

	//�r�b�g���V�t�g����H
	//�悭�킩���Ă��Ȃ�
	//�K�������Ǌo���ĂȂ�
	json json_;
	ifs >> json_;

	//���O���擾���Ă���(��������Ȃ����ǎc���Ă�(��))
	std::string name = json_["name"];
	//inputState�̑���
	int keyTypeNum = json_["keyTypeNum"];

	//���ߑł������o���Ȃ��̂��l�b�N(�m���A����s��)
	json inputInfo[static_cast<int>(InputType::Max)];
	//���������ꌙ��
	int i = 0;

	//�e���͑��u�Aid���擾
	for (const auto& keyInfo : json_["keyInfo"])
	{
		inputInfo[i] = keyInfo;
		i++;
	}

	//inputMapTable_�ɓǂݍ��񂾃f�[�^����
	for (const auto& info : inputInfo)
	{
		//���͑��u�ԍ��ƃL�[id�����ꂼ��ێ����邽�߂̂��(�ЂƂ܂Ƃ߂ɂł���Ȃ玀��)
		std::unordered_map<int, int> keybord;
		std::unordered_map<int, int> pad;

		//�ǂݍ��񂾏����ꎞ�ۊǂ��邽�߂̕ϐ�
		InputInfo tempInfo = {};

		//�f�[�^���擾
		int type = info["inputType"];
		int inputSize = info["inputSize"];
		keybord = info["keybord"];
		pad = info["pad"];

		//�e���͑��u�ƃL�[id���ЂƂ܂Ƃ߂Ɏ����߂̕ϐ�
		std::vector<InputInfo> inputInfo(inputSize);

		//�e���͑��u�ƃL�[id���擾
		tempInfo.cat = static_cast<InputCategory>(keybord.begin()->first);
		tempInfo.id = keybord.begin()->second;
		inputInfo.front() = tempInfo;
		tempInfo.cat = static_cast<InputCategory>(pad.begin()->first);
		tempInfo.id = pad.begin()->second;
		inputInfo.back() = tempInfo;

		//�ǂݍ��񂾏���������
		inputMapTable_[static_cast<InputType>(type)] = inputInfo;
	}

	//�ꉞ����
	ifs.close();
}

//�Ō�ɓ��͂��ꂽ���͑��u�𔻕ʂ���
bool InputState::LastInputDevice() const
{
	return currentInputDevice_;
}

//���͑��u�̔ԍ����擾����
int InputState::GetInputNum(const int num, const InputCategory cat)
{
	int keyNum = 0;

	auto type = static_cast<InputType>(num);

	for (const auto& key : inputMapTable_[type])
	{
		if (key.cat == cat)
		{
			keyNum = static_cast<int>(key.id);
			break;
		}
	}

	return keyNum;
}

//key�摜�̕`��
void InputState::DrawKeyGraph(const int type, const float posX, const float posY, const float size)
{
	//���͑��u�̔ԍ����擾����
	int num = GetInputNum(type, InputCategory::keybd);

	//���͑��u�̔ԍ�������̉摜�̔ԍ��ɕϊ�����
	num = static_cast<int>(keyNum_[num]);

	//�摜�̔C�ӂ̃L�[������ڂ̉��s�ڂɂ��邩�擾����
	int graphX = num % 9;
	int graphY = num / 9;

	//�摜�̕`��
	Graph::DrawRectRotaGraph(posX, posY,
							graphX * input_graph_chip_size,
							graphY * input_graph_chip_size, 
							input_graph_chip_size,
							input_graph_chip_size,
							size, 0.0f,
							GraphManager::GetInstance().GetGraph("key"),
							true, false);
}

//pad�̃{�^���̕`��
void InputState::DrawPadGraph(const int type, const  float posX, const  float posY, const float scale)
{
	//�摜�̕`��
	Graph::DrawRectRotaGraph(posX, posY,
							static_cast<int>(padNum_[type]) * input_graph_chip_size, 0,
							input_graph_chip_size, input_graph_chip_size,
							scale, 0.0f, 
							GraphManager::GetInstance().GetGraph("controllerBotton"),
							true, false);
}

//���O�̕`��
void InputState::DrawName(const int type, const  float posX, const  float posY, const  int color, const  int fontHandle, const bool editName, const bool before,const std::string& sign)
{
	//���O
	std::string name = inputNameTable_[static_cast<InputType>(type)].c_str();

	//���O���C�����邩
	if (editName) 
	{
		if (before) 
		{
			//�L���ȍ~�̕������擾
			name = StrUtil::GetStringBeforeSign(name, sign);
		}
		else
		{
			//�L���ȑO�̕������擾
			name = StrUtil::GetStringAfterSign(name, sign);
		}
	}

	DrawStringFToHandle(posX, posY, name.c_str(), color, fontHandle);
}