#include "DxLib.h"
#include "Vec2.h"
#include "Input.h"
#include "Sound.h"
#include "Font.h"
#include "UIBattle.h"
#include "Player.h"
#include "EnemyTuto.h"
#include "Camera.h"
#include "Stage.h"
#include "ScenePause.h"
#include "SceneClear.h"
#include "SceneGameover.h"
#include "SceneStage1.h"

namespace
{
	constexpr int kMaxBattleNum = 1;		// �ő�o�g����
	constexpr int kStartSeTime = 60;		// �X�^�[�gSE���Đ��\�����n�߂鎞��
	constexpr int kFadeFrame = 4;			// �t�F�[�h�ω���
	constexpr int kGameoverFadeFrame = 1;	// �Q�[���I�[�o�[���̃t�F�[�h�ω���

	/*�`���[�g���A��*/
	// �w�i�摜�̃p�X
	const Vec2 kTutoTextBoxPos = { 425.0f , 650.0f }; // �e�L�X�g�{�b�N�X�ʒu
	const Vec2 kTutoTextPos = { 480.0f , 690.0f };	  // �e�L�X�g�ʒu
	constexpr int kTextColor = 0xffffff;			  // �e�L�X�g�̐F
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneStage1::SceneStage1(std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera, std::shared_ptr<Stage> pStage):
	m_tutoNum(0),
	m_isTuto(false)
{
	m_pPlayer = pPlayer;
	m_pCamera = pCamera;
	m_pStage = pStage;
	m_pEnemy = std::make_shared<EnemyTuto>();
	m_pUIBattle = std::make_shared<UIBattle>(0.0f, m_pEnemy->GetEnemyType());
	m_battleNum = 0;

	m_tutoHandle.resize(TutoHandle::kTutoHandleNum);
	for (int i = 0; i < m_tutoHandle.size(); i++)
	{
		if (i == TutoHandle::kTuto1 || i == TutoHandle::kTuto2)
		{
			m_tutoHandle[i] = LoadGraph("data/UI/Tutorial/pGauge.png");
		}
		else if (i == TutoHandle::kTuto3)
		{
			m_tutoHandle[i] = LoadGraph("data/UI/Tutorial/eGauge.png");
		}
		else if (i == TutoHandle::kTuto4)
		{
			m_tutoHandle[i] = LoadGraph("data/UI/Tutorial/ope.png");
		}
		else if (i == TutoHandle::kTextBox)
		{
			m_tutoHandle[i] = LoadGraph("data/UI/Tutorial/textBox.png");
		}
		else
		{
			m_tutoHandle[i] = LoadGraph("data/UI/Tutorial/def.png");
		}
	}
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneStage1::~SceneStage1()
{
	StopSoundMem(Sound::m_bgmHandle[static_cast<int>(Sound::BgmKind::kStage1)]);
	for (auto& handle : m_tutoHandle)
	{
		DeleteGraph(handle);
	}
}


/// <summary>
/// ������
/// </summary>
void SceneStage1::Init()
{
	SceneStageBase::Init();
}


/// <summary>
/// �X�V
/// </summary>
/// <param name="input">���͏��</param>
/// <returns>�J�ڂ���V�[��</returns>
std::shared_ptr<SceneBase> SceneStage1::Update(Input& input)
{
#ifdef _DEBUG	// �f�o�b�O���[�h
	// P�L�[�Ń|�[�Y�A�|�[�Y����P�ŃR�}����
	if (m_debugState == DebugState::Normal && input.IsTriggered("debug_pause"))
	{
		m_debugState = DebugState::Pause;
	}
	if (m_debugState == DebugState::Pause && input.IsTriggered("debug_enter"))
	{
		m_debugState = DebugState::Normal;
	}
	if (m_debugState != DebugState::Pause || input.IsTriggered("debug_pause"))
#endif
	{
		FadeOut(kFadeFrame);	// �t�F�[�h�A�E�g
		UpdateSound();			// �T�E���h�X�V

		// �|�[�Y��ʂ��J��
		if (input.IsTriggered("pause"))
		{
			m_isPause = true;
			return std::make_shared<ScenePause>(shared_from_this());
		}

		// �`���[�g���A���\�����͓����Ȃ��悤�ɂ���
		if (!m_isTuto)
		{
			m_pCamera->StartProduction();
			m_pCamera->Update(input, *m_pPlayer);
			m_pPlayer->Update(input, *m_pCamera, *m_pEnemy, *m_pStage);
			m_pEnemy->Update(*m_pPlayer, *m_pStage, *this);
		}

		UpdateTuto(input);	// �`���[�g���A���̕\����Ԃ��X�V����
		if (m_isTuto) return shared_from_this();

		// �X�^�[�g���o�̍X�V
		m_nextBattleTime--;
		if (m_nextBattleTime > 0)
		{
			return shared_from_this();
		}
		else
		{
			m_pPlayer->SetIsStartProduction(false);
		}

		// �G��HP��0�ɂȂ����ꍇ
		if (m_pEnemy->GetHp() <= 0)
		{
			// �N���A���o���s��
			ClearProduction();

			// �N���A���o���I������玟�̃o�g���Ɉڍs����
			if (m_clearProductionTime <= 0)
			{
				UpdateNextBattle();
				FadeIn(kFadeFrame); // �t�F�[�h�C��
				m_clearTime.push_back(m_elapsedTime);
				return std::make_shared<SceneClear>(StageKind::kStage1, m_clearTime);
			}
		}
		// �v���C���[��HP��0�ɂȂ����ꍇ
		else if (m_pPlayer->GetHp() <= 0)
		{
			if (m_gameoverProductionTime > 0)
			{
				GameoverProduction();
			}
			else
			{
				FadeIn(kGameoverFadeFrame); // �t�F�[�h�C��
				StopSoundMem(Sound::m_bgmHandle[static_cast<int>(Sound::BgmKind::kStage1)]);
				return std::make_shared<SceneGameover>(shared_from_this());
			}
		}
		else
		{
			m_elapsedTime++; // �o�ߎ��Ԃ�i�߂�
		}
	}

#ifdef _DEBUG
	// �G��HP��0�ɂ���
	m_pEnemy->DebugDamage(input);
	// �V�[���J��
	if (input.IsTriggered("debug_clear"))
	{
		return std::make_shared<SceneClear>(StageKind::kStage1, m_clearTime);
	}
	else if (m_pPlayer->GetHp() <= 0.0f || input.IsTriggered("debug_gameover"))
	{
		if (m_gameoverProductionTime > 0)
		{
			GameoverProduction();
		}
		else
		{
			FadeIn(kGameoverFadeFrame); // �t�F�[�h�C��
			StopSoundMem(Sound::m_bgmHandle[static_cast<int>(Sound::BgmKind::kStage1)]);
			return std::make_shared<SceneGameover>(shared_from_this());
		}
	}
#endif

	return shared_from_this();	// ���g��shared_ptr��Ԃ�
}


/// <summary>
/// �`��
/// </summary>
void SceneStage1::Draw()
{
	SceneStageBase::Draw();

	if (m_tutoNum < TutoHandle::kTutoNum)
	{
		DrawTutorial(); // �`���[�g���A���\��
	}
	else
	{
		m_pUIBattle->DrawStartProduction(m_nextBattleTime, m_battleNum, kMaxBattleNum); // ���oUI��\��
	}

#ifdef _DEBUG	// �f�o�b�O�\��
	// ���݂̃V�[��
	DrawString(0, 0, "�X�e�[�W1", 0xffffff);
#endif
}


/// <summary>
/// �T�E���h�X�V
/// </summary>
void SceneStage1::UpdateSound()
{
	if (m_nextBattleTime < kStartSeTime && m_nextBattleTime > 0)
	{
		// �J�n����1�x����SE�𗬂�
		if (!CheckSoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kBattleStart)]))
		{
			PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kBattleStart)], DX_PLAYTYPE_BACK);
		}
	}
	else
	{
		// BGM��炷
		if (!CheckSoundMem(Sound::m_bgmHandle[static_cast<int>(Sound::BgmKind::kStage1)]))
		{
			PlaySoundMem(Sound::m_bgmHandle[static_cast<int>(Sound::BgmKind::kStage1)], DX_PLAYTYPE_LOOP);
		}
	}
}


