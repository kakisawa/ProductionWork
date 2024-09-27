#include "EffekseerForDXLib.h"
#include "DxLib.h"
#include "Vec2.h"
#include "Game.h"
#include "Input.h"
#include "UI.h"
#include "Sound.h"
#include "Font.h"
#include "Ranking.h"
#include "Player.h"
#include "Camera.h"
#include "Stage.h"
#include "SceneStage1.h"
#include "SceneStage2.h"
#include "SceneOption.h"
#include "SceneTitle.h"
#include "SceneSelectStage.h"

// �萔
namespace
{
	constexpr int kBackColor = 0xdddddd;					// �w�i�̐F
	constexpr int kBackBoxColor = 0x494949;					// �l�p�̐F
	constexpr int kBackBoxLTPos = 140;						// �l�p�̍���ʒu
	constexpr int kBackBoxWidth = 490;						// �l�p�̕�

	const Vec2 kSelectTextPos = { 260.0f, 300.0f };			// �I���e�L�X�g�\���ʒu
	constexpr float kSelectTextInterval = 120.0f;			// �I���e�L�X�g�\���Ԋu
	constexpr float kSelectTextAdj = 40.0f;					// �e�L�X�g�\���ʒu����
	const Vec2 kRankingTextPos = { 1000.0f, 250.0f };		// "�����L���O"�\���ʒu
	const Vec2 kIntroductionTextPos = { 900.0f, 770.0f };	// �X�e�[�W�Љ�e�L�X�g�ʒu
	constexpr float kIntroductionTextAdj = 30.0f;			// �X�e�[�W�Љ�e�L�X�g�ʒu����
	const Vec2 kCursorLTPos = { 140.0f, 290.0f };			// �J�[�\������ʒu
	constexpr int kTextColor = 0xffffff;					// �e�L�X�g�̐F

	constexpr int kStartFadeAlpha = 255;					// �X�^�[�g���̃t�F�[�h�l
	constexpr int kFadeFrame = 8;							// �t�F�[�h�ω���
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneSelectStage::SceneSelectStage()
{
	m_fadeAlpha = kStartFadeAlpha;
	m_select = SelectScene::kStage1;
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneSelectStage::~SceneSelectStage()
{
	StopSoundMem(Sound::m_bgmHandle[static_cast<int>(Sound::BgmKind::kStageSelect)]);
}


/// <summary>
/// ������
/// </summary>
void SceneSelectStage::Init()
{
	// �����L���O�擾
	if (m_select == SelectScene::kStage1)
	{
		m_pRank->GetRanking(StageKind::kStage1);
	}
	else if (m_select == SelectScene::kStage2)
	{
		m_pRank->GetRanking(StageKind::kStage2);
	}
}


/// <summary>
/// �X�V
/// </summary>
/// <param name="input">���͏���</param>
/// <returns>�J�ڐ�̃V�[��</returns>
std::shared_ptr<SceneBase> SceneSelectStage::Update(Input& input)
{
	FadeOut(kFadeFrame); // �t�F�[�h�A�E�g

	//�I����ԍX�V
	UpdateSelect(input, kSelectNum);
	m_pUI->Update();

	// �����L���O���Ď擾
	if (input.IsTriggered("up") || input.IsTriggered("down"))
	{
		if (m_select == SelectScene::kStage1 || m_select == SelectScene::kStage2)
		{
			Init();
		}
	}

	// BGM��炷
	if (!CheckSoundMem(Sound::m_bgmHandle[static_cast<int>(Sound::BgmKind::kStageSelect)]))
	{
		PlaySoundMem(Sound::m_bgmHandle[static_cast<int>(Sound::BgmKind::kStageSelect)], DX_PLAYTYPE_LOOP);
	}

	// �V�[���J��
	if (input.IsTriggered("OK"))
	{
		PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kSelect)], DX_PLAYTYPE_BACK); // SE��炷

		std::shared_ptr<Player> pPlayer = std::make_shared<Player>();
		std::shared_ptr<Camera> pCamera = std::make_shared<Camera>();
		std::shared_ptr<Stage> pStage = std::make_shared<Stage>();

