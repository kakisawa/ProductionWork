#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneOption.h"
#include "SceneRanking.h"
#include "../Pad.h"

SceneTitle::SceneTitle()
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
}

shared_ptr<SceneBase> SceneTitle::Update()
{
	Pad::Update();




	//if (Pad::IsTrigger(PAD_INPUT_1))		// Zキー(PAD::A)を押したら
	//{
	//	return make_shared<SceneGame>();	// ゲームシーンへ行く
	//	return make_shared<SceneOption>();	// オプション行く
	//	return make_shared<SceneRanking>();	// ランキングへ行く
	//}

#ifdef _DEBUG
	if (Pad::IsTrigger(PAD_INPUT_1))		// Zキー(PAD::A)を押したら
	{
		return make_shared<SceneGame>();	// ゲームシーンへ行く
	}
	if (Pad::IsTrigger(PAD_INPUT_2))		// Xキー(PAD::B)を押したら
	{
		return make_shared<SceneOption>();	// オプション行く
	}
	if (Pad::IsTrigger(PAD_INPUT_3))		// Cキー(PAD::X)を押したら
	{
		return make_shared<SceneRanking>();	// ランキングへ行く
	}
#endif // DEBUG


	return shared_from_this();
}

void SceneTitle::Draw()
{
	DrawString(0, 0, "SceneTitle", 0xffffff);
}

void SceneTitle::End()
{
}
