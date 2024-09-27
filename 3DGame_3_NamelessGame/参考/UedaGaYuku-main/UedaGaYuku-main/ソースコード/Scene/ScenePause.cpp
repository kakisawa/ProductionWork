#include "DxLib.h"
#include "Vec2.h"
#include "Input.h"
#include "Game.h"
#include "UI.h"
#include "Font.h"
#include "ScenePause.h"
#include "SceneOption.h"
#include "SceneSelectStage.h"

// �萔
namespace
{
	constexpr int kAlpha = 200;
	constexpr int kBackColor = 0x1a1a1a;				// �w�i�̐F
	constexpr int kBackBoxColor = 0x494949;				// �l�p�̐F
	constexpr int kTextColor = 0xffffff;				// �e�L�X�g�̐F
	const Vec2 kBackTextPos = { 800, 350 };				// "�Q�[���ɂ��ǂ�"�\���ʒu
	const Vec2 kOptionTextPos = { 860, 500 };			// "�I�v�V����"�\���ʒu
	const Vec2 kStageTextPos = { 760, 650 };			// "�X�e�[�W�I���ɂ��ǂ�"�\���ʒu
	constexpr float kSelectTextInterval = 150.0f;		// �I���e�L�X�g�\���Ԋu
	const Vec2 kCursorPos = { 740, 340 };				// �J�[�\���\���ʒu
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="pScene">�O�Ɏ��s���Ă����V�[��</param>
ScenePause::ScenePause(std::shared_ptr<SceneBase> pScene)
{
	m_select = Select::kBack;
	m_pPrevScene = pScene;
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
ScenePause::~ScenePause()
{
}


/// <summary>
/// ������
/// </summary>
void ScenePause::Init()
{
}


/// <summary>
/// �X�V
/// </summary>
/// <param name="input">���͏��</param>
/// <returns>�J�ڐ�̃|�C���^</returns>
std::shared_ptr<SceneBase> ScenePause::Update(Input& input)
{
	// �I����Ԃ��X�V
	UpdateSelect(input, Select::kSelectNum);
	m_pUI->Update();

	if (input.IsTriggered("OK"))
	{
		if (m_select == Select::kBack)
		{
			return m_pPrevScene;	// �Q�[����ʂɂ��ǂ�
		}
		else if (m_select == Select::kOption)
		{
			return std::make_shared<SceneOption>(shared_from_this());	// �I�v�V������ʂɑJ��
		}
		else if (m_select == Select::kStageSelect)
		{
			return std::make_shared<SceneSelectStage>(); // �X�e�[�W�I����ʂɑJ��
		}
	}
	else if (input.IsTriggered("pause"))
	{
		return m_pPrevScene;	// �Q�[����ʂɂ��ǂ�
	}

	return shared_from_this();	// ���g��shared_ptr��Ԃ�
}


/// <summary>
/// �`��
/// </summary>
void ScenePause::Draw()
{
	// �v���C��ʂ𔖂��\������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAlpha);
	m_pPrevScene->Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �w�i�\��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAlpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, kBackColor, true);
	m_pUI->DrawPauseBack();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �J�[�\���\��
	m_pUI->DrawCursor(kCursorPos, m_select, kSelectTextInterval);

	// �e�L�X�g�\��
	DrawStringFToHandle(kBackTextPos.x, kBackTextPos.y,
		"�Q�[���ɂ��ǂ�", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kPause)]);
	DrawStringFToHandle(kOptionTextPos.x, kOptionTextPos.y,
		"�I�v�V����", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kPause)]);
	DrawStringFToHandle(kStageTextPos.x, kStageTextPos.y,
		"�X�e�[�W�I���ɂ��ǂ�", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kPause)]);

#ifdef _DEBUG	// �f�o�b�O�\��
	// ���݂̃V�[��
	DrawString(0, 0, "�|�[�Y���", 0xffffff);
	// ���S��
	//DrawLine(Game::kScreenWidth * 0.5, 0, Game::kScreenWidth * 0.5, Game::kScreenHeight, 0x0000ff);
#endif
}
