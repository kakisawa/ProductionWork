#include "DxLib.h"
#include "Player.h"
#include "Stage.h"
#include "UIBattle.h"
#include "EffectManager.h"
#include "LoadData.h"
#include "DebugDraw.h"
#include "EnemyNinja.h"

// �萔
namespace
{
	// �G���
	const char* const kfileName = "data/Model/Chara/Ninja.mv1";	// �G�̃t�@�C����
	constexpr float kScale = 0.3f;								// �g�嗦
	constexpr int kIntervalTime = 180;							// ��Ԃ��X�V����܂ł̎���
	const VECTOR kInitPos = VGet(2660, 69.0f, 4280.0f);			// �����ʒu
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
EnemyNinja::EnemyNinja()
{
	// �L�����N�^�[����ǂݍ���
	m_pLoadData = std::make_shared<LoadData>(*this, static_cast<int>(CharaType::kEnemyNinja));
	m_enemyType = static_cast<int>(CharaType::kEnemyNinja);
	m_pUIBattle = std::make_shared<UIBattle>(m_status.maxHp, m_enemyType);
	m_hp = m_status.maxHp;
	m_pos = kInitPos;
	m_moveSpeed = m_status.maxMoveSpeed;
	m_modelHandle = MV1LoadModel(kfileName);
	MV1SetScale(m_modelHandle, VGet(kScale, kScale, kScale));
	MV1SetPosition(m_modelHandle, kInitPos);
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
EnemyNinja::~EnemyNinja()
{
	MV1DeleteModel(m_modelHandle);
}


/// <summary>
///  ������
/// </summary>
/// <param name="pos">�����ʒu</param>
void EnemyNinja::Init(std::shared_ptr<EffectManager> pEffect, VECTOR pos)
{
	m_pos = pos;
	m_pEffect = pEffect;
	MV1SetPosition(m_modelHandle, m_pos);
	m_pEffect->Init();	// �G�t�F�N�g�̏�����
	//m_pUIBattle->SetEnemyKind(static_cast<int>(CharaType::kEnemyNinja));
}


/// <summary>
/// �X�V
/// </summary>
/// <param name="player">�v���C���[�Q��</param>
/// <param name="stage">�X�e�[�W�Q��</param>
void EnemyNinja::Update(Player& player, Stage& stage, SceneStageBase& sceneStage)
{
	EnemyBase::Update(player, stage, sceneStage);
	m_pUIBattle->UpdateHpBar();		// HP�o�[�̍X�V
}


/// <summary>
/// �`��
/// </summary>
void EnemyNinja::Draw()
{
	EnemyBase::Draw();
}


/// <summary>
/// UI�`��
/// </summary>
void EnemyNinja::DrawUi()
{
	m_pUIBattle->DrawEnemySilhouette(static_cast<int>(CharacterBase::CharaType::kEnemyNinja)); // �V���G�b�g�`��
	m_pUIBattle->DrawEnemyName(static_cast<int>(CharacterBase::CharaType::kEnemyNinja));	   // �G�̖��O�Q�[�W�\��
	m_pUIBattle->DrawEnemyHp(m_hp);	// HP�Q�[�W��\��
}
