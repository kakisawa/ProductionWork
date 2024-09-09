#include "Fade.h"
#include "DxLib.h"

Fade::Fade():
	m_fadeAlpha(255), 
	m_isFadeIn(true),
	m_isFadeOut(false),
	m_isNextSceneFlag(false)
{
}

Fade::~Fade()
{
}

void Fade::Init()
{
}

void Fade::Update()
{
}

void Fade::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// �������ŕ\���J�n
	DrawBoxAA(0, 0, 1920, 1080, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// �s�����ɖ߂��Ă���
}

void Fade::FadeIn(bool flag)
{
	m_isFadeIn = flag;

	// �t�F�[�h�C��
	if (m_isFadeIn)
	{
		m_fadeAlpha -= 8;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
			m_isFadeIn = false;
		}
	}
}

void Fade::FadeOut(bool flag)
{
	m_isFadeOut = flag;

	// �t�F�[�h�A�E�g
	if (m_isFadeOut)
	{
		m_fadeAlpha += 8;
		if (m_fadeAlpha > 255)
		{
			m_fadeAlpha = 255;
			m_isNextSceneFlag = true;
			m_isFadeOut = false;
		}
	}
}
