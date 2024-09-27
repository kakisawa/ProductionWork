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
	//半分
	constexpr int half = 2;

	//画面横の分割数
	constexpr int screen_division_width = 128;

	//画面縦の分割数
	constexpr int screen_division_height = 72;

	//フェードの最大値
	constexpr int max_fade_value = 255;

	//キーの数
	constexpr int max_key_num = 256;

	//変更UIとキャンセルUIの数
	constexpr int change_and_cancel_UI_Num = 2;

	//ポップアップテキストのalphaBlendの最大値
	constexpr int pop_up_text_max_alpha_blend_value = 100;

	//UI配置ポジションX
	constexpr int UI_pos_x = 320;

	//UI配置ポジションY
	constexpr int UI_pos_y = 100;

	//画像UI配置ポジションYの分割数
	constexpr int graph_screen_division_y = 10;

	//変更ポップアップ画像UI配置ポジションYの分割数
	constexpr int cange_pop_up_graph_screen_division_y = 11;

	//キーの役割りのUI配置ポジションXの分割数
	constexpr int key_role_division_x = 32;

	//キーの役割りのUI配置ポジションYの分割数
	constexpr int key_role_division_y = 9;

	//キーの役割りのUI配置ポジション折り返しXの分割数
	constexpr int key_role_division_turn_back_x = 48;

	//変更のUI配置ポジションYの分割数
	constexpr int change_division_y = 57;

	//キャンセルのUI配置ポジションYの分割数
	constexpr int cancel_division_y = 60;

	//キーを変更する際に出てくるポップアップテキストの背景の配置ポジション左端Xの分割数
	constexpr int pop_up_background_division_left_x = 21;

	//キーを変更する際に出てくるポップアップテキストの背景の配置ポジション上Yの分割数
	constexpr int pop_up_background_division_up_y = 14;

	//キーを変更する際に出てくるポップアップテキストの背景の配置ポジション右端Xの分割数
	constexpr int pop_up_background_division_right_x = 106;

	//キーを変更する際に出てくるポップアップテキストの背景の配置ポジション下Yの分割数
	constexpr int pop_up_background_division_down_y = 57;

	//画像分割数X
	constexpr int division_graph_x = 9;

	//画像分割数Y
	constexpr int division_graph_y = 13;

	//画像分割総数
	constexpr int division_graph_total = division_graph_x * division_graph_y;

	//key画像の一枚当たりのサイズ
	constexpr int graph_chip_size = 59;

	//選択された画像のアルファ値
	constexpr float selection_alpha_value = 255;

	//選択された画像のアルファ値
	constexpr float not_selection_alpha_value = 150;

	//画像の中心座標X
	constexpr float graph_center_x = 0.5f;

	//画像の中心座標X
	constexpr float graph_center_y = 0.5f;

	//画像の隙間サイズ
	constexpr float graph_gap_size = 5.5f;

	//画像の拡縮率
	constexpr float graph_scale_size = 5.0f;

	//画像の拡縮率
	constexpr float graph_expansion_scale_size = 5.2f;

	//文字の拡縮率
	constexpr float str_scale_size = 2.5f;

	//ビルボートのサイズ
	constexpr float bill_board_size = 50.0f;

	//コントローラー画像ののサイズ
	constexpr float controller_graph_size = 90.0f;
}

//コンストラクタ
KeyConfigSceneForSceneTitle::KeyConfigSceneForSceneTitle(SceneManager& manager) :SceneBase(manager),updateFunc_(&KeyConfigSceneForSceneTitle::FadeInUpdate)
{
	//短縮化
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

//デストラクタ
KeyConfigSceneForSceneTitle::~KeyConfigSceneForSceneTitle()
{
}

//初期化
void KeyConfigSceneForSceneTitle::Init()
{
	//キーグラフを分割して読み込む
	LoadDivGraph("data/graph/key.png", division_graph_total,division_graph_x, division_graph_y,graph_chip_size, graph_chip_size,keyTypeHandle_);

	//フォントの作成
	fontHandleSize21_ = FontsManager::GetInstance().GetFontHandle("ピグモ 0021");
	fontHandleSize42_ = FontsManager::GetInstance().GetFontHandle("ピグモ 0042");

	//インスタンス化
	KeyUI_ = std::make_shared<UIItemManager>();
	PadUI_ = std::make_shared<UIItemManager>();

	//短縮化
	auto& input = InputState::GetInstance();
	auto& file = ExternalFile::GetInstance();

	//キーの役割の一つ目の座標
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

	keyDrawPos_["変更"] = file.GetUIPos("advancedSettingUIPos");
	keyDrawPos_["キャンセル"] = file.GetUIPos("backUIPos");

	padDrawPos_["戻る"] = file.GetUIPos("backUIPos");

	int nameNo = 0;
	for (const auto& table : input.inputNameTable_)
	{
		//メニューの追加
		KeyUI_->AddMenu(namePosX, namePosY, UI_pos_x, UI_pos_y, table.second.c_str(), fontHandleSize21_);
	
		//ポジション調整
		namePosY += graph_chip_size + graph_gap_size;
	
		//inputstate.tempmaptableの半分を超えたら折り返す
		if (nameNo == inputSize / half) 
		{
			namePosX += static_cast<float>(Game::screen_width / screen_division_width * key_role_division_turn_back_x);
			namePosY =  static_cast<float>(Game::screen_height / screen_division_height * key_role_division_y);
		}
		nameNo++;
	}

	//メニューの追加
	KeyUI_->AddMenu(static_cast<float>(Game::screen_width / half), static_cast<float>(Game::screen_height / screen_division_height * change_division_y), UI_pos_x, UI_pos_y, "変更", fontHandleSize21_);
	KeyUI_->AddMenu(static_cast<float>(Game::screen_width / half), static_cast<float>(Game::screen_height / screen_division_height * cancel_division_y), UI_pos_x, UI_pos_y, "キャンセル", fontHandleSize21_);
	PadUI_->AddMenu(static_cast<float>(Game::screen_width / half), static_cast<float>(Game::screen_height / screen_division_height * cancel_division_y), UI_pos_x, UI_pos_y, "戻る", fontHandleSize21_);
}

//終了
void KeyConfigSceneForSceneTitle::End()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//現在のキー入力情報を外部データとして書き出す
	input.SavekeyInfo();

	for (const auto& graph : keyTypeHandle_)
	{
		DeleteGraph(graph);
	}
}

