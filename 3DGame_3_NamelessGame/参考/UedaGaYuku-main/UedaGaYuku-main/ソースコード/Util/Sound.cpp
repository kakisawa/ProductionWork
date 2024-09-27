#include "DxLib.h"
#include "Input.h"
#include "Sound.h"
#include <cassert>

namespace
{
	// サウンドのデータ
	struct SoundData
	{
		const char* name;	// サウンドのファイル名
		int volumePal;		// 音量
	};
	constexpr int kMaxVolumePal = 255; // 最大音量
}

namespace Sound
{
	std::vector<int> m_bgmHandle;
	std::vector<int> m_seHandle;

	SoundData bgmData[] =
	{
		{"data/sound/BGM/title.mp3", kMaxVolumePal},
		{"data/sound/BGM/stageSelect.mp3", kMaxVolumePal},
		{"data/sound/BGM/stage1.wav", kMaxVolumePal},
		{"data/sound/BGM/stage2.wav", kMaxVolumePal},
		{"data/sound/BGM/clear.mp3", kMaxVolumePal},
		{"data/sound/BGM/gameover.mp3", kMaxVolumePal}
	};

	SoundData seData[] =
	{
		{"data/sound/SE/titleDisp.mp3", kMaxVolumePal},
		{"data/sound/SE/cursor.mp3", kMaxVolumePal},
		{"data/sound/SE/select.mp3", kMaxVolumePal},
		{"data/sound/SE/walk.wav", kMaxVolumePal},
		{"data/sound/SE/run.wav", kMaxVolumePal},
		{"data/sound/SE/attack.mp3", kMaxVolumePal},
		{"data/sound/SE/guardAttack.mp3", kMaxVolumePal},
		{"data/sound/SE/accumulateGauge.mp3", kMaxVolumePal},
		{"data/sound/SE/specialAttack.wav", kMaxVolumePal},
		{"data/sound/SE/avoid.mp3", kMaxVolumePal},
		{"data/sound/SE/battleStart.mp3", kMaxVolumePal},
		{"data/sound/SE/lastAttack.mp3", kMaxVolumePal},
		{"data/sound/SE/battleEnd.mp3", kMaxVolumePal},
		{"data/sound/SE/clearCheers.mp3", kMaxVolumePal}
	};


	/// <summary>
	/// ロード
	/// </summary>
	void Sound::Load()
	{
		Sound::m_bgmHandle.resize(static_cast<int>(Sound::BgmKind::kBgmNum));
		Sound::m_seHandle.resize(static_cast<int>(Sound::SeKind::kSeNum));

		// サウンドデータ生成
		for (int i = 0; i < m_bgmHandle.size(); i++)
		{
			m_bgmHandle[i] = LoadSoundMem(bgmData[i].name);
		}
		for (int i = 0; i < m_seHandle.size(); i++)
		{
			m_seHandle[i] = LoadSoundMem(seData[i].name);
		}
	}


	/// <summary>
	/// アンロード
	/// </summary>
	void Sound::UnLode()
	{
		for (const auto& handle : m_bgmHandle)
		{
			DeleteSoundMem(handle);
		}
		for (const auto& handle : m_seHandle)
		{
			DeleteSoundMem(handle);
		}
	}


	/// <summary>
	/// BGMの音量を変更する
	/// </summary>
	void ChangeBgmVol(Input& input)
	{
		// 音量を下げる
		if (input.IsPressing("left"))
		{
			for (int i = 0; i < m_bgmHandle.size(); i++)
			{
				bgmData[i].volumePal--;
				bgmData[i].volumePal = std::max(0, bgmData[i].volumePal);
			}
		}
		// 音量を上げる
		else if (input.IsPressing("right"))
		{
			for (int i = 0; i < m_bgmHandle.size(); i++)
			{
				bgmData[i].volumePal++;
				bgmData[i].volumePal = std::min(bgmData[i].volumePal, kMaxVolumePal);
			}
		}

		for (int i = 0; i < m_bgmHandle.size(); i++)
		{
			ChangeVolumeSoundMem(bgmData[i].volumePal, m_bgmHandle[i]);
		}
	}


	/// <summary>
	/// SEの音量を変更する
	/// </summary>
	void ChangeSeVol(Input& input)
	{
		// 音量を下げる
		if (input.IsPressing("left"))
		{
			for (int i = 0; i < m_seHandle.size(); i++)
			{
				seData[i].volumePal--;
				seData[i].volumePal = std::max(0, seData[i].volumePal);
			}
		}
		// 音量を上げる
		else if (input.IsPressing("right"))
		{
			for (int i = 0; i < m_seHandle.size(); i++)
			{
				seData[i].volumePal++;
				seData[i].volumePal = std::min(seData[i].volumePal, kMaxVolumePal);
			}
		}

		for (int i = 0; i < m_seHandle.size(); i++)
		{
			ChangeVolumeSoundMem(seData[i].volumePal, m_seHandle[i]);
		}
	}


	/// <summary>
	/// BGMの音量を取得する
	/// </summary>
	/// <returns>現在の音量</returns>
	int GetBgmVol()
	{
		// 0～255を0～100に変換する
		int vol = static_cast<int>((bgmData[0].volumePal / static_cast<float>(kMaxVolumePal)) * 100);
		return vol;
	}


	/// <summary>
	/// SEの音量を取得する
	/// </summary>
	/// <returns>現在の音量</returns>
	int GetSeVol()
	{
		// 0～255を0～100に変換する
		int vol = static_cast<int>((seData[0].volumePal / static_cast<float>(kMaxVolumePal)) * 100);
		return vol;
	}
}