                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 #include "SceneGameOver.h"
#include "SceneTitle.h"
#include "../Pad.h"
#include "DxLib.h"

namespace {
}

SceneGameOver::SceneGameOver()
{
	// �摜�ǂݍ���
}

SceneGameOver::~SceneGameOver()
{
	// �摜�폜
}

std::shared_ptr<SceneBase> SceneGameOver::Update()
{
	Pad::Update();

	//// �t�F�[�h�C��
	//if (!m_isFadeIn)
	//{
	//	m_fadeAlpha -= 8;
	//	if (m_fadeAlpha < 0)
	//	{
	//		m_fadeAlpha = 0;
	//		m_isFadeIn = true;
	//	}
	//}
	//if (Pad::IsTrigger(PAD_INPUT_10))		// �X�y�[�X�L�[�������ꂽ��
	//{
	//	m_isFadeOut = true;					// �t�F�[�h�A�E�g�t���O��true�ɂ���
	//	m_pSound->SoundButton();			// SE��炷
	//}
	//// �t�F�[�h�A�E�g
	//if (m_isFadeOut)
	//{
	//	if (m_isSceneEnd)
	//	{
	//		return std::make_shared<SceneTitle>();	// �^�C�g���V�[���֍s��
	//	}
	//	m_fadeAlpha += 8;
	//	if (m_fadeAlpha >= 255)
	//	{
	//		// BGM���~�߂�
	//		m_pSound->StopBGMGameOver();
	//		m_fadeAlpha = 255;
	//		m_isSceneEnd = true;
	//	}
	//}
	if (Pad::IsTrigger(PAD_INPUT_10))
	{
		return make_shared<SceneTitle>();
	}

	return shared_from_this();	// ���g��shared_ptr��Ԃ�
}

void SceneGameOver::Draw()
{

	DrawString(0, 0, "SceneGameOver", 0xffffff);

	//// �t�F�[�h�C���E�t�F�[�h�A�E�g�`��
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// �������ŕ\���J�n
	//DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, 0x00000, true);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// �s�����ɖ߂��Ă���
}