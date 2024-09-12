#include "SceneTitle.h"
#include "DxLib.h"
#include "SceneGame.h"
#include "SceneOption.h"
#include "SceneRanking.h"
#include "../Util/Pad.h"
#include "../Util/Fade.h"
#include "EffekseerForDXLib.h"
#include <cassert>

namespace {
	const char* const kUI[13]{
		"data/Image/TitleScene/UI/Title.png",			// �^�C�g�����S
		"data/Image/TitleScene/UI/Back.png",				// �Z���N�g�w�i
		"data/Image/TitleScene/UI/PressEnyButtom.png",	// PressEnyButton
		"data/Image/TitleScene/UI/GameStart.png",			// GameStart
		"data/Image/TitleScene/UI/Option.png",			// Option
		"data/Image/TitleScene/UI/Ranking.png",			// Ranking
		"data/Image/TitleScene/UI/GameFinish.png",		// GameFinish
		"data/Image/TitleScene/UI/�����w�i.png",			// �e���b�v�w�i
		"data/Image/TitleScene/UI/�ړ���1.png",			// �e���b�v1
		"data/Image/TitleScene/UI/�ړ���2.png",			// �e���b�v2
		"data/Image/TitleScene/UI/�ړ���3.png",			// �e���b�v3
		"data/Image/TitleScene/UI/�ړ���4.png",			// �e���b�v4
		"data/Image/TitleScene/UI/gf_o.png",				// �J�[�\��
	};

	constexpr int kTitlePosX = 300;			// �^�C�g�����S���W
	constexpr int kTitlePosY = 50;

	constexpr int kPressBgPosX = 630;		// �uPressEnyButton�v�w�i���W
	constexpr int kPressBgPosY = 780;
	constexpr int kPressEnyButtonPosX = 725;// �uPressEnyButton�vUI���W
	constexpr int kPressEnyButtonPosY = 810;

	constexpr int kSelectBgPosX = 200;		// �Z���N�g�w�i���W
	constexpr int kSelectBgPosY = 790;
	constexpr int kSelect1X = 300;			// �Z���N�g���W1
	constexpr int kSelect2X = 650;			// �Z���N�g���W2
	constexpr int kSelect3X = 950;			// �Z���N�g���W3
	constexpr int kSelect4X = 1250;			// �Z���N�g���W4
	constexpr int kSelectPosY = 805;		// �Z���N�g���W

	constexpr int kCaptionBgPosY = 1003;	// �e���b�v�w�i���W
	constexpr int kCaptionPosY = 1015;		// �e���b�v���W

	constexpr float kCaptionScroll = 1.5f;		// �e���b�v�X�N���[���ړ���
	constexpr float kCaptionPosX = -30.0f;		// �e���b�v�X�N���[���ړ���

	const VECTOR kSelectPos[8]{
		(VGet(kSelect1X,kSelectPosY,0)),			// �Z���N�g1���W����
		(VGet(kSelect1X + 272 ,kSelectPosY + 71,0)),// �Z���N�g1���W�E��
		(VGet(kSelect2X,kSelectPosY,0)),			// �Z���N�g2���W����
		(VGet(kSelect2X + 168 ,kSelectPosY + 71,0)),// �Z���N�g2���W�E��
		(VGet(kSelect3X,kSelectPosY,0)),			// �Z���N�g3���W����
		(VGet(kSelect3X + 210 ,kSelectPosY + 71,0)),// �Z���N�g3���W�E��
		(VGet(kSelect4X,kSelectPosY,0)),			// �Z���N�g4���W����
		(VGet(kSelect4X + 306 ,kSelectPosY + 71,0))	// �Z���N�g4���W�E��
	};
}

