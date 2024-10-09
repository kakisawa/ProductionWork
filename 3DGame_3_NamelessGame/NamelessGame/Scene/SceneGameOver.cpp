#include "SceneGameOver.h"
#include "SceneDebug.h"

SceneGameOver::SceneGameOver()
{
}

SceneGameOver::~SceneGameOver()
{
}

void SceneGameOver::Init()
{
}

std::shared_ptr<SceneBase> SceneGameOver::Update()
{
	


#ifdef _DEBUG
	if (input.IsTrigger(InputInfo::DebugStart)) {			// STARTボタン

		return std::make_shared<SceneDebug>();	// ゲームシーンへ行く
	}
#endif // DEBUG

	return shared_from_this();
}

void SceneGameOver::Draw()
{

#ifdef _DEBUG
	DrawString(0, 0, "SceneGameOver", 0xffffff);
#endif // DEBUG
}

void SceneGameOver::End()
{
}
