#include "SceneGameOver.h"
#include "SceneTitle.h"
#include "../Util/Pad.h"

SceneGameOver::SceneGameOver()
{
}

SceneGameOver::~SceneGameOver()
{
}

void SceneGameOver::Init()
{
}

shared_ptr<SceneBase> SceneGameOver::Update()
{
	if (Pad::IsTrigger(PAD_INPUT_R))		// RBボタン
	{
		return make_shared<SceneTitle>();	// タイトルシーンへ行く
	}


#ifdef _DEBUG
#endif // DEBUG

	return shared_from_this();
}

void SceneGameOver::Draw()
{
	DrawString(0, 0, "SceneGameOver", 0xffffff);
	DrawString(0, 20, "Please Press Button RB", 0x00ffff);
}

void SceneGameOver::End()
{
}
