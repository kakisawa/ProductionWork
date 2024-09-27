#include "DxLib.h"
#include "Input.h"
#include "Game.h"
#include "UI.h"
#include "UIBattle.h"
#include "EffectManager.h"
#include "Sound.h"
#include "Player.h"
#include "EnemyBase.h"
#include "Camera.h"
#include "Stage.h"
#include "SceneClear.h"
#include "SceneGameover.h"
#include "SceneStageBase.h"

namespace
{
	const VECTOR kPlayerInitPos = VGet(2600.0f, 69.0f, 4240.0f);  // �v���C���[�̏����ʒu
	const VECTOR kEnemyInitPos = VGet(2660, 69.0f, 4280.0f);	  // �G�̏����ʒu
	constexpr int kChangeColorTime = 220;						  // ��ʂ̕\����ύX���鎞��
	constexpr int kClearProductionTime = 240;					  // �N���A���o�̎���
	constexpr int kGameoverProductionTime = 240;				  // �Q�[���I�[�o�[���o�̎���
	constexpr int kNextBattleTime = 360;						  // ���̎������n�܂�܂ł̎���
	constexpr int kLastAttackSoundTime = 20;					  // �Ō�̍U��SE��炷����
	constexpr int kBattleEndSoundTime = 60;						  // �N���A�̃R���O��SE��炷����
	constexpr int kMULAPal = 240;								  // ��Z�u�����h�l
	constexpr int kAddPal = 80;									  // ���Z�u�����h�l
	
	/*�e*/
	constexpr int kShadowMapSize = 4096;							  // �쐬����V���h�E�}�b�v�̃T�C�Y
	const VECTOR kShadowAreaMinPos = VGet(2200.0f, 65.0f, 3800.0f);	  // �V���h�E�}�b�v�ɕ`�悷��ŏ��͈�
	const VECTOR kShadowAreaMaxPos = VGet(3000.0f, 100.0f, 5000.0f);  // �V���h�E�}�b�v�ɕ`�悷��ő�͈�

