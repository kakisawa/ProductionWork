#include "SceneBase.h"

SceneBase::SceneBase():
	m_isNextSceneFlag(false)
{
	m_pSound = new SoundManager();
}

void SceneBase::End()
{
	m_pSound->ReleaseSound();

	delete m_pSound;
	m_pSound = nullptr;
}
