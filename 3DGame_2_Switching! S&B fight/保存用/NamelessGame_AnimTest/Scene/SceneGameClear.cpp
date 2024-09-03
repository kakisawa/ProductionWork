#include "SceneGameClear.h"
#include "SceneTitle.h"
#include "../Pad.h"

SceneGameClear::SceneGameClear()
{
}

SceneGameClear::~SceneGameClear()
{
}

void SceneGameClear::Init()
{
}

shared_ptr<SceneBase> SceneGameClear::Update()
{
	Pad::Update();

	if (Pad::IsTrigger(PAD_INPUT_1))		// Zキー(PAD::A)を押したら
	{
		return make_shared<SceneTitle>();	// タイトルシーンへ行く
	}


	return shared_from_this();
}

void SceneGameClear::Draw()
{
	DrawString(0, 0, "SceneGameClear", 0xffffff);
}

void SceneGameClear::End()
{
}
