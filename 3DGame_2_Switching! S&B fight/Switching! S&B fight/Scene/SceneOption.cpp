#include "SceneOption.h"
#include "SceneTitle.h"
#include "../Util/Pad.h"

SceneOption::SceneOption()
{
}

SceneOption::~SceneOption()
{
}

void SceneOption::Init()
{
	m_pSound->InitSound();	// �T�E���h�̏�����
	m_pSound->LoadBGM(SoundManager::BGM_Type::kOptionBGM);	// �T�E���h�̓ǂݍ���
	m_pSound->LoadSE(SoundManager::SE_Type::kSelectSE);
	m_pSound->LoadSE(SoundManager::SE_Type::kButtonSE);

	m_pSound->PlayBGM(SoundManager::BGM_Type::kOptionBGM, DX_PLAYTYPE_LOOP);
}

shared_ptr<SceneBase> SceneOption::Update()
{
	if (Pad::IsTrigger(PAD_INPUT_R))		// RB�{�^��
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kButtonSE, DX_PLAYTYPE_NORMAL);
		return make_shared<SceneTitle>();	// �^�C�g���V�[���֍s��
	}

#ifdef _DEBUG
#endif // DEBUG

	return shared_from_this();
}

void SceneOption::Draw()
{
	DrawString(0, 0, "SceneOption", 0xffffff);
	DrawString(0, 20, "Please Press Button START", 0x00ffff);
}

void SceneOption::End()
{
	SceneBase::End();
}
