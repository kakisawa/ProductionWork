#include "SoundManager.h"
#include "DxLib.h"
//
//SoundManager* SoundManager::m_instance = nullptr;
//
//void SoundManager::CreateInstance(void)
//{
//	if (m_instance == nullptr)
//	{
//		m_instance = new SoundManager();
//	}
//	m_instance->InitSound();
//}
//
//SoundManager& SoundManager::GetInstance(void)
//{
//	return *m_instance;
//}

void SoundManager::InitSound(void)
{
	InitBGM();
	InitSE();
}

void SoundManager::InitBGM(void)
{
	m_bgmPass[BGM_Type::kTitleBGM] = "Title.mp3";
	m_bgmPass[BGM_Type::kGameBGM] = "";
	m_bgmPass[BGM_Type::kGameClearBGM] = "";
	m_bgmPass[BGM_Type::kGameOverBGM] = "";
	m_bgmPass[BGM_Type::kOptionBGM] = "Option.mp3";
}

void SoundManager::LoadBGM(BGM_Type bgm)
{
	m_bgm[bgm] = LoadSoundMem(("Data/Sound/BGM/" + m_bgmPass[bgm]).c_str());
}

void SoundManager::InitSE(void)
{
	m_sePass[SE_Type::kSelectSE] = "";
	m_sePass[SE_Type::kButtonSE] = "";
}

void SoundManager::LoadSE(SE_Type se)
{
	m_se[se] = LoadSoundMem(("Data/Sound/SE/" + m_sePass[se]).c_str());
}

void SoundManager::PlayBGM(BGM_Type bgm, int playType, int volumePar, bool topPositionFlag)
{
	ChangeVolumeSoundMem((255 * volumePar / 100), m_bgm[bgm]);
	PlaySoundMem(m_bgm[bgm], playType, topPositionFlag);
}

void SoundManager::PlaySE(SE_Type se, int playType, int volumePar, bool topPositionFlag)
{
	ChangeVolumeSoundMem((255 * volumePar / 100), m_se[se]);
	PlaySoundMem(m_se[se], playType, topPositionFlag);
}

bool SoundManager::CheckPlayBGM(BGM_Type bgm)
{
	return CheckSoundMem(m_bgm[bgm]);
}

bool SoundManager::CheckPlaySE(SE_Type se)
{
	return CheckSoundMem(m_se[se]);
}

void SoundManager::StopBGM(BGM_Type bgm)
{
	StopSoundMem(m_bgm[bgm]);
}

void SoundManager::StopSE(SE_Type se)
{
	StopSoundMem(m_se[se]);
}

void SoundManager::ReleaseSound(void)
{
	for (int b = 0; b < static_cast<int>(BGM_Type::MAX); b++)
	{
		DeleteSoundMem(m_bgm[static_cast<BGM_Type>(b)]);
	}

	for (int s = 0; s < static_cast<int>(SE_Type::MAX); s++)
	{
		DeleteSoundMem(m_se[static_cast<SE_Type>(s)]);
	}
}
