#include "SceneTitle.h"
#include "SceneGame.h"
#include "DxLib.h"
#include "../Pad.h"
#include "../Camera.h"

namespace {
	const char* const kModelPlayer = "data/model/Player.mv1";
	float camera_y=0.0f;
}

SceneTitle::SceneTitle():
	m_scele(5.0f),
	m_pos(VGet(0.0f, 1.0f, 0.0f)),
	m_model(MV1LoadModel(kModelPlayer))// �v���C���[�̃��[�h
{
	MV1SetScale(m_model, VGet(m_scele, m_scele, m_scele));
	MV1SetPosition(m_model, m_pos);

	m_pCamera->Init_Title(m_pos);
}

SceneTitle::~SceneTitle()
{
	// �摜�폜
}

shared_ptr<SceneBase> SceneTitle::Update()
{
	Pad::Update();
	
	/*m_pCamera->AddCameraPos(VGet(0, camera_y, 0));
	camera_y += 0.1f;*/

	//m_pCamera->Update();

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

	// �v���C���[�`��
	MV1DrawModel(m_model);

	//// �t�F�[�h�C���E�t�F�[�h�A�E�g�`��
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// �������ŕ\���J�n
	//DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, 0x00000, true);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// �s�����ɖ߂��Ă���
}

void SceneTitle::End()
{// �v���C���[�̃A�����[�h
	MV1DeleteModel(m_model);
	m_model = -1;
}
