#include "DxLib.h"
#include "Input.h"
#include "Game.h"
#include "UI.h"
#include "Ranking.h"
#include "Sound.h"
#include "SceneBase.h"

namespace
{
	constexpr int kFadeColor = 0x1a0306; // �t�F�[�h�̐F
	constexpr int kMaxFade = 255;		 // �t�F�[�h�̍ő�l
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneBase::SceneBase():
	m_select(0),
	m_fadeAlpha(0),
	m_isFadeOut(false)
{
	m_pUI = std::make_shared<UI>();
	m_pRank = std::make_shared<Ranking>();
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneBase::~SceneBase()
{
}


/// <summary>
/// �I����Ԃ��X�V
/// </summary>
/// <param name="input">���͏��</param>
/// <param name="selectNum">�I�����̐�</param>
void SceneBase::UpdateSelect(Input& input, int selectNum)
{
	// �I����Ԃ�1������
	if (input.IsTriggered("down"))
	{
		m_select = (m_select + 1) % selectNum;
		m_pUI->Init();
		PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kCursor)], DX_PLAYTYPE_BACK);
	}
	// �I����Ԃ�1�グ��
	if (input.IsTriggered("up"))
	{
		m_select = (m_select + (selectNum - 1)) % selectNum;
		m_pUI->Init();
		PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kCursor)], DX_PLAYTYPE_BACK);
	}
}


/// <summary>
/// �t�F�[�h�C������
/// </summary>
/// <param name="fadeFrame">�t�F�[�h�ω���</param>
void SceneBase::FadeIn(int fadeFrame)
{
	m_fadeAlpha += fadeFrame;
	m_fadeAlpha = std::min(m_fadeAlpha, kMaxFade);
}


/// <summary>
/// �t�F�[�h�A�E�g����
/// </summary>
/// <param name="fadeFrame">�t�F�[�h�ω���</param>
void SceneBase::FadeOut(int fadeFrame)
{
	m_fadeAlpha -= fadeFrame;
	m_fadeAlpha = std::max(0, m_fadeAlpha);

	if (m_fadeAlpha <= 0)
	{
		m_isFadeOut = false;
	}
	else
	{
		m_isFadeOut = true;
	}
}


/// <summary>
/// �t�F�[�h�C���A�E�g�̕`��
/// </summary>
void SceneBase::DrawFade()
{
	// �t�F�[�h�C��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, kFadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
