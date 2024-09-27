#include "PopUpTextScene.h"
#include "SceneManager.h"
#include "ScenePause.h"

#include "../util/game.h"
#include "../util/InputState.h"

#include "../util/FontsManager.h"

namespace
{
	//半分
	constexpr int half = 2;

	//背景のアルファ値
	constexpr int background_alpha_value = 150;
}

//コンストラクタ
PopUpTextScene::PopUpTextScene(SceneManager& manager):SceneBase(manager)
{
}

//デストラクタ
PopUpTextScene::~PopUpTextScene()
{
}

//初期化
void PopUpTextScene::Init()
{
}

//終了
void PopUpTextScene::End()
{
}

//更新
void PopUpTextScene::Update()
{
	//短縮化
	auto& input = InputState::GetInstance();

	if (input.IsTriggered(InputType::Space))
	{
		manager_.PopFrontScene();
	}
}

//描画
void PopUpTextScene::Draw()
{
	//短縮化
	auto& font = FontsManager::GetInstance();

	//背景の描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, background_alpha_value);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//文字列の描画
	std::string str = "変更しました";

	int width = GetDrawStringWidthToHandle(str.c_str(), static_cast<int>(str.size()), font.GetFontHandle("ピグモ 0042"));

	DrawStringToHandle(Game::screen_width / half - width / half, Game::screen_height / half, str.c_str(), 0xffffff, font.GetFontHandle("ピグモ 0042"));
}