//メンバ関数ポインタの更新
void KeyConfigSceneForSceneTitle::Update()
{
	(this->*updateFunc_)();
}

//描画
void KeyConfigSceneForSceneTitle::Draw()
{
	(this->*drawFunc_)();
}

//選択操作
void KeyConfigSceneForSceneTitle::SelectionOperation()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//現在のキーの数を取得する
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

//キーの役割りの描画
void KeyConfigSceneForSceneTitle::KeyStateDraw()
{
	KeyUI_->AlphaChangeDrawBillBoard(keyDrawPos_, selectNum_, fadeValue_, bill_board_size);

	//キーに対応する画像を描画する
	KeyGraphDraw();
}

//keyの画像を描画
void KeyConfigSceneForSceneTitle::KeyGraphDraw()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//dxlib内の各キー番号を入手する
	int keyNum = 0;

	//アルファ値
	float alphaValue = 0;

	//画像の拡縮率
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

		//key番号を取得する
		keyNum = static_cast<int>(keyNum_[key.second.begin()->id]);

		//現在カーソルがあっている場合
		//サイズと明るさを引く値を変更する
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

//コントローラーが使用されている場合の描画
void KeyConfigSceneForSceneTitle::ControllerDraw()
{
	//UI配置ポジション
	VECTOR pos = ExternalFile::GetInstance().GetUIPos("controllerUIPos");

	//コントローラー画像の描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBillboard3D(pos, graph_center_x, graph_center_y, controller_graph_size, 0.0f, GraphManager::GetInstance().GetGraph("controller"), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//UIの描画
	PadUI_->AlphaChangeDrawBillBoard(padDrawPos_, selectNum_, fadeValue_, bill_board_size);
}

//変更したいkeyを入力させるためのポップアップ描画
void KeyConfigSceneForSceneTitle::ChangeKeyPopUpText()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//背景の黒描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, pop_up_text_max_alpha_blend_value);
	DrawBox(Game::screen_width / screen_division_width * pop_up_background_division_left_x, Game::screen_height / screen_division_height * pop_up_background_division_up_y,
			Game::screen_width / screen_division_width * pop_up_background_division_right_x, Game::screen_height / screen_division_height * pop_up_background_division_down_y,
			0x7d7d7d, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//文字列
	std::string text = input.inputNameTable_.find(static_cast<InputType>(selectNum_))->second.c_str();

	//文字列の横幅
	int strWidth = GetDrawStringWidthToHandle(text.c_str(), static_cast<int>(text.size()), fontHandleSize21_);

	//選択したキーの名前を出力
	DrawStringToHandle(Game::screen_width / screen_division_width * pop_up_background_division_left_x + strWidth / half,
					   Game::screen_height / screen_division_height * cange_pop_up_graph_screen_division_y,
					   text.c_str(), 0xffffff, fontHandleSize21_);

	//キー画像の描画
	input.DrawKeyGraph(selectNum_,
					   static_cast<float>(Game::screen_width / screen_division_width * pop_up_background_division_left_x + strWidth * str_scale_size),
					   static_cast<float>(Game::screen_height / screen_division_height * pop_up_background_division_up_y - graph_chip_size / half),
					   1.0f);

	//文字列
	text = "変更したいキーを入力してください";
	//文字列の横幅
	strWidth = GetDrawStringWidthToHandle(text.c_str(), static_cast<int>(text.size()), fontHandleSize42_);
	//文字列の描画
	DrawStringToHandle(Game::screen_width / half - strWidth / half, Game::screen_height / half, text.c_str(), 0xffffff, fontHandleSize42_);

}

