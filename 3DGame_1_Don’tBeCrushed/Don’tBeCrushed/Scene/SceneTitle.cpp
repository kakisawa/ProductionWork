#include "SceneTitle.h"
#include "SceneGame.h"
#include "DxLib.h"
#include "../Pad.h"

namespace {
}

SceneTitle::SceneTitle()
{
	// �摜�ǂݍ���
}

SceneTitle::~SceneTitle()
{
	// �摜�폜
}

shared_ptr<SceneBase> SceneTitle::Update()
{
	Pad::Update();

	//// �t�F�[�h�C��
	//if (m_isFadeIn)
	//{
	//	m_fadeAlpha -= 8;
	//	if (m_fadeAlpha < 0)
	//	{
	//		m_fadeAlpha = 0;
	//		m_isFadeIn = false;
	//	}
	//}
	//if (Pad::IsTrigger(PAD_INPUT_10))
	//{
	//	m_pSound->SoundButton();
	//	m_isFadeOut = true;
	//	m_isFadeIn = false;
	//}
	//// �t�F�[�h�A�E�g
	//if (m_isFadeOut)
	//{
	//	if (m_isSceneEnd)
	//	{
	//		return make_shared<SceneGame>();	// �Q�[���V�[���֍s��
	//	}
	//	m_fadeAlpha += 8;
	//	if (m_fadeAlpha >= 255)
	//	{
	//		m_isSceneEnd = true;
	//		m_fadeAlpha = 255;
	//	}
	//}


	if (Pad::IsTrigger(PAD_INPUT_10))
	{
		return make_shared<SceneGame>();
	}
	return shared_from_this();
}

void SceneTitle::Draw()
{
	DrawString(0, 0, "SceneTitle", 0xffffff);

	//// �t�F�[�h�C���E�t�F�[�h�A�E�g�`��
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// �������ŕ\���J�n
	//DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, 0x00000, true);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// �s�����ɖ߂��Ă���
}