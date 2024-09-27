#include "ScenePause.h"
#include "SceneTitle.h"
#include "DebugScene.h"
#include "SceneManager.h"
#include "SettingScene.h"
#include "KeyConfigScene.h"

#include "../util/Util.h"
#include "../util/game.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/FontsManager.h"
#include "../util/GraphManager.h"
#include "../util/UIItemManager.h"

#include <algorithm>

namespace
{
	//半分
	constexpr int half = 2;

	//画面の高さの三分割する
	constexpr int three_part = 3;

	//UI配置ポジションX
	constexpr int UI_pos_x = 640;

	//UI配置ポジションY
	constexpr int UI_pos_y = 200;

	//ブレンドモードのアルファ値
	constexpr int blend_alpha_value = 150;

	//UImenuのアルファ値
	constexpr int UI_menu_alpha_value = 255;

	//タイトルの画像のサイズ
	constexpr float title_graph_size = 1.0f;

	//タイトル画像の角度
	constexpr float title_angle = 0.0f;

	//UImenuのY座標の初期位置
	constexpr float UI_menu_init_pos_y = 100.0f;

	//UImenuの隙間サイズ
	constexpr float UI_menu_gap_size = 40.0f;
}

//コンストラクタ
ScenePause::ScenePause(SceneManager& manager):SceneBase(manager)
{
}

//デストラクタ
ScenePause::~ScenePause()
{
}

//初期化
void ScenePause::Init()
{
	//インスタンス化
	UI_ = std::make_shared<UIItemManager>();

	//UI文字列の作成
	menuName_.push_back("戻る");
	menuName_.push_back("設定");
	menuName_.push_back("タイトルへ");
#ifdef _DEBUG
	menuName_.push_back("デバッグシーンへ");
#endif // _DEBUG

	//UI画像の作成
	int font = FontsManager::GetInstance().GetFontHandle("ピグモ 0042");
	float y = UI_menu_init_pos_y;
#ifdef _DEBUG
#else
	y += 40;
#endif // _DEBUG

	for (const auto& menu : menuName_)
	{
		UI_->AddMenu(static_cast<float>(Game::screen_width / half), static_cast<float>(Game::screen_height / half + y), UI_pos_x, UI_pos_y, menu.c_str(), font);
		y += UI_menu_gap_size;
	}
}

//終了
void ScenePause::End()
{
}

//更新
void ScenePause::Update()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//項目選択
	if (input.IsTriggered(InputType::Up)) 
	{
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}
	if (input.IsTriggered(InputType::Down)) 
	{
		selectNum_ = (std::min)(selectNum_ + 1, static_cast<int>(menuName_.size()) - 1);
	}
	
	if (input.IsTriggered(InputType::Space))
	{
		 switch(selectNum_) 
		 {
		//一個前のシーン(メインシーン)へ遷移
		case 0:
			manager_.PopFrontScene();
			break;
		//サウンドセッティングへの遷移
		 case 1:
			 manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<SettingScene>(manager_)));
			 break;
		//タイトルシーンへの遷移
		case 2:
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
			break;
		//デバッグシーンに戻る
		case 3:
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<DebugScene>(manager_)));
			break;
		 }
	}

	//一個前のシーン(メインシーン)へ遷移
	if (input.IsTriggered(InputType::Pause))
	{
		manager_.PopFrontScene();
	}
}

//描画
void ScenePause::Draw()
{
	//背景黒の描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blend_alpha_value);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

	//タイトル画像の描画
	DrawRotaGraph(Game::screen_width / half, Game::screen_height / three_part, title_graph_size, title_angle, GraphManager::GetInstance().GetGraph("title"), true);

	//UIの描画
	UI_->AlphaChangeDraw(selectNum_, UI_menu_alpha_value);
}
