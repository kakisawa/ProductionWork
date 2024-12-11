#include "SceneGameClear.h"
#include "SceneSelect.h"
#include "SceneDebug.h"

using namespace MyInputInfo;

SceneGameClear::SceneGameClear()
{
}

SceneGameClear::~SceneGameClear()
{
}

void SceneGameClear::Init()
{
	m_pSound->InitBGM();
	m_pSound->LoadBGM(SoundManager::BGM_Type::kGameClearBGM);

	m_pSound->PlayBGM(SoundManager::BGM_Type::kGameClearBGM, DX_PLAYTYPE_LOOP);
}

std::shared_ptr<SceneBase> SceneGameClear::Update(Input& input)
{
	
	if (input.IsTrigger(InputInfo::OK)) {			// Aボタン

		return std::make_shared<SceneSelect>();	// セレクトシーンへ行く
	}



#ifdef _DEBUG
	//if (input.IsTrigger(InputInfo::DebugStart)) {			// STARTボタン

	//	return std::make_shared<SceneDebug>();	// ゲームシーンへ行く
	//}
#endif // DEBUG

	return shared_from_this();
}

void SceneGameClear::Draw()
{

#ifdef _DEBUG
	//DrawString(0, 0, "SceneGameClear", 0xffffff);
#endif // DEBUG
}

void SceneGameClear::End()
{
	m_pSound->ReleaseSound();
}
