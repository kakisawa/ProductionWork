#include "SceneGameClear.h"
#include "SceneTitle.h"
#include "../Pad.h"
#include "DxLib.h"

namespace {
}

SceneGameClear::SceneGameClear()
{
	// �摜�ǂݍ���
}

SceneGameClear::~SceneGameClear()
{
	// �摜�폜
}

std::shared_ptr<SceneBase> SceneGameClear::Update()
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
	//// �uSpace�L�[���N���b�N�v�摜�_�ŗp
	//if (m_displayCount >= 80)
	//{
	//	m_displayCount = 0;
	//}
	//m_displayCount++;
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
	//		return make_shared<SceneTitle>();		// �^�C�g���V�[���֍s��
	//	}
	//	m_fadeAlpha += 8;
	//	if (m_fadeAlpha >= 255)
	//	{
	//		m_pSound->StopBGMGameClear();	// BGM���~�߂�
	//		m_fadeAlpha = 255;
	//		m_isSceneEnd = true;
	//	}
	//}

	if (Pad::IsTrigger(PAD_INPUT_10))
	{
		return make_shared<SceneTitle>();
	}

	return shared_from_this();		// ���g��shared_ptr��Ԃ�
}

void SceneGameClear::Draw()
{
	DrawString(0, 0, "SceneGameOver", 0xffffff);

	//// �t�F�[�h�C���E�t�F�[�h�A�E�g�`��
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// �������ŕ\���J�n
	//DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, 0x00000, true);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// �s�����ɖ߂��Ă���
}