#include "SceneOption.h"
#include "SceneTitle.h"
#include "../Util/Pad.h"

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
	if (Pad::IsTrigger(PAD_INPUT_R))		// RB�{�^��
	{
		return make_shared<SceneTitle>();	// �^�C�g���V�[���֍s��
	}

#ifdef _DEBUG
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
