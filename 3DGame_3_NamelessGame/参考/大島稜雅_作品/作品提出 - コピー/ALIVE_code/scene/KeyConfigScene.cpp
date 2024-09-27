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

	//ガウス処理で使用するピクセル幅
	constexpr int pixel_width = 32;

	//ぼかしのパラメーター
	constexpr int pixel_param = 800;

	//変更UIとキャンセルUIの数
	constexpr int change_and_cancel_UI_Num = 2;

	//背景のalphaBlendの最大値
	constexpr int background_max_alpha_blend_value = 180;

	//ポップアップテキストのalphaBlendの最大値
	constexpr int pop_up_text_max_alpha_blend_value = 100;

	//明るさを引く値
	constexpr int sub_brightness = 180;

	//UI配置ポジションX
	constexpr int UI_pos_x = 320;
	
	//UI配置ポジションY
	constexpr int UI_pos_y = 100;

	//画像UI配置ポジションXの分割数
	constexpr int graph_screen_division_x = 48;

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

	//key画像の一枚当たりのサイズ
	constexpr float graph_chip_size = 59.0f;

	//画像の隙間サイズ
	constexpr float graph_gap_size = 10.0f;

	//画像の拡縮率
	constexpr float graph_scale_size = 1.2f;

	//文字の拡縮率
	constexpr float str_scale_size = 2.5f;
}

//コンストラクタ
KeyConfigScene::KeyConfigScene(SceneManager& manager):SceneBase(manager),updateFunc_(&KeyConfigScene::FadeInUpdate)
{
	//短縮化
	auto& input = InputState::GetInstance();

	//最後に入力された装置によって更新、描画関数を変更する
	if (!input.LastInputDevice()) {
		changeKeyUpdateFunc_ = &KeyConfigScene::ControllerUpdate;
		drawFunc_ = &KeyConfigScene::ControllerDraw;
	}
	else {
		changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigScene::KeyStateDraw;
	}
}

//デストラクタ
KeyConfigScene::~KeyConfigScene()
{
}

//初期化
void KeyConfigScene::Init()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//インスタンス化
	KeyUI_ = std::make_shared<UIItemManager>();
	PadUI_ = std::make_shared<UIItemManager>();

	//スクリーンサイズのハンドルを作成
	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);

	//フォントの作成
	fontHandleSize21_ = FontsManager::GetInstance().GetFontHandle("ピグモ 0021");
	fontHandleSize42_ = FontsManager::GetInstance().GetFontHandle("ピグモ 0042");
	
	//キーの役割の一つ目の座標
	float namePosX = static_cast<float>(Game::screen_width / screen_division_width * key_role_division_x);
	float namePosY = static_cast<float>(Game::screen_height / screen_division_height * key_role_division_y);

	int nameNo = 0;
	for (const auto& table : input.inputNameTable_) {
		//メニューの追加
		KeyUI_->AddMenu(namePosX, namePosY, UI_pos_x, UI_pos_y, table.second.c_str(), fontHandleSize21_);

		//ポジション調整
		namePosY += graph_chip_size + graph_gap_size;

		//inputstate.tempmaptableの半分を超えたら折り返す
		if (nameNo == input.tempMapTable_.size() / half - 1)
		{
			namePosX += static_cast<float>(Game::screen_width / screen_division_width * key_role_division_turn_back_x);
			namePosY = static_cast<float>(Game::screen_height / screen_division_height * key_role_division_y);
		}

		nameNo++;
	}

	//メニューの追加
	KeyUI_->AddMenu(static_cast<float>(Game::screen_width / half), static_cast<float>(Game::screen_height / screen_division_height * change_division_y), UI_pos_x, UI_pos_y, "変更", fontHandleSize42_);
	KeyUI_->AddMenu(static_cast<float>(Game::screen_width / half), static_cast<float>(Game::screen_height / screen_division_height * cancel_division_y), UI_pos_x, UI_pos_y, "キャンセル", fontHandleSize42_);
	PadUI_->AddMenu(static_cast<float>(Game::screen_width / half), static_cast<float>(Game::screen_height / screen_division_height * cancel_division_y), UI_pos_x, UI_pos_y, "戻る", fontHandleSize42_);
}

