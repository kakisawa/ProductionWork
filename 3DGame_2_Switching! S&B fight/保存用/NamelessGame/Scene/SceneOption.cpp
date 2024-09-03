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
	if (m_isNextSceneFlag)
	{
		if (Pad::IsTrigger(PAD_INPUT_Z))		// RBボタン
		{
			return make_shared<SceneTitle>();	// タイトルシーンへ行く
		}
	}
	



#ifdef _DEBUG
	// デバッグ用?(フェード入れたら使わないかも)
	if (Pad::IsNotPress(PAD_INPUT_Z)) {	// RBボタン
		m_isNextSceneFlag = true;
	}
#endif // DEBUG

	return shared_from_this();
}

void SceneOption::Draw()
{
	DrawString(0, 0, "SceneOption", 0xffffff);
	DrawString(0, 20, "Please Press Button RB", 0x00ffff);
}

void SceneOption::End()
{
}
