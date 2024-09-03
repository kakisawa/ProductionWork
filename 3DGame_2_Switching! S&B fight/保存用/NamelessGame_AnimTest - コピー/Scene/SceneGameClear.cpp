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

	if (Pad::IsTrigger(PAD_INPUT_1))		// Z�L�[(PAD::A)����������
	{
		return make_shared<SceneTitle>();	// �^�C�g���V�[���֍s��
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