//終了
void KeyConfigScene::End()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//現在のキー入力情報を外部データとして書き出す
	input.SavekeyInfo();
	DeleteGraph(makeScreenHandle_);
}

//更新
void KeyConfigScene::Update()
{
	(this->*updateFunc_)();
}

//描画
void KeyConfigScene::Draw()
{
	//背景に黒の透過
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, background_max_alpha_blend_value);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ガウス処理を施す
	GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, pixel_width, max_fade_value - fadeValue_);

	//makeScreenで作成したハンドルを描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawGraph(0, 0, makeScreenHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	(this->*drawFunc_)();
}

//選択操作
void KeyConfigScene::SelectionOperation()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//現在のキーの数を取得する
	const int keyNum = static_cast<int>(input.inputNameTable_.size() + change_and_cancel_UI_Num);

	//選択操作
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

//キーの役割りの描画
void KeyConfigScene::KeyStateDraw()
{
	//書き込みスクリーンの変更
	SetDrawScreen(makeScreenHandle_);

	//スクリーンのクリア
	ClearDrawScreen();

	//キーの役割を描画する
	KeyUI_->AlphaChangeDraw(selectNum_,fadeValue_);

	//キーに対応する画像を描画する
	KeyGraphDraw();

	//書き込みスクリーンを裏のスクリーンに変更する
	SetDrawScreen(DX_SCREEN_BACK);
}

//キーの画像を描画する
void KeyConfigScene::KeyGraphDraw()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//ポジション設定
	float graphPosX = static_cast<float>(Game::screen_width / screen_division_width * graph_screen_division_x);
	float graphPosY = static_cast<float>(Game::screen_height / screen_division_height * graph_screen_division_y);

	//for文で何番目かを取得する
	int keyCount = 0;

	//明るさを引く値
	int subBrightness = 0;

	//画像の拡縮率
	float graphScale = 1.0f;

	for (const auto& key : input.tempMapTable_) {
		
		if (key.first == InputType::Creative) {
			continue;
		}

		//現在カーソルがあっている場合
		//サイズと明るさを引く値を変更する
		if (keyCount == selectNum_) {
			graphScale = graph_scale_size;
			subBrightness = 0;
		}
		else {
			graphScale = 1.0f;
			subBrightness = sub_brightness;
		}

		//keyTypeの描画
		input.DrawKeyGraph(static_cast<int>(key.first), graphPosX, graphPosY, graphScale);

		//暗くした画像を画像の上に乗せる
		SetDrawBlendMode(DX_BLENDMODE_SUB, subBrightness);

		DrawBoxAA(graphPosX - graph_chip_size, graphPosY - graph_chip_size / half - graph_gap_size / half,
				  graphPosX + graph_chip_size , graphPosY + graph_chip_size,
				  fadeColor_, true);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//半分を超えたら折り返す処理
		graphPosY += graph_chip_size + graph_gap_size;
		if (keyCount == input.tempMapTable_.size() / half - 1)
		{
			graphPosY = static_cast<float>(Game::screen_height / screen_division_height * graph_screen_division_y);
			graphPosX += static_cast<float>(Game::screen_width / screen_division_width * graph_screen_division_x);
		}

		keyCount++;
	}
}

//コントローラー画像の描画
void KeyConfigScene::ControllerDraw()
{
	//書き込みスクリーンの変更
	SetDrawScreen(makeScreenHandle_);

	//スクリーンのクリア
	ClearDrawScreen();

	//UIの描画
	PadUI_->AlphaChangeDraw(selectNum_, fadeValue_);

	//操作説明画像の描画
	DrawGraph(0, 0, GraphManager::GetInstance().GetGraph("controller"), true);

	//書き込みスクリーンを裏のスクリーンに変更する
	SetDrawScreen(DX_SCREEN_BACK);
}