/// <summary>
/// �`���[�g���A���̕\����Ԃ��X�V����
/// </summary>
/// <param name="input">���͏��</param>
void SceneStage1::UpdateTuto(Input& input)
{
	if (m_tutoNum < TutoHandle::kTutoNum)
	{
		m_isTuto = true;
	}

	if (m_isTuto)
	{
		if (m_tutoNum >= TutoHandle::kTutoNum)
		{
			m_isTuto = false;
		}

		if (input.IsTriggered("A"))
		{
			m_tutoNum++;	// �`���[�g���A���̕\����i�߂�
		}
	}
}


/// <summary>
/// �`���[�g���A���\��
/// </summary>
void SceneStage1::DrawTutorial()
{
	// �w�i�\��
	DrawGraph(0, 0, m_tutoHandle[m_tutoNum], true);

	// �e�L�X�g�{�b�N�X�\��
	DrawGraphF(kTutoTextBoxPos.x, kTutoTextBoxPos.y, m_tutoHandle[TutoHandle::kTextBox], true);

	// �{�^���\��
	m_pUIBattle->DrawTutoButtonText();

	// �e�L�X�g�\��
	if (m_tutoNum == TutoHandle::kTuto0)		m_tutoText = "�����ł͊�{�I�ȃ��[���Ȃǂɂ��Đ������邼�I";
	else if (m_tutoNum == TutoHandle::kTuto1)	m_tutoText = "����̐Ԃ��Q�[�W���v���C���[��HP���B\n���̉��̐��o�[���A�v���C���[�̃Q�[�W���B";
	else if (m_tutoNum == TutoHandle::kTuto2)	m_tutoText = "�Q�[�W�͍U�����s�����Ƃŗ��܂��Ă������B\n�Q�[�W�����܂�ƕK�E�Z�������ł��邼�I";
	else if (m_tutoNum == TutoHandle::kTuto3)	m_tutoText = "�E���̐Ԃ��Q�[�W���G��HP���B\n�G��HP��0�ɂ����珟�����I";
	else if (m_tutoNum == TutoHandle::kTuto4)	m_tutoText = "������@�ɂ��Ă͉E���ɏ����Ă��邩��m�F���Ă���I";
	else if (m_tutoNum == TutoHandle::kTuto5)	m_tutoText = "�����ɂ��Ă͈ȏゾ�I\n�l�`��p�ӂ���������ۂɓ����Ă݂悤�I";

	DrawStringFToHandle(kTutoTextPos.x, kTutoTextPos.y, m_tutoText.c_str(), kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kTutorial)]);
}