#include "SceneSelect.h"
#include "SceneDebug.h"

SceneSelect::SceneSelect()
{
}

SceneSelect::~SceneSelect()
{
}

void SceneSelect::Init()
{
}

std::shared_ptr<SceneBase> SceneSelect::Update()
{
	

#ifdef _DEBUG
	if (Pad::IsTrigger(PAD_INPUT_R)) {			// STARTボタン

		return std::make_shared<SceneDebug>();	// ゲームシーンへ行く
	}
#endif // DEBUG

	return shared_from_this();

}

void SceneSelect::Draw()
{

#ifdef _DEBUG
	DrawString(0, 0, "SceneSelect", 0xffffff);
#endif // DEBUG
}

void SceneSelect::End()
{
}
