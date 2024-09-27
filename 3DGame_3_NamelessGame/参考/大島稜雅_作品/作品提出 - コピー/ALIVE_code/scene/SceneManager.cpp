#include "SceneManager.h"
#include "SceneBase.h"
#include "DxLib.h"
#include "util/game.h"

namespace
{
	//updateBarを描画する上の座標の補正値
	constexpr int update_bar_up_correction = 16;

	//boxを作る描画する上の座標の補正値
	constexpr int draw_box_up_correction = 48;

	//boxを作る描画する下の座標の補正値
	constexpr int draw_box_down_correction = 32;

	//UIUpdateBox表示位置X
	constexpr int UI_update_box_draw_pos_x = 0;

	//UIDrawBox表示位置X
	constexpr int UI_draw_box_draw_pos_x = 64;
	
	//UIboxサイズ
	constexpr int draw_box_size = 16;

	//デバッグUIの
}

//シーン遷移
void SceneManager::ChangeScene(const std::shared_ptr<SceneBase>& scene)
{
	//今あるシーンをすべて削除する
	while (!scenes_.empty())
	{
		//先頭のシーンを削除する
		scenes_.front()->End();
		scenes_.pop_front();
	}
	//先頭に引数で得たシーンを追加する
	scenes_.push_front(std::shared_ptr<SceneBase>(scene));
	scenes_.front()->Init();
}

//メインのシーンを保ったまま別のシーンを前画面に表示する
void SceneManager::PushFrontScene(const std::shared_ptr<SceneBase>& scene)
{
	//先頭に引数で得たシーンを追加する
	scenes_.push_front(scene);
	scenes_.front()->Init();
}

//シーンをすり替える
void SceneManager::SwapScene(const std::shared_ptr<SceneBase>& scene)
{
	//先頭のシーンを削除する
	while(scenes_.size() > 1) {
		scenes_.front()->End();
		scenes_.pop_front();
	}
	//先頭に引数で得たシーンを追加する
	scenes_.push_front(std::shared_ptr<SceneBase>(scene));
	scenes_.front()->Init();
}

//ひとつ前のシーンに戻る
void SceneManager::PopFrontScene()
{
	//先頭のシーンを削除する
	if (scenes_.size() > 1) {
		scenes_.front()->End();
		scenes_.pop_front();
	}
}

//更新
void SceneManager::Update()
{
	LONGLONG start = GetNowHiPerformanceCount();
	
	//先頭にあるシーンの更新
	scenes_.front()->Update();

	debugUpdateTime_ = static_cast<int>(GetNowHiPerformanceCount() - start);
}

//描画
void SceneManager::Draw()
{

	LONGLONG start = GetNowHiPerformanceCount();

	//シーンすべての描画
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

//ウィンドウモードを変更する
void SceneManager::SetChangeWindowMode(const bool windowMode)
{
	windowMode_ = windowMode;
	DxLib::ChangeWindowMode(windowMode_);
}