SceneTitle::SceneTitle() :
	m_bgGraph(-1),
	m_titleGraph(-1),
	m_scrollX(kCaptionPosX),
	m_isMenu(false),
	m_nextScene(nextScene::kGameScene),
	c1{ kSelectPos[0],kSelectPos[1] }
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	// �w�i�摜�̓ǂݍ���
	m_bgGraph = LoadGraph("data/Image/TitleScene/Castle_Forrest10.png");

	// UI�摜�̓ǂݍ���
	for (int i = 0; i < m_uiGraph.size(); i++) {
		m_uiGraph[i] = LoadGraph(kUI[i]);
		assert(m_uiGraph[i] != -1);
	}

	// �T�E���h�֌W
	//�|�C���^�̊m�ۂ�SceneBase
	m_pSound->Init();
	m_pSound->LoadBGM(SoundManager::BGM_Type::kTitleBGM);	// �T�E���h�̓ǂݍ���
	m_pSound->LoadSE(SoundManager::SE_Type::kSelectSE);
	m_pSound->LoadSE(SoundManager::SE_Type::kButtonSE);

	// BGM
	m_pSound->PlayBGM(SoundManager::BGM_Type::kTitleBGM, DX_PLAYTYPE_LOOP);

	// �t�F�[�h
	m_pFade = std::make_shared<Fade>();

	m_isNextSceneFlag = false;
}

shared_ptr<SceneBase> SceneTitle::Update()
{
	m_pFade->FadeIn(m_pFade->GatFadeInFlag());	// �t�F�[�h�C��

	if (m_isMenu) {					// �����ꂩ�̃{�^���������ꂽ��A���j���[��\������

		// �e���b�v�X�N���[��
		m_scrollX += kCaptionScroll;

		// ���̃V�[���I��
		SwitchingScene();

		// �t�F�[�h�A�E�g
		m_pFade->FadeOut(m_pFade->GatFadeOutFlag());
		m_isNextSceneFlag = m_pFade->GatNextSceneFlag();
		

		// �V�[���J��
		if (Pad::IsTrigger(PAD_INPUT_A)) {			// A�{�^��

			m_pSound->PlaySE(SoundManager::SE_Type::kButtonSE, DX_PLAYTYPE_BACK);	// SE
			m_pFade->SetFadeOutFlag(true);			// �t�F�[�h�A�E�g�t���O
		}

		if (m_isNextSceneFlag)						// ���̃V�[��
		{
			if (m_nextScene == nextScene::kGameScene)
			{
				return make_shared<SceneGame>();	// �Q�[���V�[���֍s��
			}
			if (m_nextScene == nextScene::kOptionScene)
			{
				return make_shared<SceneOption>();	// �I�v�V�����V�[���֍s��
			}
			if (m_nextScene == nextScene::kRankingScene)
			{
				return make_shared<SceneRanking>();	// �����L���O�V�[���֍s��
			}
			if (m_nextScene == nextScene::kGameEnd)
			{
				Effkseer_End();
				DxLib_End();						// �Q�[�����I������
			}
		}
	}
	else {
		m_isMenu = CheckHitKeyAll(DX_CHECKINPUT_PAD);		// �����ꂩ�̃{�^��������
	}

	return shared_from_this();
}

