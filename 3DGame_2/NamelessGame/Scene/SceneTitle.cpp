#include "SceneTitle.h"
#include "DxLib.h"
#include "SceneGame.h"
#include "SceneOption.h"
#include "SceneRanking.h"
#include "../Pad.h"

SceneTitle::SceneTitle():
	m_nextScene(nextScene::kGameScene)
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

	m_titleGraph = LoadGraph("data/Title.png");
}

shared_ptr<SceneBase> SceneTitle::Update()
{
	

	// ���L�[�������Ǝ��̃V�[���̕ύX������
	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		if (m_nextScene == nextScene::kGameScene)
		{
			m_nextScene = nextScene::kOptionScene;
		}
		else if (m_nextScene == nextScene::kOptionScene)
		{
			m_nextScene = nextScene::kRankingScene;
		}
		else if (m_nextScene == nextScene::kRankingScene)
		{
			m_nextScene = nextScene::kGameScene;
		}
	}
	// ��L�[�������Ǝ��̃V�[���̕ύX������
	if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		if (m_nextScene == nextScene::kGameScene)
		{
			m_nextScene = nextScene::kRankingScene;
		}
		else if (m_nextScene == nextScene::kRankingScene)
		{
			m_nextScene = nextScene::kOptionScene;
		}
		else if (m_nextScene == nextScene::kOptionScene)
		{
			m_nextScene = nextScene::kGameScene;
		}
	}


	if (Pad::IsTrigger(PAD_INPUT_1))		// Z�L�[(PAD::A)����������
	{
		if (m_nextScene == nextScene::kGameScene)
		{
			return make_shared<SceneGame>();	// �Q�[���V�[���֍s��
		}

		if (m_nextScene == nextScene::kOptionScene)
		{
			return make_shared<SceneOption>();	// �I�v�V�����V�[���֍s��
		}

		if (m_nextScene == nextScene::kRankingScene)
		{
			return make_shared<SceneRanking>();	// �����L���O�V�[���֍s��
		}
	}

#ifdef _DEBUG
	
	
#endif // DEBUG

	Pad::Update();

	return shared_from_this();
}

void SceneTitle::Draw()
{
	DrawString(0, 0, "SceneTitle", 0xffffff);

	DrawFormatString(0, 700, 0xffffff, "m_nextScene=%d", m_nextScene);

	DrawExtendGraph(150, 20,1350,450, m_titleGraph, true);
}

void SceneTitle::End()
{
}
