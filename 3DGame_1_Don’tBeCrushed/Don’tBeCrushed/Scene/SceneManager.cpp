#include "SceneManager.h"
#include "SceneBase.h"
#include "SceneTitle.h"

SceneManager::SceneManager() :
	m_pScene(nullptr)
{
}

void SceneManager::Init()
{
	// 最初のシーンのメモリを確保する
	m_pScene = make_shared<SceneTitle>();
	m_pScene->Init();					// シーンのInitを呼び出す
}

void SceneManager::Update()
{
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
	m_pScene->Draw();	// シーンのDrawを呼び出す
}

void SceneManager::End()
{
	m_pScene->End();	// シーンのEndを呼び出す
}