//変更したいキーを選択する
void KeyConfigSceneForSceneTitle::SelectChangeKeyUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//選択操作
	SelectionOperation();

	//キーの変更を保存する
	if (selectNum_ == input.inputNameTable_.size())
	{
		if (input.IsTriggered(InputType::Space))
		{
			//仮のキー情報を実際に参照しているキー情報に代入する
			input.CommitChangedInputInfo();

			//シーンをポップアップテキストを描画するシーンに変更する
			manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<PopUpTextScene>(manager_)));
			return;
		}
	}

	//仮キー情報を消去してポーズシーンに戻る
	if (selectNum_ == input.inputNameTable_.size() + 1) 
	{
		if (input.IsTriggered(InputType::Space)) 
		{
			input.ResetInputInfo();
			updateFunc_ = &KeyConfigSceneForSceneTitle::FadeOutUpdate;
			return;
		}
	}

	//どのキーを変更するかを仮決定
	if (input.IsTriggered(InputType::Space)) 
	{
		isEditing_ = !isEditing_;
		drawFunc_ = &KeyConfigSceneForSceneTitle::ChangeKeyPopUpText;
		changeKeyUpdateFunc_ = &KeyConfigSceneForSceneTitle::ChangeKeyborardUpdate;
		return;
	}

	//ひとつ前のシーンに戻る
	if (input.IsTriggered(InputType::Pause))
	{
		updateFunc_ = &KeyConfigSceneForSceneTitle::FadeOutUpdate;
		input.RollbackChangedInputInfo();
		return;
	}

	//コントローラーの入力が入ったときに
	//コントローラー用の描画と更新用クラスに変更する
	if (!input.LastInputDevice()) 
	{
		changeKeyUpdateFunc_ = &KeyConfigSceneForSceneTitle::ControllerUpdate;
		drawFunc_ = &KeyConfigSceneForSceneTitle::ControllerDraw;
	}
}

//変更したいキーをどのキーに変更するのか
void KeyConfigSceneForSceneTitle::ChangeKeyborardUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//一フレーム前の入力情報が残っていたらリターンする
	for (const auto& key : input.lastInput_) 
	{
		if (key == true) return;
	}

	//メンバ関数ポインタを変更するキーを選択する関数に変更する
	if (input.IsTriggered(InputType::Pause)) 
	{
		isEditing_ = !isEditing_;
		input.UndoSelectKey(static_cast<InputType>(selectNum_), InputCategory::keybd);
		drawFunc_ = &KeyConfigSceneForSceneTitle::KeyStateDraw;
		changeKeyUpdateFunc_ = &KeyConfigSceneForSceneTitle::SelectChangeKeyUpdate;
		return;
	}

	//キーボードとパッドの入力を得る
	char keyState[max_key_num];
	auto padState = GetJoypadInputState(DX_INPUT_PAD1);
	GetHitKeyStateAll(keyState);

	int idx = 0;
	InputType currentType = InputType::Max;

	//現在の選択inputNameTableを取得する
	for (const auto& name : input.inputNameTable_)
	{
		if (selectNum_ == idx) 
		{
			currentType = name.first;
			break;
		}
		idx++;
	}

	//キーボードの入力を変更する
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
	//パッドの入力を変更する
	if (padState != 0)
	{
		isEditing_ = !isEditing_;
		input.RewriteInputInfo(currentType, InputCategory::pad, padState);
		drawFunc_ = &KeyConfigSceneForSceneTitle::KeyStateDraw;
		changeKeyUpdateFunc_ = &KeyConfigSceneForSceneTitle::SelectChangeKeyUpdate;
	}
}

//コントローラーの場合の更新
void KeyConfigSceneForSceneTitle::ControllerUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

	selectNum_ = 0;

	//仮キー情報を消去してポーズシーンに戻る
	if (input.IsTriggered(InputType::Space))
	{
		input.ResetInputInfo();
		updateFunc_ = &KeyConfigSceneForSceneTitle::FadeOutUpdate;
		return;
	}

	//ひとつ前のシーンに戻る
	if (input.IsTriggered(InputType::Pause)) 
	{
		updateFunc_ = &KeyConfigSceneForSceneTitle::FadeOutUpdate;
		input.RollbackChangedInputInfo();
		return;
	}

	//キーボードの入力が入ったときに
	//キーボード用の描画と更新用クラスに変更する
	if (input.LastInputDevice())
	{
		drawFunc_ = &KeyConfigSceneForSceneTitle::KeyStateDraw;
		changeKeyUpdateFunc_ = &KeyConfigSceneForSceneTitle::SelectChangeKeyUpdate;
	}
}

//フェードインの更新
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

//通常時の更新
void KeyConfigSceneForSceneTitle::NormalUpdate()
{
	(this->*changeKeyUpdateFunc_)();
}

//フェードアウトの更新
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