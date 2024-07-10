#include "SceneTitle.h"
#include "DxLib.h"
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
	graph = LoadGraph("data/UI/Asset1080p.png");	// �O�g
	graph2 = LoadGraph("data/UI/Asset8_1080p.png");	// ���I��
	graph3 = LoadGraph("data/UI/Asset_1080p.png");	// �I��

}

shared_ptr<SceneBase> SceneTitle::Update()
{
	Pad::Update();




	//if (Pad::IsTrigger(PAD_INPUT_1))		// Z�L�[(PAD::A)����������
	//{
	//	return make_shared<SceneGame>();	// �Q�[���V�[���֍s��
	//	return make_shared<SceneOption>();	// �I�v�V�����s��
	//	return make_shared<SceneRanking>();	// �����L���O�֍s��
	//}

#ifdef _DEBUG
	if (Pad::IsTrigger(PAD_INPUT_1))		// Z�L�[(PAD::A)����������
	{
		return make_shared<SceneGame>();	// �Q�[���V�[���֍s��
	}
	if (Pad::IsTrigger(PAD_INPUT_2))		// X�L�[(PAD::B)����������
	{
		return make_shared<SceneOption>();	// �I�v�V�����s��
	}
	if (Pad::IsTrigger(PAD_INPUT_3))		// C�L�[(PAD::X)����������
	{
		return make_shared<SceneRanking>();	// �����L���O�֍s��
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
