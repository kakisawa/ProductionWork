#include "DxLib.h"
#include "Player.h"
#include "Stage.h"
#include "UIBattle.h"
#include "EffectManager.h"
#include "LoadData.h"
#include "DebugDraw.h"
#include "EnemyTuto.h"

// �萔
namespace
{
	// �G���
	const char* const kfileName = "data/Model/Chara/EnemyTuto.mv1";	// �G�̃t�@�C����
	constexpr float kScale = 0.3f;									// �g�嗦
	constexpr int kIntervalTime = 120;								// ��Ԃ��X�V����܂ł̎���
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
EnemyTuto::EnemyTuto()
{
	// �L�����N�^�[����ǂݍ���
	m_pLoadData = std::make_shared<LoadData>(*this, static_cast<int>(CharaType::kEnemyTuto));
	m_enemyType = static_cast<int>(CharaType::kEnemyTuto);
	m_pUIBattle = std::make_shared<UIBattle>(m_status.maxHp, m_enemyType);
	m_hp = m_status.maxHp;
	m_moveSpeed = m_status.maxMoveSpeed;
	m_isAttack = false;
	m_isMove = false;
	m_modelHandle = MV1LoadModel(kfileName);
	MV1SetScale(m_modelHandle, VGet(kScale, kScale, kScale));
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
EnemyTuto::~EnemyTuto()
{
	MV1DeleteModel(m_modelHandle);
}


/// <summary>
/// ������
/// </summary>
/// <param name="pos">�����ʒu</param>
void EnemyTuto::Init(std::shared_ptr<EffectManager> pEffect, VECTOR pos)
{
	m_pos = pos;
	m_pEffect = pEffect;
	MV1SetPosition(m_modelHandle, m_pos);
	m_pEffect->Init();	// �G�t�F�N�g�̏�����
}


/// <summary>
/// �X�V
/// </summary>
/// <param name="player">�v���C���[�Q��</param>
/// <param name="stage">�X�e�[�W�Q��</param>
void EnemyTuto::Update(Player& player, Stage& stage, SceneStageBase& sceneStage)
{
	EnemyBase::Update(player, stage, sceneStage);
	m_pUIBattle->UpdateHpBar();		// HP�o�[�̍X�V
}


/// <summary>
/// �`��
/// </summary>
void EnemyTuto::Draw()
{
	EnemyBase::Draw();
}


/// <summary>
/// UI�`��
/// </summary>
void EnemyTuto::DrawUi()
{
	m_pUIBattle->DrawEnemySilhouette(static_cast<int>(CharacterBase::CharaType::kEnemyTuto)); // �V���G�b�g�`��
	m_pUIBattle->DrawEnemyName(static_cast<int>(CharacterBase::CharaType::kEnemyTuto));		  // �G�̖��O�Q�[�W�\��
	m_pUIBattle->DrawEnemyHp(m_hp);	// HP�Q�[�W��\��
}


/// <summary>
/// �_���[�W���󂯂��ۂ̏���
/// </summary>
/// <param name="damage">�_���[�W��</param>
void EnemyTuto::OnDamage(float damage)
{
	EnemyBase::OnDamage(damage);

	// �K�[�h��Ԃ̏ꍇ
	if (m_currentState == CharacterBase::State::kGuard)
	{
		OffGuard();
	}
}
