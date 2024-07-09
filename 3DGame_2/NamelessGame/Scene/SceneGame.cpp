#include "SceneGame.h"
#include "SceneGameClear.h"
#include "SceneGameOver.h"
#include "../Pad.h"

SceneGame::SceneGame()
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
}

shared_ptr<SceneBase> SceneGame::Update()
{
	Pad::Update();



#ifdef _DEBUG
	if (Pad::IsTrigger(PAD_INPUT_2))		// X�L�[(PAD::B)����������
	{
		return make_shared<SceneGameClear>();	// �Q�[���N���A�֍s��
	}
	if (Pad::IsTrigger(PAD_INPUT_3))		// C�L�[(PAD::X)����������
	{
		return make_shared<SceneGameOver>();	// �Q�[���I�[�o�[�֍s��
	}
#endif // _DEBUG

	return shared_from_this();
}

void SceneGame::Draw()
{
	DrawString(0, 0, "SceneGame", 0xffffff);
}

void SceneGame::End()
{
}
