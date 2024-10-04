#include "SceneRanking.h"
#include "SceneDebug.h"

SceneRanking::SceneRanking()
{
}

SceneRanking::~SceneRanking()
{
}

void SceneRanking::Init()
{
}

std::shared_ptr<SceneBase> SceneRanking::Update()
{


#ifdef _DEBUG
	if (Pad::IsTrigger(PAD_INPUT_R)) {			// STARTボタン

		return std::make_shared<SceneDebug>();	// ゲームシーンへ行く
	}
#endif // DEBUG

	return shared_from_this();
}

void SceneRanking::Draw()
{

#ifdef _DEBUG
	DrawString(0, 0, "SceneRanking", 0xffffff);
#endif // DEBUG
}

void SceneRanking::End()
{
}
