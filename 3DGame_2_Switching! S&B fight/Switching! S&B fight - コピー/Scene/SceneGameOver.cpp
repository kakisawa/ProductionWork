#include "SceneGameOver.h"
#include "SceneTitle.h"
#include "../Util/Pad.h"

namespace {
	constexpr int kTitlePosX = 500;
	constexpr int kTitlePosY = 150;
}

SceneGameOver::SceneGameOver() :
	m_graph(-1)
{
}

SceneGameOver::~SceneGameOver()
{
}

void SceneGameOver::Init()
{
	m_pSound->InitSound();	// �T�E���h�̏�����
	m_graph = LoadGraph("data/GameOver_.png");
}

shared_ptr<SceneBase> SceneGameOver::Update()
{
	if (Pad::IsTrigger(PAD_INPUT_R))		// RB�{�^��
	{
		return make_shared<SceneTitle>();	// �^�C�g���V�[���֍s��
	}


#ifdef _DEBUG
#endif // DEBUG

	return shared_from_this();
}

void SceneGameOver::Draw()
{
	DrawString(0, 0, "SceneGameOver", 0xffffff);
	DrawString(0, 20, "Please Press Button START", 0x00ffff);

	DrawGraph(kTitlePosX, kTitlePosY, m_graph, true);
}

void SceneGameOver::End()
{
	DeleteGraph(m_graph);
	SceneBase::End();
}
