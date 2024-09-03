#include "SceneDebug.h"
#include "../Util/Pad.h"
#include "SceneTitle.h"
#include "SceneOption.h"
#include "SceneRanking.h"
#include "SceneGame.h"
#include "SceneGameClear.h"
#include "SceneGameOver.h"

SceneDebug::SceneDebug():
	m_nextScene(nextScene::kTitleScene)
{
}

void SceneDebug::Init()
{
}

shared_ptr<SceneBase> SceneDebug::Update()
{
	if (Pad::IsTrigger(PAD_INPUT_R)) {			// START�{�^��
		if (m_nextScene == nextScene::kTitleScene)
		{
			return make_shared<SceneTitle>();	// �Q�[���V�[���֍s��
		}
		if (m_nextScene == nextScene::kOptionScene)
		{
			return make_shared<SceneOption>();	// �I�v�V�����V�[���֍s��
		}
		if (m_nextScene == nextScene::kRankingScene)
		{
			return make_shared<SceneRanking>();	// �����L���O�V�[���֍s��
		}
		if (m_nextScene == nextScene::kGameScene)
		{
			return make_shared<SceneGame>();	// �Q�[���V�[���֍s��
		}
		if (m_nextScene == nextScene::kGameClearScene)
		{
			return make_shared<SceneGameClear>();	// �Q�[���N���A�V�[���֍s��
		}
		if (m_nextScene == nextScene::kGameOverScene)
		{
			return make_shared<SceneGameOver>();	// �Q�[���I�[�o�[�V�[���֍s��
		}
		if (m_nextScene == nextScene::kGameEnd)
		{
			DxLib_End();						// �Q�[�����I������
		}
	}

	SwitchingScene();

	return shared_from_this();
}


void SceneDebug::Draw()
{
	DrawString(0, 0, "SceneDebug", 0xffffff);
	DrawFormatString(0, 700, 0xffffff, "m_nextScene=%d", m_nextScene);

}

void SceneDebug::SwitchingScene()
{
	// ���L�[�������Ǝ��̃V�[���̕ύX������
	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		if (m_nextScene == nextScene::kTitleScene)
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
		else if (m_nextScene == nextScene::kGameScene)
		{
			m_nextScene = nextScene::kGameClearScene;
		}
		else if (m_nextScene == nextScene::kGameClearScene)
		{
			m_nextScene = nextScene::kGameOverScene;
		}
		else if (m_nextScene == nextScene::kGameOverScene)
		{
			m_nextScene = nextScene::kGameEnd;
		}
		else if (m_nextScene == nextScene::kGameEnd)
		{
			m_nextScene = nextScene::kTitleScene;
		}
	}

	// ��L�[�������Ǝ��̃V�[���̕ύX������
	if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		if (m_nextScene == nextScene::kTitleScene)
		{
			m_nextScene = nextScene::kGameEnd;
		}
		else if (m_nextScene == nextScene::kGameEnd)
		{
			m_nextScene = nextScene::kGameOverScene;
		}
		else if (m_nextScene == nextScene::kGameOverScene)
		{
			m_nextScene = nextScene::kGameClearScene;
		}
		else if (m_nextScene == nextScene::kGameClearScene)
		{
			m_nextScene = nextScene::kGameScene;
		}
		else if (m_nextScene == nextScene::kGameScene)
		{
			m_nextScene = nextScene::kRankingScene;
		}
		else if (m_nextScene == nextScene::kRankingScene)
		{
			m_nextScene = nextScene::kOptionScene;
		}
		else if (m_nextScene == nextScene::kOptionScene)
		{
			m_nextScene = nextScene::kTitleScene;
		}
	}
}
