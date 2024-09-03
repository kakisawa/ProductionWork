#include "SceneTitle.h"
#include "DxLib.h"
#include "SceneGame.h"
#include "SceneOption.h"
#include "SceneRanking.h"
#include "../Manager/SoundManager.h"
#include "../Util/Pad.h"

namespace {
	constexpr int kTitlePosX= 300;
	constexpr int kTitlePosY= 20;

}

SceneTitle::SceneTitle() :
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

	m_pSound->InitSound();	// �T�E���h�̏�����
	m_pSound->LoadBGM(SoundManager::BGM_Type::kTitleBGM);	// �T�E���h�̓ǂݍ���

	m_pSound->PlayBGM(SoundManager::BGM_Type::kTitleBGM, DX_PLAYTYPE_LOOP);
	m_isNextSceneFlag = false;
}

shared_ptr<SceneBase> SceneTitle::Update()
{
	// �V�[���J��

	if (Pad::IsTrigger(PAD_INPUT_R)) {		// RB�{�^��
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



	SwitchingScene();


#ifdef _DEBUG

	if (m_nextScene == nextScene::kNone)
	{
		if (Pad::IsNotPress(PAD_INPUT_Z)) {	// RB�{�^��
			m_nextScene = nextScene::kGameScene;
		}
	}

#endif // DEBUG

	return shared_from_this();
}

void SceneTitle::Draw()
{
	DrawExtendGraph(kTitlePosX, kTitlePosY, kTitlePosX+1200, kTitlePosY+430, m_titleGraph, true);

	DrawString(0, 20, "Please Press Button Start", 0x00ffff);

#ifdef _DEBUG
	DrawString(0, 0, "SceneTitle", 0xffffff);
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
