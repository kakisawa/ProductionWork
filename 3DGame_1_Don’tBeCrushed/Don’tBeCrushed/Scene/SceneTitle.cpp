#include "SceneTitle.h"
#include "SceneGame.h"
#include "DxLib.h"
#include "../Pad.h"
#include "../Camera.h"
#include "../Game.h"

namespace {
	const char* const kModelPlayer = "data/model/Title.mv1";
	const char* const kTitle = "data/graph/Title4.png";

	constexpr int kFadeMax = 255;
	constexpr float kCamera= 0.01f;
	constexpr float kScale = 0.01f;
}

SceneTitle::SceneTitle():
	m_graph(LoadGraph(kTitle)),			// �^�C�g�����S�̃��[�h
	m_model(MV1LoadModel(kModelPlayer)),// ���f���̃��[�h
	m_fadeAlpha(kFadeMax),
	m_scele(kScale),
	m_isFadeIn(true),
	m_isFadeOut(false),
	m_isSceneEnd(false),
	m_pos(VGet(0.0f, -5.0f, -13.0f))
{
	MV1SetScale(m_model, VGet(m_scele, m_scele, m_scele));	// ���f���̃T�C�Y�ݒ�
	MV1SetPosition(m_model, m_pos);							// ���f���̍��W�ݒ�

	// �J�����̏�����
	m_pCamera->Init_Title(m_pos);
}

SceneTitle::~SceneTitle()
{
	// �摜�폜
}

shared_ptr<SceneBase> SceneTitle::Update()
{
	Pad::Update();	// �p�b�h�̍X�V

	m_pCamera->AddCameraAngle(kCamera);	// �J�����̉�]�ʒǉ�
	m_pCamera->Update();// �J�����̍X�V

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
	if (Pad::IsTrigger(PAD_INPUT_10))	// ����Space�L�[�������ꂽ��
	{									// �t�F�[�h�A�E�g���n�߂�
		m_isFadeOut = true;
		m_isFadeIn = false;
	}

	// �t�F�[�h�A�E�g
	if (m_isFadeOut)
	{
		if (m_isSceneEnd)
		{
			return make_shared<SceneGame>();	// �Q�[���V�[���֍s��
		}
		m_fadeAlpha += 8;
		if (m_fadeAlpha >= 255)
		{
			m_isSceneEnd = true;
			m_fadeAlpha = 255;
		}
	}
	return shared_from_this();
}

void SceneTitle::Draw()
{
	// ���f���`��
	MV1DrawModel(m_model);

	// �^�C�g�����S�`��
	DrawGraph(50, 20, m_graph, true);

	// �t�F�[�h�C���E�t�F�[�h�A�E�g�`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// �������ŕ\���J�n
	DrawBox(0, 0, kScreenWidth, kScreenHeight, 0x00000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// �s�����ɖ߂��Ă���

#ifdef DEBUG
	DrawString(0, 0, "SceneTitle", 0xffffff);
#endif // DEBUG
}

void SceneTitle::End()
{
	// ���f���̃A�����[�h
	MV1DeleteModel(m_model);
	m_model = -1;
	// �^�C�g�����S�̍폜
	DeleteGraph(m_graph);
}
