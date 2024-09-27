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
	//半分
	constexpr int half = 2;

	//画面の横幅の分割数
	constexpr int screen_width_division_num = 128;

	//画面の縦の分割数
	constexpr int screen_height_division_num = 72;

	//UI配置ポジションX
	constexpr int UI_pos_x = 320;

	//UI配置ポジションY
	constexpr int UI_pos_y = 100;

	//音の最小値
	constexpr int min_sound_value = 0;
	
	//音の最大値
	constexpr int max_sound_value = 9;

	//音の1メモリの大きさ
	constexpr int sound_1_momery_size = 25;

	//フェードの最大値
	constexpr int max_fade_value = 255;

	//ガウス処理で使用するピクセル幅
	constexpr int pixel_width = 32;

	//ブレンドモードのアルファ値
	constexpr int blend_mode_alpha_value = 150;

	//操作設定と戻るの分割数
	constexpr int operation_setting_and_back_width_division_num = 64;

	//モード、SE、BGMの分割数
	constexpr int mode_SE_BGM_width_division_num = 32;

	//モードの高さの分割数
	constexpr int mode_height_division_num = 24;
	 
	//SEの高さの分割数
	constexpr int SE_height_division_num = 34;

	//BGMの高さの分割数
	constexpr int BGM_height_division_num = 46;

	//操作設定の高さの分割数
	constexpr int operation_height_division_num = 54;
	
	//戻るの高さの分割数
	constexpr int back_height_division_num = 58;

	//円の横幅の分割数
	constexpr int circle_width_division_num = 34;

	//円の数
	constexpr int circle_num = 10;

	//BGMの高さの分割数
	constexpr int circle_height_division_num = 48;

	//円の頂点の数
	constexpr int circle_vertex_num = 32;

	//選択されている項目のアルファ値
	constexpr int selected_item_alpha_value = 250;

	//選択されていない項目のアルファ値
	constexpr int not_selected_item_alpha_value = 150;

	//円同士の間隔
	constexpr float circle_distance = 53.5f;

	//円の半径
	constexpr float circle_radius = 14.5f;

	//円の拡縮率
	constexpr float circle_expansion_size = 1.3f;
}

//コンストラクタ
SettingScene::SettingScene(SceneManager& manager):SceneBase(manager),updateFunc_(&SettingScene::GaussFadeInUpdate)
{
}

//デストラクタ
SettingScene::~SettingScene()
{
}

//初期化
void SettingScene::Init()
{
	//短縮化
	auto& sound = SoundManager::GetInstance();
	auto& font = FontsManager::GetInstance();

	//インスタンス化
	UIManager_ = std::make_shared<UIItemManager>();

	//現在のボリュームの取得
	volumeBGM_ = sound.GetBGMVolume() / sound_1_momery_size;
	volumeSE_ = sound.GetSEVolume() / sound_1_momery_size;

	//UI画像の作成
	//フォントの取得
	int pigumo42Font = font.GetFontHandle("ピグモ 0042");

	//フォントを適用した文字列のサイズ取得
	int windowFontSize	= font.GetStringSize("モード", "ピグモ 0042");
	int BGMFontSize		= font.GetStringSize("BGM", "ピグモ 0042");
	int SEFontSize		= font.GetStringSize("SE", "ピグモ 0042");

	//UI画像の作成
	UIManager_->AddMenu(Game::screen_width / screen_width_division_num * mode_SE_BGM_width_division_num, Game::screen_height / screen_height_division_num * mode_height_division_num					, UI_pos_x, UI_pos_y, "モード", pigumo42Font);
	UIManager_->AddMenu(Game::screen_width / screen_width_division_num * mode_SE_BGM_width_division_num, Game::screen_height / screen_height_division_num * SE_height_division_num						, UI_pos_x, UI_pos_y, "BGM", pigumo42Font);
	UIManager_->AddMenu(Game::screen_width / screen_width_division_num * mode_SE_BGM_width_division_num, Game::screen_height / screen_height_division_num * BGM_height_division_num						, UI_pos_x, UI_pos_y, "SE", pigumo42Font);
	UIManager_->AddMenu(Game::screen_width / screen_width_division_num * operation_setting_and_back_width_division_num, Game::screen_height / screen_height_division_num * operation_height_division_num, UI_pos_x, UI_pos_y, "操作設定", pigumo42Font);
	UIManager_->AddMenu(Game::screen_width / screen_width_division_num * operation_setting_and_back_width_division_num, Game::screen_height / screen_height_division_num * back_height_division_num		, UI_pos_x, UI_pos_y, "戻る", pigumo42Font);

	//スクリーン画像の作成
	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);

	if (manager_.GetWindowMode()) 
	{
		windowModeText_ = "≪  ウィンドウモード  ≫";
	}
	else {
		windowModeText_ = "≪  フルスクリーン  ≫";
	}
}

