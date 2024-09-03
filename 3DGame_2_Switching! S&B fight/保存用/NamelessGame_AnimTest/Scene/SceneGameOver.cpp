#include "SceneGameOver.h"
#include "SceneTitle.h"
#include "../Pad.h"

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

	Pad::Update();

	if (Pad::IsTrigger(PAD_INPUT_1))		// Zキー(PAD::A)を押したら
	{
		return make_shared<SceneTitle>();	// タイトルシーンへ行く
	}

	return shared_from_this();
}

void SceneGameOver::Draw()
{
	DrawString(0, 0, "SceneGameOver", 0xffffff);
}

void SceneGameOver::End()
{
}
