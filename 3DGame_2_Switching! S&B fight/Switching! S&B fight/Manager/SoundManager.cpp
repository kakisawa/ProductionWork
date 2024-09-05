#include "SoundManager.h"
#include "DxLib.h"

void SoundManager::InitSound(void)
{
	InitBGM();
	InitSE();
}

void SoundManager::InitBGM(void)
{
	m_bgmPass[BGM_Type::kTitleBGM] = "Title.mp3";
	m_bgmPass[BGM_Type::kGameBGM] = "MECHANICAL_DEATH.mp3";
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
	m_sePass[SE_Type::kSelectSE] = "カーソル移動2.mp3";
	m_sePass[SE_Type::kButtonSE] = "決定ボタンを押す6.mp3";
	m_sePass[SE_Type::kBackSE] = "キャンセル7.mp3";
	m_sePass[SE_Type::kSord1SE] = "しなる風切り音.mp3";
	m_sePass[SE_Type::kSord2SE] = "しなる風切り音_2.mp3";
	m_sePass[SE_Type::kSord3SE] = "しなる風切り音_3.mp3";
}

void SoundManager::LoadSE(SE_Type se)
{
	SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);
	m_se[se] = LoadSoundMem(("Data/Sound/SE/" + m_sePass[se]).c_str(),6);
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