		if (m_select == SelectScene::kStage1)
		{
			FadeIn(kFadeFrame); // �t�F�[�h�C��
			return std::make_shared<SceneStage1>(pPlayer, pCamera, pStage);
		}
		else if (m_select == SelectScene::kStage2)
		{
			FadeIn(kFadeFrame); // �t�F�[�h�C��
			return std::make_shared<SceneStage2>(pPlayer, pCamera, pStage);
		}
		else if (m_select == kOption)
		{
			FadeIn(kFadeFrame); // �t�F�[�h�C��
			return std::make_shared<SceneOption>(shared_from_this());
		}
		else if (m_select == kEnd)
		{
			Effkseer_End();	// Effekseer�̏I������
			DxLib_End();
		}
	}
	if (input.IsTriggered("back"))
	{
		FadeIn(kFadeFrame); // �t�F�[�h�C��
		return std::make_shared<SceneTitle>();
	}

	return shared_from_this();
}


/// <summary>
/// �`��
/// </summary>
void SceneSelectStage::Draw()
{
	// �w�i�`��
	m_pUI->DrawMenuBg();
	// �J�[�\���\��
	m_pUI->DrawCursor(kCursorLTPos, m_select, kSelectTextInterval);
	m_pUI->DrawButtonText();

	// �e�L�X�g�\��
	DrawStringFToHandle(kSelectTextPos.x, kSelectTextPos.y + kSelectTextInterval * SelectScene::kStage1,
		"���K�։���", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kSelectStage)]);
	DrawStringFToHandle(kSelectTextPos.x, kSelectTextPos.y + kSelectTextInterval * SelectScene::kStage2,
		"�����։���", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kSelectStage)]);
	DrawStringFToHandle(kSelectTextPos.x, kSelectTextPos.y + kSelectTextInterval * SelectScene::kOption,
		"�I�v�V����", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kSelectStage)]);
	DrawStringFToHandle(kSelectTextPos.x - kSelectTextAdj, kSelectTextPos.y + kSelectTextInterval * SelectScene::kEnd,
		"�Q�[�����I���", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kSelectStage)]);

	// �����L���O�\��
	if (m_select == SelectScene::kStage1 || m_select == SelectScene::kStage2)
	{
		DrawStringFToHandle(kRankingTextPos.x, kRankingTextPos.y,
			"�I�����C�������L���O", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kRankingText)]);
		m_pRank->DrawStageSelectRanking();
	}

	//�X�e�[�W�̏Љ��\������
	DrawIntroductionText();

	DrawFade();	// �t�F�[�h�C���A�E�g�`��

#ifdef _DEBUG	// �f�o�b�O�\��
	// ���݂̃V�[��
	DrawString(0, 0, "�X�e�[�W�I�����", 0xffffff);
#endif
}


/// <summary>
/// �X�e�[�W�̏Љ��\������
/// </summary>
void SceneSelectStage::DrawIntroductionText()
{
	if (m_select == SelectScene::kStage1)
	{
		DrawStringFToHandle(kIntroductionTextPos.x, kIntroductionTextPos.y,
			"������@�̐������s���܂��B", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kIntroductionText)]);
	}
	else if (m_select == SelectScene::kStage2)
	{
		DrawStringFToHandle(kIntroductionTextPos.x, kIntroductionTextPos.y - kIntroductionTextAdj,
			"3�A������������D���ł��B\n�D���߂����Ċ撣���Ă�������!", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kIntroductionText)]);
	}
	else if (m_select == SelectScene::kOption)
	{
		DrawStringFToHandle(kIntroductionTextPos.x, kIntroductionTextPos.y,
			"���ʂ��ʃT�C�Y�̐ݒ���s���܂��B", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kIntroductionText)]);
	}
	else if (m_select == SelectScene::kEnd)
	{
		DrawStringFToHandle(kIntroductionTextPos.x, kIntroductionTextPos.y,
			"�Q�[�����I�����܂��B", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kIntroductionText)]);
	}
}


