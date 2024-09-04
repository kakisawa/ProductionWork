#include "SceneTitle.h"
#include "DxLib.h"
#include "SceneGame.h"
#include "SceneOption.h"
#include "SceneRanking.h"
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

	//�|�C���^�̊m�ۂ�SceneBase
	m_pSound->InitSound();	// �T�E���h�̏�����
	m_pSound->LoadBGM(SoundManager::BGM_Type::kTitleBGM);	// �T�E���h�̓ǂݍ���
	m_pSound->LoadSE(SoundManager::SE_Type::kSelectSE);
	m_pSound->LoadSE(SoundManager::SE_Type::kButtonSE);

	m_pSound->PlayBGM(SoundManager::BGM_Type::kTitleBGM, DX_PLAYTYPE_LOOP);

	m_isNextSceneFlag = false;
}

shared_ptr<SceneBase> SceneTitle::Update()
{

	// ���̃V�[���I��
	SwitchingScene();

	// �V�[���J��
	if (Pad::IsTrigger(PAD_INPUT_R)) {			// START�{�^��

		m_pSound->PlaySE(SoundManager::SE_Type::kButtonSE, DX_PLAYTYPE_NORMAL);

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
		if (m_nextScene == nextScene::kGameEnd)
		{
			DxLib_End();						// �Q�[�����I������
		}
	}

#ifdef _DEBUG
#endif // DEBUG

	return shared_from_this();
}

void SceneTitle::Draw()
{
	DrawExtendGraph(kTitlePosX, kTitlePosY, kTitlePosX+1200, kTitlePosY+430, m_titleGraph, true);

	DrawString(0, 20, "Please Press AnyButton", 0x00ffff);

#ifdef _DEBUG
	DrawString(0, 0, "SceneTitle", 0xffffff);
	DrawFormatString(0, 700, 0xffffff, "m_nextScene=%d", m_nextScene);
#endif // DEBUG
}

void SceneTitle::End()
{
	SceneBase::End();
}

void SceneTitle::SwitchingScene()
{
	// ���L�[�������Ǝ��̃V�[���̕ύX������
	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kSelectSE, DX_PLAYTYPE_BACK);

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
			m_nextScene = nextScene::kGameEnd;
		}
		else if(m_nextScene == nextScene::kGameEnd)
		{ 
			m_nextScene = nextScene::kGameScene;
		}
	}

	// ��L�[�������Ǝ��̃V�[���̕ύX������
	if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kSelectSE, DX_PLAYTYPE_BACK);

		if (m_nextScene == nextScene::kGameScene)
		{
			m_nextScene = nextScene::kGameEnd;
		}
		else if (m_nextScene == nextScene::kGameEnd)
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