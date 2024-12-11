#include "SceneGameOver.h"
#include "SceneSelect.h"
#include "SceneDebug.h"

using namespace MyInputInfo;

SceneGameOver::SceneGameOver()
{
}

SceneGameOver::~SceneGameOver()
{
}

void SceneGameOver::Init()
{
}

std::shared_ptr<SceneBase> SceneGameOver::Update(Input& input)
{
	
	if (input.IsTrigger(InputInfo::OK)) {			// Aボタン

		return std::make_shared<SceneSelect> ();	// セレクトシーンへ行く
	}

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
