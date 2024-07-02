#include "SceneGameClear.h"
#include "SceneTitle.h"
#include "../Pad.h"
#include "DxLib.h"
#include "../Game.h"

namespace {
}

SceneGameClear::SceneGameClear() :
	m_fadeAlpha(255),
	m_isFadeIn(false),
	m_isFadeOut(false),
	m_isSceneEnd(false)
{
	// �摜�ǂݍ���
	graph = LoadGraph("data/graph/GameClear2.png");
}

SceneGameClear::~SceneGameClear()
{
	// �摜�폜
}

std::shared_ptr<SceneBase> SceneGameClear::Update()
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
			return make_shared<SceneTitle>();		// �^�C�g���V�[���֍s��
		}
		m_fadeAlpha += 8;
		if (m_fadeAlpha >= 255)
		{
			m_fadeAlpha = 255;
			m_isSceneEnd = true;
		}
	}

	return shared_from_this();		// ���g��shared_ptr��Ԃ�
}

void SceneGameClear::Draw()
{
	DrawString(0, 0, "SceneGameClear", 0xffffff);


	DrawGraph(100, 200, graph, true);

	// �t�F�[�h�C���E�t�F�[�h�A�E�g�`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// �������ŕ\���J�n
	DrawBoxAA(0, 0, kScreenWidth, kScreenHeight, 0x00000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// �s�����ɖ߂��Ă���
}