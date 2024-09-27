#include "SceneManager.h"
#include "Input.h"
#include "SceneBase.h"
#include "SceneTitle.h"
#include "SceneDebug.h"


/// <summary>
/// コンストラクタ
/// </summary>
SceneManager::SceneManager():
	m_pScene(nullptr)
{
}


/// <summary>
/// デストラクタ
/// </summary>
SceneManager::~SceneManager()
{
}


/// <summary>
/// 初期化
/// </summary>
void SceneManager::Init()
{
#ifdef _DEBUG
	m_pScene = std::make_shared<SceneDebug>();
	m_pScene->Init();
#else

	m_pScene = std::make_shared<SceneTitle>();
	m_pScene->Init();
#endif
}


/// <summary>
/// 更新
/// </summary>
void SceneManager::Update(Input& input)
{
	std::shared_ptr<SceneBase> pNext = m_pScene->Update(input);

	// 遷移先のシーンの開始処理を行う
	if (pNext != m_pScene)
	{
		// Updateが返した新しいシーンの開始処理を行う
		m_pScene = pNext;
		m_pScene->Init();
	}
}


/// <summary>
/// 描画
/// </summary>
void SceneManager::Draw()
{
	m_pScene->Draw();
}
