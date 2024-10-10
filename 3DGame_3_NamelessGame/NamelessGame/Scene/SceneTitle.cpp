#include "SceneTitle.h"
#include "SceneSelect.h"
#include "SceneDebug.h"

SceneTitle::SceneTitle()
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
}

std::shared_ptr<SceneBase> SceneTitle::Update(Input& input)
{
	if (input.IsTrigger(InputInfo::OK)) {			// STARTボタン

		return std::make_shared<SceneSelect>();	// ゲームセレクトシーンへ行く
	}


#ifdef _DEBUG
	if (input.IsTrigger(InputInfo::DebugStart)) {			// STARTボタン

		return std::make_shared<SceneDebug>();	// デバッグシーンへ行く
	}
#endif // DEBUG

	return shared_from_this();
}

void SceneTitle::Draw()
{
#ifdef _DEBUG
	DrawString(0, 0, "SceneTitle", 0xffffff);
#endif // DEBUG
}

void SceneTitle::End()
{
}
