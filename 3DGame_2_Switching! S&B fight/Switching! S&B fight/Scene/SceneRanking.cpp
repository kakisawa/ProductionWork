#include "SceneRanking.h"
#include "SceneTitle.h"
#include "../Util/Pad.h"
#include "../Util/Fade.h"
#include <cassert>

namespace
{
	const char* const kUI[4]{

		"data/Image/RankingScene/Firefly-������-�O��-001-scaled.jpg",
		"data/Image/RankingScene/UI/ComingSoon.png",
		"data/Image/RankingScene/UI/�����w�i.png",// �Z���N�g�w�i
		"data/Image/RankingScene/UI/������.png",	// ������//�v�C��
	};

	constexpr float kCaptionScroll = 1.5f;

	constexpr int kCaptionBgPosY = 1003;
	constexpr int kCaptionPosY = 1015;
}

SceneRanking::SceneRanking()
{
}

SceneRanking::~SceneRanking()
{
}

void SceneRanking::Init()
{
	// UI�摜�̓ǂݍ���
	for (int i = 0; i < m_uiGraph.size(); i++) {
		m_uiGraph[i] = LoadGraph(kUI[i]);
		assert(m_uiGraph[i] != -1);
	}


	m_pSound->Init();	// �T�E���h�̏�����
	m_pSound->LoadSE(SoundManager::SE_Type::kButtonSE);

	m_pFade = std::make_shared<Fade>();
}

shared_ptr<SceneBase> SceneRanking::Update()
{
	m_pFade->FadeIn(m_pFade->GatFadeInFlag());

	// �w�i�X�N���[��
	m_scrollX_1 += kCaptionScroll;

	m_pFade->FadeOut(m_pFade->GatFadeOutFlag());
	m_isNextSceneFlag = m_pFade->GatNextSceneFlag();

	if (Pad::IsTrigger(PAD_INPUT_A))		// RB�{�^��
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kButtonSE, DX_PLAYTYPE_BACK);
		m_pFade->SetFadeOutFlag(true);
	}

	if (m_isNextSceneFlag)
	{
		return make_shared<SceneTitle>();	// �^�C�g���V�[���֍s��
	}
#ifdef _DEBUG

#endif // DEBUG
	return shared_from_this();
}

void SceneRanking::Draw()
{
	DrawExtendGraph(0, 0, 1920, 1080, m_uiGraph[0], false);	// �w�i
	DrawGraph(200, 400, m_uiGraph[1], true);

	DrawExplanation();

	m_pFade->Draw();

#ifdef _DEBUG
	DrawString(0, 0, "SceneRanking", 0xffffff); 
	DrawString(0, 20, "������", 0xffffff);
#endif // DEBUG
}

void SceneRanking::End()
{
	// UI�摜�̍폜
	for (int i = 0; i < m_uiGraph.size(); i++) {
		DeleteGraph(m_uiGraph[i]);
	}

	SceneBase::End();
}

void SceneRanking::DrawExplanation()
{
	// �w�i�摜�̃T�C�Y�擾
	Size expBgSize, expSize;
	GetGraphSize(m_uiGraph[2], &expBgSize.m_width, &expBgSize.m_height);
	GetGraphSize(m_uiGraph[3], &expSize.m_width, &expSize.m_height);

	// �X�N���[���ʂ��v�Z
	int scrollBg = static_cast<int>(m_scrollX_1) % static_cast<int>(expBgSize.m_width);

	// �������w�i�`��
	for (int index = 0; index < 4; index++)
	{
		DrawGraph(-scrollBg + index * expBgSize.m_width,
			static_cast<int>(kCaptionBgPosY),
			m_uiGraph[2], true);

		DrawGraph(-scrollBg + index * expSize.m_width,
			static_cast<int>(kCaptionPosY),
			m_uiGraph[3], true);
	}
}