	constexpr int kStartFadeAlpha = 255; // �X�^�[�g���̃t�F�[�h�l
	constexpr int kFadeFrame = 8;		 // �t�F�[�h�ω���
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneStageBase::SceneStageBase() :
	m_battleNum(0),
	m_nextBattleTime(kNextBattleTime),
	m_clearProductionTime(kClearProductionTime),
	m_gameoverProductionTime(kGameoverProductionTime),
	m_elapsedTime(0),
	m_isPause(false)
{
	//m_pLight->Create(m_pPlayer);
	m_shadowMap = MakeShadowMap(kShadowMapSize, kShadowMapSize);
	// �V���h�E�}�b�v���z�肷�郉�C�g�̕������Z�b�g
	SetShadowMapLightDirection(m_shadowMap, GetLightPosition());
	// �V���h�E�}�b�v�ɕ`�悷��͈͂�ݒ�
	SetShadowMapDrawArea(m_shadowMap, kShadowAreaMinPos, kShadowAreaMaxPos);

	m_fadeAlpha = kStartFadeAlpha;
	m_pEffect = std::make_shared<EffectManager>();
	m_clearBackHandle = LoadGraph("data/UI/clearBack.png");
}


/// <summary>
/// �������R���X�g���N�^
/// </summary>
/// <param name="pPlayer">�v���C���[�Q��</param>
/// <param name="pCamera">�J�����Q��</param>
/// <param name="pStage">�X�e�[�W�Q��</param>
SceneStageBase::SceneStageBase(std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera, std::shared_ptr<Stage> pStage):
	m_pPlayer(pPlayer),
	m_pCamera(pCamera),
	m_pStage(pStage),
	m_pEnemy(nullptr),
	m_pUIBattle(nullptr),
	m_battleNum(0),
	m_clearProductionTime(0),
	m_gameoverProductionTime(0),
	m_nextBattleTime(0),
	m_elapsedTime(0),
	m_isPause(false),
	m_shadowMap(-1),
	m_clearBackHandle(-1)
{
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneStageBase::~SceneStageBase()
{
	DeleteShadowMap(m_shadowMap); // �V���h�E�}�b�v�̍폜
	DeleteGraph(m_clearBackHandle);
}


/// <summary>
/// ������
/// </summary>
void SceneStageBase::Init()
{
	if (!m_isPause)
	{
		m_pPlayer->Init(m_pEffect, kPlayerInitPos);
		m_pCamera->Init();
		m_pEnemy->Init(m_pEffect, kEnemyInitPos);
	}
	m_isPause = false;
}


/// <summary>
/// �`��
/// </summary>
void SceneStageBase::Draw()
{
	ShadowMap_DrawSetup(m_shadowMap); // �V���h�E�}�b�v�ւ̕`��̏���
	m_pStage->Draw();				  // �X�e�[�W�`��
	m_pPlayer->Draw();				  // �v���C���[�`��
	m_pEnemy->Draw();				  // �G�`��
	ShadowMap_DrawEnd();			  // �V���h�E�}�b�v�ւ̕`����I��

	SetUseShadowMap(0, m_shadowMap); // �`��Ɏg�p����V���h�E�}�b�v��ݒ�
	m_pStage->Draw();				 // �X�e�[�W�`��
	m_pPlayer->Draw();				 // �v���C���[�`��
	m_pEnemy->Draw();				 // �G�`��
	SetUseShadowMap(0, -1);			 // �`��Ɏg�p����V���h�E�}�b�v�̐ݒ������
	m_pEnemy->DrawUi();				 // �G��UI�`��
	m_pUIBattle->DrawOperation();	 // ���������\��

	// �N���A���o�\��
	const bool isClearProduction = m_clearProductionTime < kClearProductionTime && m_clearProductionTime >= 0;
	if (isClearProduction)
	{
		m_pPlayer->SetIsClearProduction(true);
		m_pUIBattle->DrawClearProduction(m_clearProductionTime);
	}
	const bool isGameoverProduction = m_gameoverProductionTime < kGameoverProductionTime && m_gameoverProductionTime >= 0;
	// �Q�[���I�[�o�[���o�\��
	if (isGameoverProduction)
	{
		m_pPlayer->SetIsGameoverProduction(true);
		m_pUIBattle->DrawGameoverProduciton();
	}

	DrawFade();	// �t�F�[�h�C���A�E�g�`��

#ifdef _DEBUG
	//TestDrawShadowMap(m_shadowMap, 0, 0, 320, 240); // ��ʍ���ɃV���h�E�}�b�v���e�X�g�`��
	DrawFormatString(0, 180, 0xffffff, "�o�ߎ���:%d", m_elapsedTime);
#endif
}


/// <summary>
/// �N���A���o���s��
/// </summary>
void SceneStageBase::ClearProduction()
{
	// SE��炷
	if (m_clearProductionTime >= kClearProductionTime && m_clearProductionTime >= kClearProductionTime - kLastAttackSoundTime)
	{
		if (!CheckSoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kLastAttack)]))
		{
			PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kLastAttack)], DX_PLAYTYPE_BACK);
		}
	}
	else if (m_clearProductionTime >= kClearProductionTime - kBattleEndSoundTime)
	{
		if (!CheckSoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kBattleEnd)]))
		{
			PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kBattleEnd)], DX_PLAYTYPE_BACK);
		}
	}
	else
	{
		if (!CheckSoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kClearCheers)]))
		{
			PlaySoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kClearCheers)], DX_PLAYTYPE_BACK);
		}
	}

	if (m_clearProductionTime >= kClearProductionTime - kChangeColorTime)
	{
		m_clearProductionTime--;
		return;
	}

	// �N���A���o�����Z�b�g
	m_clearProductionTime = 0;
	m_pUIBattle->ResetClearProduction();
	StopSoundMem(Sound::m_seHandle[static_cast<int>(Sound::SeKind::kClearCheers)]);
}


/// <summary>
/// �G��|���Ď��������n�܂�O�̏���
/// </summary>
void SceneStageBase::UpdateNextBattle()
{
	m_nextBattleTime = kNextBattleTime;
	m_clearProductionTime = kClearProductionTime;
	m_pUIBattle->ResetStartProduction();
	m_pUIBattle->SetEnemyKind(m_pEnemy->GetEnemyType());
	// �v���C���[�̈ʒu�A�J�����ʒu���ŏ��̏�Ԃɖ߂�
	m_pPlayer->Recovery();
	Init();
	m_pPlayer->SetIsStartProduction(true);
	FadeIn(kFadeFrame); // �t�F�[�h�C��
}


/// <summary>
/// �Q�[���I�[�o�[���̉��o
/// </summary>
void SceneStageBase::GameoverProduction()
{
	if (m_gameoverProductionTime >= 0)
	{
		m_gameoverProductionTime--;
	}
}
