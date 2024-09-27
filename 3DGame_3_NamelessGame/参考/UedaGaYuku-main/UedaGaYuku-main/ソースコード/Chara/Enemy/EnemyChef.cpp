#include "DxLib.h"
#include "Player.h"
#include "Stage.h"
#include "UIBattle.h"
#include "EffectManager.h"
#include "LoadData.h"
#include "EnemyChef.h"

// �萔
namespace
{
	// �G���
	const char* const kfileName = "data/Model/Chara/Chef.mv1";	// �G�̃t�@�C����
	constexpr float kScale = 0.3f;								// �g�嗦
	constexpr int kIntervalTime = 120;							// ��Ԃ��X�V����܂ł̎���
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
EnemyChef::EnemyChef()
{
	// �L�����N�^�[����ǂݍ���
	m_pLoadData = std::make_shared<LoadData>(*this, static_cast<int>(CharaType::kEnemyChef));
	m_enemyType = static_cast<int>(CharaType::kEnemyChef);
	m_pUIBattle = std::make_shared<UIBattle>(m_status.maxHp, m_enemyType);
	m_hp = m_status.maxHp;
	m_moveSpeed = m_status.maxMoveSpeed;
	m_modelHandle = MV1LoadModel(kfileName);
	MV1SetScale(m_modelHandle, VGet(kScale, kScale, kScale));
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
EnemyChef::~EnemyChef()
{
	MV1DeleteModel(m_modelHandle);
}


/// <summary>
/// ������
/// </summary>
/// <param name="pos">�����ʒu</param>
void EnemyChef::Init(std::shared_ptr<EffectManager> pEffect, VECTOR pos)
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
void EnemyChef::Update(Player& player, Stage& stage, SceneStageBase& sceneStage)
{
	EnemyBase::Update(player, stage, sceneStage);
	m_pUIBattle->UpdateHpBar();		// HP�o�[�̍X�V
}


/// <summary>
/// �`��
/// </summary>
void EnemyChef::Draw()
{
	EnemyBase::Draw();
}


/// <summary>
/// UI�\��
/// </summary>
void EnemyChef::DrawUi()
{
	m_pUIBattle->DrawEnemySilhouette(static_cast<int>(CharacterBase::CharaType::kEnemyChef)); // �V���G�b�g�`��
	m_pUIBattle->DrawEnemyName(static_cast<int>(CharacterBase::CharaType::kEnemyChef));		  // �G�̖��O�Q�[�W�\��
	m_pUIBattle->DrawEnemyHp(m_hp);		// HP�Q�[�W��\��
}
