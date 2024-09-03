#include "SceneOption.h"
#include "SceneTitle.h"
#include "../Pad.h"

SceneOption::SceneOption()
{
}

SceneOption::~SceneOption()
{
}

void SceneOption::Init()
{
}

shared_ptr<SceneBase> SceneOption::Update()
{
	Pad::Update();




	if (Pad::IsTrigger(PAD_INPUT_1))		// Zキー(PAD::A)を押したら
	{
		return make_shared<SceneTitle>();	// タイトルシーンへ行く
	}

	return shared_from_this();
}

void SceneOption::Draw()
{
	DrawString(0, 0, "SceneOption", 0xffffff);
}

void SceneOption::End()
{
}
