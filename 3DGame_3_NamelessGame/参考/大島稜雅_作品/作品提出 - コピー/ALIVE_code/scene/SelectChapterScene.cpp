#include "SelectChapterScene.h"
#include "GameMain.h"
#include "SceneManager.h"

#include "../object/ObjectData.h"

#include "../util/game.h"
#include "../util/Model.h"
#include "../UTIL/Easing.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/ModelManager.h"
#include "../util/GraphManager.h"

#include <string>
#include <algorithm>

namespace {
	//アニメーションを変更するのにかかる時間
	constexpr int anim_change_time = 10;

	//テクスチャの番号
	constexpr int texture_num = 0;

	//最大チャプター数
	constexpr int max_chapter_num = 2;

	//最小チャプター数
	constexpr int min_chapter_num = 0;

	//フェードの最大値
	constexpr int max_fade_value = 255;

	//総時間
	constexpr float total_time = 60.0f;
}

SelectChapterScene::SelectChapterScene(SceneManager& manager) : SceneBase(manager), updateFunc_(&SelectChapterScene::NormalUpdate)
{
}

//コンストラクタ
SelectChapterScene::SelectChapterScene(SceneManager& manager, std::shared_ptr<Model> modelPointer) : SceneBase(manager),updateFunc_(&SelectChapterScene::NormalUpdate)
{
	//モデルのポインタを設定する
	model_ = modelPointer;

	//モデルのアニメーションを設定する
	model_->SetAnimation(static_cast<int>(BookAnim::idle), false, false);
}

//デストラクタ
SelectChapterScene::~SelectChapterScene()
{
}

//初期化
void SelectChapterScene::Init()
{
}

//終了
void SelectChapterScene::End()
{
}

//更新
void SelectChapterScene::Update()
{
	(this->*updateFunc_)();
}

//描画
void SelectChapterScene::Draw()
{
	//モデルの描画
	model_->Draw();

	//画面全体を真っ黒に塗りつぶす
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//チャプターを変更する
void SelectChapterScene::ChangeChapter()
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//選ばれているチャプターの文字列を取得する
	std::string str = "Chapter" + std::to_string(selectNum_);

	//プレイヤーの開始位置を設定する
	file.SetStartName(str);
}

//通常の更新
void SelectChapterScene::NormalUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();
	auto& graph = GraphManager::GetInstance();

	//モデルの更新
	model_->Update();

	if (model_->IsAnimEnd()) 
	{
		model_->ChangeAnimation(static_cast<int>(BookAnim::idle), false, false, anim_change_time);
	}
	else 
	{
		return;
	}

	//チャプター選択
	if (input.IsTriggered(InputType::Right)) 
	{
		if (selectNum_ < max_chapter_num)
		{
			model_->ChangeAnimation(static_cast<int>(BookAnim::open), false, false, anim_change_time);
		}
		selectNum_ = (std::min)(selectNum_ + 1, max_chapter_num);
	}
	//チャプター選択
	if (input.IsTriggered(InputType::Left))
	{
		if (selectNum_ > 0) 
		{
			model_->ChangeAnimation(static_cast<int>(BookAnim::close), false, false, anim_change_time);
		}
		selectNum_ = (std::max)(selectNum_ - 1, min_chapter_num);
	}

	//画像名の文字列の取得
	std::string str = "Chapter" + std::to_string(selectNum_ + 1);

	//マテリアルのテクスチャを変更する
	MV1SetTextureGraphHandle(model_->GetModelHandle(), texture_num, graph.GetGraph(str), true);

	//戻る
	if (input.IsTriggered(InputType::Down) || input.IsTriggered(InputType::Activate))
	{
		manager_.PopFrontScene();
	}

	//決定
	if (input.IsTriggered(InputType::Space)) 
	{
		updateFunc_ = &SelectChapterScene::FadeOutUpdate;
	}
}

//フェードアウト
void SelectChapterScene::FadeOutUpdate()
{
	fadeValue_ = static_cast <int>(max_fade_value * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));

	if (++fadeTimer_ == fadeInterval_) 
	{
		ChangeChapter();
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
		return;
	}
}
