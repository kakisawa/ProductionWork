#include "DxLib.h"
#include "Input.h"
#include "Sound.h"
#include <cassert>

namespace
{
	// �T�E���h�̃f�[�^
	struct SoundData
	{
		const char* name;	// �T�E���h�̃t�@�C����
		int volumePal;		// ����
	};
	constexpr int kMaxVolumePal = 255; // �ő剹��
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
	/// ���[�h
	/// </summary>
	void Sound::Load()
	{
		Sound::m_bgmHandle.resize(static_cast<int>(Sound::BgmKind::kBgmNum));
		Sound::m_seHandle.resize(static_cast<int>(Sound::SeKind::kSeNum));

		// �T�E���h�f�[�^����
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
	/// �A�����[�h
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
	/// BGM�̉��ʂ�ύX����
	/// </summary>
	void ChangeBgmVol(Input& input)
	{
		// ���ʂ�������
		if (input.IsPressing("left"))
		{
			for (int i = 0; i < m_bgmHandle.size(); i++)
			{
				bgmData[i].volumePal--;
				bgmData[i].volumePal = std::max(0, bgmData[i].volumePal);
			}
		}
		// ���ʂ��グ��
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
	/// SE�̉��ʂ�ύX����
	/// </summary>
	void ChangeSeVol(Input& input)
	{
		// ���ʂ�������
		if (input.IsPressing("left"))
		{
			for (int i = 0; i < m_seHandle.size(); i++)
			{
				seData[i].volumePal--;
				seData[i].volumePal = std::max(0, seData[i].volumePal);
			}
		}
		// ���ʂ��グ��
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
	/// BGM�̉��ʂ��擾����
	/// </summary>
	/// <returns>���݂̉���</returns>
	int GetBgmVol()
	{
		// 0�`255��0�`100�ɕϊ�����
		int vol = static_cast<int>((bgmData[0].volumePal / static_cast<float>(kMaxVolumePal)) * 100);
		return vol;
	}


	/// <summary>
	/// SE�̉��ʂ��擾����
	/// </summary>
	/// <returns>���݂̉���</returns>
	int GetSeVol()
	{
		// 0�`255��0�`100�ɕϊ�����
		int vol = static_cast<int>((seData[0].volumePal / static_cast<float>(kMaxVolumePal)) * 100);
		return vol;
	}
}