//終了
void SettingScene::End()
{
	DeleteGraph(makeScreenHandle_);
}

//更新
void SettingScene::Update()
{
	(this->*updateFunc_)();
}

//描画
void SettingScene::Draw()
{
	//少し透過した黒の背景を描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blend_mode_alpha_value);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//作成した画面サイズの画像に
	//以降の内容を書き込む
	SetDrawScreen(makeScreenHandle_);
	ClearDrawScreen();

	//UI文字列の描画
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

	//ウィンドウモードの描画
	DrawWindowMode(alpha[static_cast<int>(UpdateItem::WindowMode)]);

	//BGM調整バーの描画
	DrawBGMBar(alpha[static_cast<int>(UpdateItem::BGM)]);

	//SE調整バーの描画
	DrawSEBar(alpha[static_cast<int>(UpdateItem::SE)]);

	//通常に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	//フェードの時、アルファ値を変更するとともにガウスぼかしを掛ける
	GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, pixel_width, max_fade_value - fadeValue_);

	//描画画像のアルファ値を変更する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawGraph(0, 0, makeScreenHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//ウィンドウモードの描画
void SettingScene::DrawWindowMode(int alphaValue)
{
	//短縮化
	auto& font = FontsManager::GetInstance();

	//現在の画面モードを表示
	int pigumo42 = font.GetFontHandle("ピグモ 0042");

	//フォントを適用した文字列の横幅のサイズを取得
	int windowModeFontSize = font.GetStringSize(windowModeText_.c_str(), "ピグモ 0042");

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaValue);
	DrawStringToHandle(Game::screen_width / half - windowModeFontSize / half, Game::screen_height / screen_height_division_num * mode_height_division_num, windowModeText_.c_str(), 0xffffff, pigumo42);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//SE調整バーの描画
void SettingScene::DrawSEBar(int alphaValue)
{
	//円を描画する位置
	float circlePosX = Game::screen_width / screen_width_division_num * circle_width_division_num;

	//拡縮率
	float scale = 1.0f;

	//色
	int color = 0xb1b3b6;

	//SEバー
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaValue);

	//音量バー画像の描画
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

//BGM調整バーの描画
void SettingScene::DrawBGMBar(int alphaValue)
{
	//円を描画する位置
	float circlePosX = Game::screen_width / screen_width_division_num * circle_width_division_num;

	//拡縮率
	float scale = 1.0f;

	//色
	int color = 0xb1b3b6;

	//BGMバー
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaValue);

	//音量バー画像の描画
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

//通常時の更新
void SettingScene::NormalUpdate()
{
	//短縮化
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

	//シーン切り替え
	if (input.IsTriggered(InputType::Pause)) 
	{
		nextScene_ = std::make_shared<ScenePause>(manager_);
		updateFunc_ = &SettingScene::GaussFadeOutUpdate;
	}
}

//ガウス処理を施したフェードイン
void SettingScene::GaussFadeInUpdate()
{
	fadeValue_ = static_cast <int>(max_fade_value * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_)
	{
		updateFunc_ = &SettingScene::NormalUpdate;
		fadeValue_ = max_fade_value;
	}
}

//ガウス処理を施したフェードアウト
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

//BGMの音量を変更する
void SettingScene::BGMUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();
	auto& sound = SoundManager::GetInstance();

	//BGM音量調整
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

	//音量の変更
	SoundManager::GetInstance().SetBGMVolume(volumeBGM_ * sound_1_momery_size);
}

//SEの音量を変更する
void SettingScene::SEUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();
	auto& sound = SoundManager::GetInstance();

	//SE音量調整
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

	//音量の変更
	SoundManager::GetInstance().SetSEVolume(volumeSE_ * sound_1_momery_size);
}

//ウィンドウモードを変更する
void SettingScene::ChangeWindowUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

	if (input.IsTriggered(InputType::Left))
	{
		windowModeText_ = "≪  ウィンドウモード  ≫";
		manager_.SetChangeWindowMode(true);
	}
	if (input.IsTriggered(InputType::Right))
	{
		windowModeText_ = "≪  フルスクリーン  ≫";
		manager_.SetChangeWindowMode(false);
	}
}

//updateFuncの中身を変更する
void SettingScene::ChangeUpdateFunc()
{
	//短縮化
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
