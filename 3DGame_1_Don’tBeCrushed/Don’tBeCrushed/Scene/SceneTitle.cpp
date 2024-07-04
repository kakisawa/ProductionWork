#include "SceneTitle.h"
#include "SceneGame.h"
#include "DxLib.h"
#include "../Pad.h"
#include "../Camera.h"
#include "../Game.h"

namespace {
	const char* const kModelPlayer = "data/model/Title.mv1";	// ���f��
	const char* const kTitle = "data/graph/Title6.png";			// �^�C�g��
	const char* const kSelect1 = "data/graph/GameStart.png";	// �Z���N�g1(�Q�[���X�^�[�g)
	const char* const kSelect2 = "data/graph/Instructions.png";	// �Z���N�g2(�������)
	const char* const kSelect3 = "data/graph/Finish.png";		// �Z���N�g3(�Q�[���I��)

	constexpr int kFadeMax = 255;		// �t�F�[�h�lMAX
	constexpr float kCamera= 0.01f;		// �J�����̉�]��
	constexpr float kScale = 0.01f;		// ���f���T�C�Y

	constexpr int kTitlePosX = 50;		// �^�C�g��X���W
	constexpr int kTitlePosY = 20;		// �^�C�g��Y���W

	constexpr int kSelect1PosX = 100;	// �Z���N�g1X���W
	constexpr int kSelect2PosX = 700;	// �Z���N�g2X���W
	constexpr int kSelect3PosX = 1300;	// �Z���N�g3X���W
	constexpr int kSelectPosY = 780;	// �Z���N�gY���W
}

SceneTitle::SceneTitle():
	m_graph(LoadGraph(kTitle)),			// �^�C�g�����S�̃��[�h
	m_select(LoadGraph(kSelect1)),
	m_select2(LoadGraph(kSelect2)),
	m_select3(LoadGraph(kSelect3)),
	m_model(MV1LoadModel(kModelPlayer)),// ���f���̃��[�h
	m_fadeAlpha(kFadeMax),
	m_scele(kScale),
	m_angle(0.0f),
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


	m_angle++;
	float angle2 = m_angle * (DX_PI_F / 180.0f);


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
		if (m_fadeAlpha >= kFadeMax)
		{
			m_isSceneEnd = true;
			m_fadeAlpha = kFadeMax;
		}
	}
	return shared_from_this();
}

void SceneTitle::Draw()
{
	// ���f���`��
	MV1DrawModel(m_model);

	// �^�C�g�����S�`��
	DrawGraph(kTitlePosX, kTitlePosY, m_graph, true);

	// �Z���N�g�`��
	DrawGraph(kSelect1PosX, kSelectPosY, m_select,true);
	DrawGraph(kSelect2PosX, kSelectPosY, m_select2, true);
	DrawGraph(kSelect3PosX, kSelectPosY, m_select3, true);

	//angle2
	
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
