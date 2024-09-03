#include "SceneGameClear.h"
#include "SceneTitle.h"
#include "../Util/Pad.h"

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
	if (Pad::IsTrigger(PAD_INPUT_R))		// RBボタン
	{
		return make_shared<SceneTitle>();	// タイトルシーンへ行く
	}


#ifdef _DEBUG
#endif // DEBUG

	return shared_from_this();
}

void SceneGameClear::Draw()
{
	DrawString(0, 0, "SceneGameClear", 0xffffff);
	DrawString(0, 20, "Please Press Button RB", 0x00ffff);
}

void SceneGameClear::End()
{
}
