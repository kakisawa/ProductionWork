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
	//入力装置のボタン画像のチップサイズ
	constexpr int input_graph_chip_size = 59;
}

using json = nlohmann::json;

//コンストラクタ
InputState::InputState()
{
	//ポーズ
	defaultMapTable_[InputType::Pause] =		{ {InputCategory::keybd,KEY_INPUT_TAB},
												{InputCategory::pad,PAD_INPUT_8} };
	//上
	defaultMapTable_[InputType::Up] =			{ {InputCategory::keybd,KEY_INPUT_W},
												{InputCategory::pad,PAD_INPUT_UP} };
	//下
	defaultMapTable_[InputType::Down] =			{ {InputCategory::keybd,KEY_INPUT_S},
												{InputCategory::pad,PAD_INPUT_DOWN} };
	//左
	defaultMapTable_[InputType::Left] =			{ {InputCategory::keybd,KEY_INPUT_A},
												{InputCategory::pad,PAD_INPUT_LEFT} };
	//右
	defaultMapTable_[InputType::Right] =		{ {InputCategory::keybd,KEY_INPUT_D},
												{InputCategory::pad,PAD_INPUT_RIGHT} };
	//上矢印
	defaultMapTable_[InputType::UpArrow] =		{ {InputCategory::keybd,KEY_INPUT_UP}};

	//下矢印
	defaultMapTable_[InputType::DownArrow] =	{ {InputCategory::keybd,KEY_INPUT_DOWN}};

	//左矢印
	defaultMapTable_[InputType::LeftArrow] =	{ {InputCategory::keybd,KEY_INPUT_LEFT}};

	//右矢印
	defaultMapTable_[InputType::RightArrow] =	{ {InputCategory::keybd,KEY_INPUT_RIGHT}};

	//Space
	defaultMapTable_[InputType::Space] =		{ {InputCategory::keybd,KEY_INPUT_SPACE},
												{InputCategory::pad,PAD_INPUT_1} };
	//死亡
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

	//一時マップテーブルにコピー
	tempMapTable_ = inputMapTable_;

	//入力タイプの名前のテーブルを作る
	inputNameTable_[InputType::Pause]		= "ポーズ";
	inputNameTable_[InputType::Up]			= "上へ移動";
	inputNameTable_[InputType::Down]		= "下へ移動";
	inputNameTable_[InputType::Left]		= "左へ移動";
	inputNameTable_[InputType::Right]		= "右へ移動";
	inputNameTable_[InputType::UpArrow]		= "上を見る";
	inputNameTable_[InputType::DownArrow]	= "下を見る";
	inputNameTable_[InputType::LeftArrow]	= "左を見る";
	inputNameTable_[InputType::RightArrow]	= "右を見る";
	inputNameTable_[InputType::Space]		= "決定/ジャンプ";
	inputNameTable_[InputType::Death]		= "死亡";
	inputNameTable_[InputType::Dush]		= "走る";
	inputNameTable_[InputType::Activate]	= "アクション";

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

//デストラクタ
InputState::~InputState()
{
}

//長押し不可能入力関数
bool InputState::IsTriggered(const InputType type) const
{
	return IsPressed(type) && !lastInput_[static_cast<int>(type)];
}

//長押し可能入力関数
bool InputState::IsPressed(const InputType type) const
{
	return currentInput_[static_cast<int>(type)];
}

//更新
void InputState::Update()
{

	lastInput_ = currentInput_;//直前の入力情報を記憶しておく

	char keystate[256];
	GetHitKeyStateAll(keystate);//全キー情報取得

	int padState = GetJoypadInputState(DX_INPUT_PAD1);//パッド1コンの情報を取得する

	//マップの全情報をループする
	for (const auto& keymap : inputMapTable_) 
	{			
		//入力情報配列をループする
		for (const auto& input : keymap.second) 
		{		
			//この中身は、keybd,KEY_INPUT_RETURNなどのセット(InputInfo)が入ってる
			//keymap.secondには、この入力情報セットInputInfoが入っている。
			//keymap.firstには、対応するゲーム入力名の"InputType"などが入っている

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
			//2つの入力のうちどれかがtrueだったらもう「入力されてる」
			//とみなして、breakする。
			if (currentInput_[static_cast<int>(keymap.first)])
			{
				break;
			}
		}
	}
}

//変更中のキーコンフィグをtempMapTableに書き込む
void InputState::RewriteInputInfo(InputType type, InputCategory cat, const int id)
{
	//入力種別(割り当て先)がなければ、無効なので無視します。
	if (tempMapTable_.count(type) == 0)
	{
		return;
	}

	bool isRewrite = false;

	for (auto& info : tempMapTable_[type])
	{
		if (info.cat == cat)//カテゴリがヒットしたら
		{
			info.id = id;//IDを上書きする
			isRewrite = true;
			break;
		}
	}

	//もしカテゴリが存在しなかったら、ここで追加しておく
	if (!isRewrite)
	{
		tempMapTable_[type].push_back({ cat,id });
	}
}

//キーの変更を実行する
void InputState::CommitChangedInputInfo()
{
	inputMapTable_ = tempMapTable_;
}

//変更前のキーコンフィグに戻す
void InputState::RollbackChangedInputInfo()
{
	tempMapTable_ = inputMapTable_;
}

//キーコンフィグを初期化する
void InputState::ResetInputInfo()
{
	inputMapTable_ = defaultMapTable_;
	tempMapTable_ = defaultMapTable_;
}

//変更したキーを戻す
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

//キーコンフィグを外部ファイルとして保存する
void InputState::SavekeyInfo()
{
	//決め打ちしか出来ないのがネック
	json keyInfo_[static_cast<int>(InputType::Max)];

	//配列番号を進めるための変数(出来るなら書きたくない)
	int i = 0;
	for (const auto& input : inputMapTable_)
	{
		//inputMapTable_(map型)のキーをもとに値を取得する
		//vectorのサイズを値のサイズ分用意する
		std::vector<InputInfo> rewrite(input.second.size());
		rewrite = input.second;

		//※※※他の入力装置が増えた場合は手動でプログラムを増やさないと行けない※※※
		//各入力装置名と対応するidを取得
		//キーボード
		std::unordered_map<int, int> keybord;
		keybord[static_cast<int>(rewrite.front().cat)] = static_cast<int>(rewrite.front().id);
		//パッド
		std::unordered_map<int, int> pad;
		pad[static_cast<int>(rewrite.back().cat)] = static_cast<int>(rewrite.back().id);

		//各入力装置とid、サイズ、inputTypeをデータにまとめる
		keyInfo_[i] = 
		{
			{"inputType",static_cast<int>(input.first)},
			{"inputSize",static_cast<int>(input.second.size())},
			{"keybord",keybord},
			{"pad",pad},
		};
		i++;
	}

	//出力したデータをまとめる
	json keySize = 
	{
		{"name","keyInfo"},
		{"keyTypeNum",inputMapTable_.size()},
		{"keyInfo",keyInfo_}
	};

	//書き出すファイル名を"name"にする(別に無くてもいい)
	std::string filename = keySize["name"];
	std::string extension = ".json";
	filename += extension;
	filename = "data/jsonFile/" + filename;

	//出力
	std::ofstream writeingFile;
	//ファイルを開く
	writeingFile.open(filename, std::ios::out);
	//まとめたデータを一列にまとめる
	writeingFile << keySize.dump() << std::endl;
	//閉じる
	writeingFile.close();
}

//キーコンフィグを読み込む
void InputState::LoadKeyInfo(const std::string& filename)
{

	//初期化した中身を消す
	inputMapTable_.clear();

	//読み込むファイルのパスを生成
	std::string path = "data/jsonFile/";
	path += filename;
	path += ".json";

	//読み込むこと
	std::ifstream ifs(path.c_str());
	assert(ifs);

	//ビットをシフトする？
	//よくわかっていない
	//習ったけど覚えてない
	json json_;
	ifs >> json_;

	//名前を取得している(正直いらないけど残してる(笑))
	std::string name = json_["name"];
	//inputStateの総数
	int keyTypeNum = json_["keyTypeNum"];

	//決め打ちしか出来ないのがネック(知識、理解不足)
	json inputInfo[static_cast<int>(InputType::Max)];
	//↓嫌だこれ嫌い
	int i = 0;

	//各入力装置、idを取得
	for (const auto& keyInfo : json_["keyInfo"])
	{
		inputInfo[i] = keyInfo;
		i++;
	}

	//inputMapTable_に読み込んだデータを代入
	for (const auto& info : inputInfo)
	{
		//入力装置番号とキーidをそれぞれ保持するためのやつ(ひとまとめにできるなら死体)
		std::unordered_map<int, int> keybord;
		std::unordered_map<int, int> pad;

		//読み込んだ情報を一時保管するための変数
		InputInfo tempInfo = {};

		//データを取得
		int type = info["inputType"];
		int inputSize = info["inputSize"];
		keybord = info["keybord"];
		pad = info["pad"];

		//各入力装置とキーidをひとまとめに持つための変数
		std::vector<InputInfo> inputInfo(inputSize);

		//各入力装置とキーidを取得
		tempInfo.cat = static_cast<InputCategory>(keybord.begin()->first);
		tempInfo.id = keybord.begin()->second;
		inputInfo.front() = tempInfo;
		tempInfo.cat = static_cast<InputCategory>(pad.begin()->first);
		tempInfo.id = pad.begin()->second;
		inputInfo.back() = tempInfo;

		//読み込んだ情報を代入する
		inputMapTable_[static_cast<InputType>(type)] = inputInfo;
	}

	//一応閉じる
	ifs.close();
}

//最後に入力された入力装置を判別する
bool InputState::LastInputDevice() const
{
	return currentInputDevice_;
}

//入力装置の番号を取得する
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

//key画像の描画
void InputState::DrawKeyGraph(const int type, const float posX, const float posY, const float size)
{
	//入力装置の番号を取得する
	int num = GetInputNum(type, InputCategory::keybd);

	//入力装置の番号を自作の画像の番号に変換する
	num = static_cast<int>(keyNum_[num]);

	//画像の任意のキーが何列目の何行目にあるか取得する
	int graphX = num % 9;
	int graphY = num / 9;

	//画像の描画
	Graph::DrawRectRotaGraph(posX, posY,
							graphX * input_graph_chip_size,
							graphY * input_graph_chip_size, 
							input_graph_chip_size,
							input_graph_chip_size,
							size, 0.0f,
							GraphManager::GetInstance().GetGraph("key"),
							true, false);
}

//padのボタンの描画
void InputState::DrawPadGraph(const int type, const  float posX, const  float posY, const float scale)
{
	//画像の描画
	Graph::DrawRectRotaGraph(posX, posY,
							static_cast<int>(padNum_[type]) * input_graph_chip_size, 0,
							input_graph_chip_size, input_graph_chip_size,
							scale, 0.0f, 
							GraphManager::GetInstance().GetGraph("controllerBotton"),
							true, false);
}

//名前の描画
void InputState::DrawName(const int type, const  float posX, const  float posY, const  int color, const  int fontHandle, const bool editName, const bool before,const std::string& sign)
{
	//名前
	std::string name = inputNameTable_[static_cast<InputType>(type)].c_str();

	//名前を修正するか
	if (editName) 
	{
		if (before) 
		{
			//記号以降の文字を取得
			name = StrUtil::GetStringBeforeSign(name, sign);
		}
		else
		{
			//記号以前の文字を取得
			name = StrUtil::GetStringAfterSign(name, sign);
		}
	}

	DrawStringFToHandle(posX, posY, name.c_str(), color, fontHandle);
}