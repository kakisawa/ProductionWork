#include "SoundManager.h"
#include "../Util/Pad.h"
#include "DxLib.h"

namespace {
	// 音量の最大
	constexpr int kMaxVolume = 255;

	// 初期音量
	constexpr float kInitBgmVolume = kMaxVolume*0.5f;	// BGM初期音量
	constexpr float kInitSeVolume = kMaxVolume* 0.5f;	// SE初期音量

	// 変更後音量保存
	float kChangeBgm = kInitBgmVolume;
	float kChangeSe = kInitSeVolume;
}

void SoundManager::Init()
{
	m_bgmVolume = kChangeBgm;
	m_seVolume = kChangeSe;

	m_select == Select::kBgmVolume;

	InitSound();
}

void SoundManager::ChangeSound()
{
	if (Pad::IsTrigger(PAD_INPUT_RIGHT))
	{
		if (m_select == kSeVolume)
		{
			m_seVolume += kMaxVolume * 0.08f;
			SetSeVolume();
			PlaySE(SE_Type::kSelectSE, DX_PLAYTYPE_BACK);
			if (m_seVolume >= kMaxVolume)
			{
				m_seVolume = kMaxVolume;
			}
			kChangeSe = m_seVolume;
		}
		else if (m_select == kBgmVolume)
		{
			m_bgmVolume += kMaxVolume * 0.08f;
			SetBgmVolume();
			if (m_bgmVolume >= kMaxVolume)
			{
				m_bgmVolume = kMaxVolume;
			}
			kChangeBgm = m_bgmVolume;
		}
	}
	else if (Pad::IsTrigger(PAD_INPUT_LEFT))
	{
		if (m_select == kBgmVolume)
		{
			m_bgmVolume -= kMaxVolume * 0.08f;
			SetBgmVolume();
			if (m_bgmVolume <= 0)
			{
				m_bgmVolume = 0;
			}
			kChangeBgm = m_bgmVolume;
		}
		if (m_select == kSeVolume)
		{
			m_seVolume -= kMaxVolume * 0.08f;
			SetSeVolume();
			PlaySE(SE_Type::kSelectSE, DX_PLAYTYPE_BACK);
			if (m_seVolume <= 0)
			{
				m_seVolume = 0;
			}
			kChangeSe = m_seVolume;
		}
	}
}

void SoundManager::SelectOption()
{
	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		m_select += 1;
		if (m_select >= 3)
		{
			m_select = Select::kBgmVolume;
		}
	}
	if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		m_select -= 1;
		if (m_select <= -1)
		{
			m_select = Select::kBack;
		}
	}
}

void SoundManager::InitSound(void)
{
	InitBGM();
	InitSE();
}

void SoundManager::InitBGM(void)
{
	m_bgmPass[BGM_Type::kTitleBGM] = "Title.mp3";
	m_bgmPass[BGM_Type::kGameBGM] = "MECHANICAL_DEATH.mp3";
	m_bgmPass[BGM_Type::kGameClearBGM] = "ゲームクリアー！.mp3";
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
	m_sePass[SE_Type::kSordSE] = "しなる風切り音.mp3";
	m_sePass[SE_Type::kBowSE] = "se_gun_fire05.mp3";
	m_sePass[SE_Type::kExtinction] = "データ表示4.mp3";
}

void SoundManager::LoadSE(SE_Type se)
{
	SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);
	m_se[se] = LoadSoundMem(("Data/Sound/SE/" + m_sePass[se]).c_str(),6);
}

void SoundManager::PlayBGM(BGM_Type bgm, int playType, int volumePar, bool topPositionFlag)
{
	// 今回用
	ChangeVolumeSoundMem(m_bgmVolume, m_bgm[bgm]);
	PlaySoundMem(m_bgm[bgm], playType, topPositionFlag);

	/*ChangeVolumeSoundMem((255 * volumePar / 100), m_bgm[bgm]);
	PlaySoundMem(m_bgm[bgm], playType, topPositionFlag);*/
}

void SoundManager::PlaySE(SE_Type se, int playType, int volumePar, bool topPositionFlag)
{
	// 今回用
	ChangeVolumeSoundMem(m_seVolume, m_se[se]);
	PlaySoundMem(m_se[se], playType, topPositionFlag);

	/*ChangeVolumeSoundMem((255 * volumePar / 100), m_se[se]);
	PlaySoundMem(m_se[se], playType, topPositionFlag);*/
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

void SoundManager::SetBgmVolume()
{
	for (const auto& entry : m_bgm) {
		ChangeVolumeSoundMem(static_cast<int>(m_bgmVolume),entry.second);
	}
}

void SoundManager::SetSeVolume()
{
	for (const auto& entry : m_se) {
		ChangeVolumeSoundMem(static_cast<int>(m_bgmVolume), entry.second);
	}
}

void SoundManager::Draw()
{
	DrawFormatString(0, 500, 0xffffff, "Volume=%.2f", m_seVolume);
	
}
