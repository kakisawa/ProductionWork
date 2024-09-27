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
	//文字列の補正配置場所Y
	constexpr int correction_pos_Y = 32;

	//文字列を表示するX座標
	constexpr int str_draw_pos_x = 100;

	//文字列を表示するY座標
	constexpr int str_draw_pos_y = 150;
}

//コンストラクタ
DebugScene::DebugScene(SceneManager& manager):SceneBase(manager)
{
}

//デストラクタ
DebugScene::~DebugScene()
{
}

//初期化
void DebugScene::Init()
{
	sceneName_.push_back("main");
	sceneName_.push_back("title");
	sceneName_.push_back("testScene");
	sceneName_.push_back("end");
	sceneName_.push_back("pause");
	sceneName_.push_back("exit");
}

//終了
void DebugScene::End()
{
}

//更新
void DebugScene::Update()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//選択
	if (input.IsTriggered(InputType::Up)) {
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}
	if (input.IsTriggered(InputType::Down)) {
		selectNum_ = (std::min)(selectNum_ + 1, static_cast<int>(sceneName_.size()) - 1);
	}

	//シーンの切り替え
	if (input.IsTriggered(InputType::Space)) {
		switch (selectNum_) {
		case 0:
			//ゲームプレイシーン
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
			break;
		case 1:
			//シーンタイトル
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
			break;
		case 2:
			//デバッグしたいシーン
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<KeyConfigScene>(manager_)));
			break;
		case 3:
			//終了画面
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameEnd>(manager_)));
			break;
		case 4:
			//シーンポーズ
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
			break;
		case 5:
			//終了
			manager_.SetEndFlag(true);
			break;
		}
	}
}

//描画
void DebugScene::Draw()
{
	int x = str_draw_pos_x;
	int y = str_draw_pos_y;

	//文字列の描画
	for (const auto& scene : sceneName_) {
		DrawFormatString(0, y, 0xffffff, "%s", scene.c_str());
		y += correction_pos_Y;
	}

	y = str_draw_pos_y;
	//選択番号によって変わる矢印の描画
	DrawString(x, selectNum_ * correction_pos_Y + y, "←", 0xff0000);
}
