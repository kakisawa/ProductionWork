#include "SceneTitle.h"
#include "DxLib.h"
#include "SceneGame.h"
#include "SceneOption.h"
#include "SceneRanking.h"
#include "../Pad.h"

SceneTitle::SceneTitle():
	graph(-1),
	graph2(-1),
	graph3(-1),
	m_titleGraph(-1),
	m_nextScene(nextScene::kNone)
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

	m_isNextSceneFlag = false;
}

shared_ptr<SceneBase> SceneTitle::Update()
{
	
	// �V�[���J��
	if (m_isNextSceneFlag)
	{
		if (Pad::IsTrigger(PAD_INPUT_Z)) {		// RB�{�^��
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
		
	}

	SwitchingScene();


#ifdef _DEBUG

	if (m_nextScene == nextScene::kNone)
	{
		if (Pad::IsNotPress(PAD_INPUT_Z)) {	// RB�{�^��
			m_isNextSceneFlag = true;
			m_nextScene = nextScene::kGameScene;
		}
	}
	
#endif // DEBUG

	return shared_from_this();
}

void SceneTitle::Draw()
{
	DrawString(0, 0, "SceneTitle", 0xffffff);

	DrawExtendGraph(150, 20, 1350, 450, m_titleGraph, true);

	DrawString(0, 20, "Please Press Button RB", 0x00ffff);

#ifdef _DEBUG

	DrawFormatString(0, 700, 0xffffff, "m_nextScene=%d", m_nextScene);

#endif // DEBUG
}

void SceneTitle::End()
{
}

void SceneTitle::SwitchingScene()
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
}
