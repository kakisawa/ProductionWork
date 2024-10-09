#include "SceneOption.h"
#include "SceneDebug.h"

SceneOption::SceneOption()
{
}

SceneOption::~SceneOption()
{
}

void SceneOption::Init()
{
}

std::shared_ptr<SceneBase> SceneOption::Update()
{
	

#ifdef _DEBUG
	if (input.IsTrigger(InputInfo::DebugStart)) {			// STARTボタン

		return std::make_shared<SceneDebug>();	// ゲームシーンへ行く
	}
#endif // DEBUG

	return shared_from_this();
}

void SceneOption::Draw()
{

#ifdef _DEBUG
	DrawString(0, 0, "SceneOption", 0xffffff);
#endif // DEBUG
}

void SceneOption::End()
{
}
