#include "SceneManager.h"
#include "SceneBase.h"
#include "SceneTitle.h"
#include "SceneDebug.h"
#include "DxLib.h"
#include "../Util/Pad.h"

SceneManager::SceneManager() :
	m_pScene(nullptr)
{
}

void SceneManager::Init()
{
#ifdef _DEBUG

	m_pScene = make_shared<SceneDebug>();
#else
	// 最初のシーンのメモリを確保する
	m_pScene = make_shared<SceneTitle>();
	
#endif // DEBUG

	m_pScene->Init();					// シーンのInitを呼び出す
}

void SceneManager::Update()
{
	Pad::Update();

	shared_ptr<SceneBase> pNext = m_pScene->Update();	// シーンのUpdateを呼び出す
	if (pNext != m_pScene)
	{
		// 現在のシーンの終了処理
		m_pScene->End();

		// Updateが返した新しいシーンの開始処理を行う
		m_pScene = pNext;
		m_pScene->Init();
	}
}

void SceneManager::Draw()
{
	m_pScene->Draw();
}

void SceneManager::End()
{
	m_pScene->End();	// シーンのEndを呼び出す
}