#include "SceneDebug.h"
#include "SceneGame.h"
#include "../Pad.h"

SceneDebug::SceneDebug():
	m_nextScene(nextScene::kGameScene)
{
}

void SceneDebug::Init()
{
}

std::shared_ptr<SceneBase> SceneDebug::Update()
{
	if (Pad::IsTrigger(PAD_INPUT_R)) {
		if (m_nextScene == nextScene::kGameScene)
		{
			return std::make_shared<SceneGame>();	// ゲームシーンへ行く
		}
	}

	return shared_from_this();
}

void SceneDebug::Draw()
{
	DrawString(0, 0, "SceneDebug", 0xffffff);
	DrawFormatString(0, 700, 0xffffff, "m_nextScene=%d", m_nextScene);
}