//変更したいkeyを入力させるためのポップアップ描画
void KeyConfigScene::ChangeKeyPopUpText()
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
	std::string text = input.inputNameTable_.find(static_cast<InputType>(selectNum_))->second;

	//文字列の横幅
	int strWidth = GetDrawStringWidthToHandle(text.c_str(), static_cast<int>(text.size()), fontHandleSize21_);

	//選択したキーの名前を出力
	DrawStringToHandle(Game::screen_width / screen_division_width * pop_up_background_division_left_x + strWidth / half ,
					   Game::screen_height / screen_division_height * cange_pop_up_graph_screen_division_y,
					   text.c_str(), 0xffffff, fontHandleSize21_);

	//int型にキャストしたgraphChipSize
	int castGraphChipSize = static_cast<int>(graph_chip_size);

	//キー画像の描画
	input.DrawKeyGraph( selectNum_, 
						static_cast<float>(Game::screen_width / screen_division_width * pop_up_background_division_left_x + strWidth * str_scale_size),
						static_cast<float>(Game::screen_height / screen_division_height * pop_up_background_division_up_y - graph_chip_size / half),
						1.0f);

	//文字列
	text = "変更したいキーを入力してください";
	//文字列の横幅
	strWidth = GetDrawStringWidthToHandle(text.c_str(), static_cast<int>(strlen(text.c_str())), fontHandleSize42_);
	//文字列の描画
	DrawStringToHandle(Game::screen_width / half - strWidth / half, Game::screen_height / half, text.c_str(), 0xffffff, fontHandleSize42_);

}

//どの入力装置のキーを変更するかを選択する
void KeyConfigScene::SelectChangeKeyUpdate()
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
			updateFunc_ = &KeyConfigScene::FadeOutUpdate;
			return;
		}
	}
	
	//どのキーを変更するかを仮決定
	if (input.IsTriggered(InputType::Space))
	{
		isEditing_ = !isEditing_;
		drawFunc_ = &KeyConfigScene::ChangeKeyPopUpText;
		changeKeyUpdateFunc_ = &KeyConfigScene::ChangeKeyborardUpdate;
		GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, pixel_width, pixel_param);
		return;
	}

	//ひとつ前のシーンに戻る
	if (input.IsTriggered(InputType::Pause)) 
	{
		updateFunc_ = &KeyConfigScene::FadeOutUpdate;
		input.RollbackChangedInputInfo();
		return;
	}

	//コントローラーの入力が入ったときに
	//コントローラー用の描画と更新用クラスに変更する
	if (!input.LastInputDevice()) 
	{
		changeKeyUpdateFunc_ = &KeyConfigScene::ControllerUpdate;
		drawFunc_ = &KeyConfigScene::ControllerDraw;
	}
}

//変更するキーをどのキーに変更するのかを決定する
void KeyConfigScene::ChangeKeyborardUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//キーボードとパッドの入力を得る
	char keyState[max_key_num];
	GetHitKeyStateAll(keyState);

	int idx = 0;
	InputType currentType = InputType::Max;

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
		drawFunc_ = &KeyConfigScene::KeyStateDraw;
		changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
		return;
	}

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
			drawFunc_ = &KeyConfigScene::KeyStateDraw;
			changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
			break;
		}
	}
}

//コントローラーの場合の更新
void KeyConfigScene::ControllerUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//選択肢がないため0にする
	selectNum_ = 0;

	//仮キー情報を消去してポーズシーンに戻る
	if (input.IsTriggered(InputType::Space))
	{
		input.ResetInputInfo();
		updateFunc_ = &KeyConfigScene::FadeOutUpdate;
		return;
	}

	//ひとつ前のシーンに戻る
	if (input.IsTriggered(InputType::Pause))
	{
		updateFunc_ = &KeyConfigScene::FadeOutUpdate;
		input.RollbackChangedInputInfo();
		return;
	}

	//キーボードの入力が入ったときに
	//キーボード用の描画と更新用クラスに変更する
	if (input.LastInputDevice()) 
	{
		changeKeyUpdateFunc_ = &KeyConfigScene::SelectChangeKeyUpdate;
		drawFunc_ = &KeyConfigScene::KeyStateDraw;
	}

}

//フェードインの更新
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

//通常時の更新
void KeyConfigScene::NormalUpdate()
{
	(this->*changeKeyUpdateFunc_)();
}

//フェードアウトの更新
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
