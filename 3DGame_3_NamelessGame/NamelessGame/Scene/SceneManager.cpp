#include "SceneManager.h"
#include "SceneDebug.h"
#include "../Pad.h"
#include "DxLib.h"

SceneManager::SceneManager() :
	m_pScene(nullptr)
{
}

void SceneManager::Init()
{
#ifdef _DEBUG

	m_pScene = std::make_shared<SceneDebug>();
#else
	// �ŏ��̃V�[���̃��������m�ۂ���
	//m_pScene = make_shared<SceneTitle>();

#endif // DEBUG

	m_pScene->Init();					// �V�[����Init���Ăяo��
}

void SceneManager::Update()
{
	Pad::Update();

	std::shared_ptr<SceneBase> pNext = m_pScene->Update();	// �V�[����Update���Ăяo��
	if (pNext != m_pScene)
	{
		// ���݂̃V�[���̏I������
		m_pScene->End();

		// Update���Ԃ����V�����V�[���̊J�n�������s��
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
	m_pScene->End();	// �V�[����End���Ăяo��
}