void SceneTitle::Draw()
{
	DrawExtendGraph(0, 0, 1920, 1080, m_bgGraph, true);		// �w�i
	DrawGraph(kTitlePosX, kTitlePosY, m_uiGraph[0], true);	// �^�C�g�����S

	if (!m_isMenu) {
		static int m_fadeAlpha;
		static bool isFade;

		if (isFade) {
			m_fadeAlpha += 3;

			if (m_fadeAlpha >= 255) {
				isFade = false;
			}
		}
		else {
			m_fadeAlpha -= 5;
			if (m_fadeAlpha <= 0) {
				isFade = true;
			}
		}

		// �t�F�[�h�C���E�t�F�[�h�A�E�g�`��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// �������ŕ\���J�n
		DrawGraph(kPressBgPosX, kPressBgPosY, m_uiGraph[1], true);
		DrawGraph(kPressEnyButtonPosX, kPressEnyButtonPosY, m_uiGraph[2], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// �s�����ɖ߂��Ă���	
	}
	else {
		DrawExtendGraph(kSelectBgPosX, kSelectBgPosY, kSelectBgPosX + 1500, kSelectBgPosY + 100, m_uiGraph[1], true);
		DrawGraph(kSelect1X, kSelectPosY, m_uiGraph[3], true);
		DrawGraph(kSelect2X, kSelectPosY + 2, m_uiGraph[4], true);
		DrawGraph(kSelect3X, kSelectPosY + 3, m_uiGraph[5], true);
		DrawGraph(kSelect4X, kSelectPosY, m_uiGraph[6], true);

		DrawExtendGraphF(c1.m_selectBox1.x - 20, c1.m_selectBox1.y - 1,
			c1.m_selectBox2.x + 20, c1.m_selectBox2.y + 1, m_uiGraph[12], true);

		DrawCaption();
	}

	m_pFade->Draw();
#ifdef _DEBUG
	DrawString(0, 0, "SceneTitle", 0xffffff);
	DrawFormatString(0, 700, 0xffffff, "m_nextScene=%d", m_nextScene);
#endif // DEBUG
}

void SceneTitle::End()
{
	DeleteGraph(m_bgGraph);

	// UI�摜�̍폜
	for (int i = 0; i < m_uiGraph.size(); i++) {
		DeleteGraph(m_uiGraph[i]);
	}

	SceneBase::End();
}

void SceneTitle::SwitchingScene()
{
	// ���L�[�������Ǝ��̃V�[���̕ύX������
	if (Pad::IsTrigger(PAD_INPUT_RIGHT))
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kSelectSE, DX_PLAYTYPE_BACK);

		m_scrollX = kCaptionPosX;

		if (m_nextScene == nextScene::kGameScene)
		{
			c1 = { kSelectPos[2],kSelectPos[3] };
			m_nextScene = nextScene::kOptionScene;
		}
		else if (m_nextScene == nextScene::kOptionScene)
		{
			c1 = { kSelectPos[4],kSelectPos[5] };
			m_nextScene = nextScene::kRankingScene;
		}
		else if (m_nextScene == nextScene::kRankingScene)
		{
			c1 = { kSelectPos[6],kSelectPos[7] };
			m_nextScene = nextScene::kGameEnd;
		}
		else if (m_nextScene == nextScene::kGameEnd)
		{
			c1 = { kSelectPos[0],kSelectPos[1] };
			m_nextScene = nextScene::kGameScene;
		}
	}

	// ��L�[�������Ǝ��̃V�[���̕ύX������
	if (Pad::IsTrigger(PAD_INPUT_LEFT))
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kSelectSE, DX_PLAYTYPE_BACK);

		m_scrollX = kCaptionPosX;

		if (m_nextScene == nextScene::kGameScene)
		{
			c1 = { kSelectPos[6],kSelectPos[7] };
			m_nextScene = nextScene::kGameEnd;
		}
		else if (m_nextScene == nextScene::kGameEnd)
		{
			c1 = { kSelectPos[4],kSelectPos[5] };
			m_nextScene = nextScene::kRankingScene;
		}
		else if (m_nextScene == nextScene::kRankingScene)
		{
			c1 = { kSelectPos[2],kSelectPos[3] };
			m_nextScene = nextScene::kOptionScene;
		}
		else if (m_nextScene == nextScene::kOptionScene)
		{
			c1 = { kSelectPos[0],kSelectPos[1] };
			m_nextScene = nextScene::kGameScene;
		}
	}
}

void SceneTitle::DrawCaption()
{
	// �w�i�摜�̃T�C�Y�擾
	Size expBgSize, expSize;
	GetGraphSize(m_uiGraph[7], &expBgSize.m_width, &expBgSize.m_height);
	GetGraphSize(m_uiGraph[8], &expSize.m_width, &expSize.m_height);

	// �X�N���[���ʂ��v�Z
	int scrollBg = static_cast<int>(m_scrollX) % static_cast<int>(expBgSize.m_width);

	//DrawGraph(0.0f,static_cast<int>(kCaptionBgPosY),m_uiGraph[7], true);

	// �������w�i�`��
	for (int index = 0; index < 4; index++)
	{
		DrawGraph(-100-scrollBg + index * expBgSize.m_width,
			static_cast<int>(kCaptionBgPosY),
			m_uiGraph[7], true);
	}

	static int next;
	// �������`��
	if (m_nextScene == nextScene::kGameScene) {
		next = 8;
	}
	if (m_nextScene == nextScene::kOptionScene) {
		next = 9;
	}
	if (m_nextScene == nextScene::kRankingScene) {
		next = 10;
	}
	if (m_nextScene == nextScene::kGameEnd) {
		next = 11;
	}

	for (int index = 0; index < 4; index++)
	{
		DrawGraph(-scrollBg + index * expSize.m_width,
			static_cast<int>(kCaptionPosY),
			m_uiGraph[next], true);
	}
}
