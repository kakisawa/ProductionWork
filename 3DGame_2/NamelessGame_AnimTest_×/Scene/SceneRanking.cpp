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

	if (Pad::IsTrigger(PAD_INPUT_1))		// Z�L�[(PAD::A)����������
	{
		return make_shared<SceneTitle>();	// �^�C�g���V�[���֍s��
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
