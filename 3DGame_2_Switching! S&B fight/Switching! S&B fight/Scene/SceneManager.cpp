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
	// �ŏ��̃V�[���̃��������m�ۂ���
	m_pScene = make_shared<SceneTitle>();
	
#endif // DEBUG

	m_pScene->Init();					// �V�[����Init���Ăяo��
}

void SceneManager::Update()
{
	Pad::Update();

	shared_ptr<SceneBase> pNext = m_pScene->Update();	// �V�[����Update���Ăяo��
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