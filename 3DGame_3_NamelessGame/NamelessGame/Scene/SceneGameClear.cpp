#include "SceneGameClear.h"
#include "SceneDebug.h"

SceneGameClear::SceneGameClear()
{
}

SceneGameClear::~SceneGameClear()
{
}

void SceneGameClear::Init()
{
}

std::shared_ptr<SceneBase> SceneGameClear::Update()
{
	


#ifdef _DEBUG
	if (input.IsTrigger(InputInfo::DebugStart)) {			// STARTボタン

		return std::make_shared<SceneDebug>();	// ゲームシーンへ行く
	}
#endif // DEBUG

	return shared_from_this();
}

void SceneGameClear::Draw()
{

#ifdef _DEBUG
	DrawString(0, 0, "SceneGameClear", 0xffffff);
#endif // DEBUG
}

void SceneGameClear::End()
{
}
