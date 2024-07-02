#include "SceneGameOver.h"
#include "SceneTitle.h"
#include "../Pad.h"
#include "DxLib.h"
#include "../Game.h"

namespace {
}

SceneGameOver::SceneGameOver():
	m_fadeAlpha(255),
	m_isFadeIn(false),
	m_isFadeOut(false),
	m_isSceneEnd(false)
{
	// �摜�ǂݍ���
	graph = LoadGraph("data/graph/GameOver2.png");
}

SceneGameOver::~SceneGameOver()
{
	// �摜�폜
}

std::shared_ptr<SceneBase> SceneGameOver::Update()
{
	Pad::Update();

	// �t�F�[�h�C��
	if (!m_isFadeIn)
	{
		m_fadeAlpha -= 8;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
			m_isFadeIn = true;
		}
	}
	if (Pad::IsTrigger(PAD_INPUT_10))		// �X�y�[�X�L�[�������ꂽ��
	{
		m_isFadeOut = true;					// �t�F�[�h�A�E�g�t���O��true�ɂ���
	}
	// �t�F�[�h�A�E�g
	if (m_isFadeOut)
	{
		if (m_isSceneEnd)
		{
			return std::make_shared<SceneTitle>();	// �^�C�g���V�[���֍s��
		}
		m_fadeAlpha += 8;
		if (m_fadeAlpha >= 255)
		{
			m_fadeAlpha = 255;
			m_isSceneEnd = true;
		}
	}
	return shared_from_this();	// ���g��shared_ptr��Ԃ�
}

void SceneGameOver::Draw()
{

	DrawString(0, 0, "SceneGameOver", 0xffffff);

	DrawGraph(100, 200, graph, true);

	// �t�F�[�h�C���E�t�F�[�h�A�E�g�`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// �������ŕ\���J�n
	DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, 0x00000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// �s�����ɖ߂��Ă���
}