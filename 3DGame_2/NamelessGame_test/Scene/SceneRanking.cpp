#include "SceneRanking.h"
#include "SceneTitle.h"
#include "../Pad.h"

SceneRanking::SceneRanking()
{
}

SceneRanking::~SceneRanking()
{
}

void SceneRanking::Init()
{
}

shared_ptr<SceneBase> SceneRanking::Update()
{
	Pad::Update();

	if (Pad::IsTrigger(PAD_INPUT_1))		// Zキー(PAD::A)を押したら
	{
		return make_shared<SceneTitle>();	// タイトルシーンへ行く
	}

	return shared_from_this();
}

void SceneRanking::Draw()
{
	DrawString(0, 0, "SceneRanking", 0xffffff);
}

void SceneRanking::End()
{
}
