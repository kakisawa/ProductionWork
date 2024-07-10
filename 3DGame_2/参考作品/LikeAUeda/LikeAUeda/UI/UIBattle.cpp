#include "DxLib.h"
#include "UIBattle.h"

// �萔
namespace
{
	// �v���C���[UI
	constexpr float kpHpDispL = 30.0f;		// HP�o�[���\���ʒu
	constexpr float kpHpDispT = 40.0f;		// HP�o�[��\���ʒu
	constexpr float kpHpWidth = 400.0f;		// HP�o�[�̉���
	constexpr float kpHpHeight = 20.0f;		// HP�o�[�̏c��
	constexpr int kpHpColor = 0xff0000;		// HP�o�[�̐F
	constexpr float kpGaugeDispL = 30.0f;	// �Q�[�W�o�[���\���ʒu
	constexpr float kpGaugeDispT = 80.0f;	// �Q�[�W�o�[��\���ʒu
	constexpr float kpGaugeWidth = 400.0f;	// �Q�[�W�o�[�̉���
	constexpr float kpGaugeHeight = 20.0f;  // �Q�[�W�o�[�̏c��
	constexpr int kpGaugeColor = 0x0000ff;	// HP�o�[�̐F
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
UIBattle::UIBattle()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
UIBattle::~UIBattle()
{
}


/// <summary>
/// ������
/// </summary>
void UIBattle::Init()
{
}


/// <summary>
/// �X�V
/// </summary>
void UIBattle::Update()
{
}


/// <summary>
/// �v���C���[��HP�o�[�\��
/// </summary>
/// <param name="currentHp">���݂�HP</param>
/// <param name="MaxHp">�ő�HP</param>
void UIBattle::DrawPlayerHP(float currentHp, float MaxHp)
{
	// �\������Q�[�W�ʂ��v�Z����
	float hpRatio = currentHp / MaxHp;
	float hpLength = kpHpWidth* hpRatio;

	// TODO:�o�[�̔w�i�����͉摜�ɂ���
	DrawBoxAA(kpHpDispL, kpHpDispT, kpHpDispL + kpHpWidth, kpHpDispT + kpHpHeight, 0xffffff, false);
	DrawBoxAA(kpHpDispL, kpHpDispT, kpHpDispL + hpLength, kpHpDispT + kpHpHeight, kpHpColor, true);
}


/// <summary>
/// �v���C���[�̃Q�[�W�o�[�\��
/// </summary>
/// <param name="currentGauge">���݂̃Q�[�W��</param>
/// <param name="MaxGauge">�ő�Q�[�W��</param>
void UIBattle::DrawPlayerGauge(float currentGauge, float MaxGauge)
{
	// �\������Q�[�W�ʂ��v�Z����
	float hpRatio = currentGauge / MaxGauge;
	float hpLength = kpGaugeWidth * hpRatio;

	// TODO:�o�[�̔w�i�����͉摜�ɂ���
	DrawBoxAA(kpGaugeDispL, kpGaugeDispT, kpGaugeDispL + kpGaugeWidth, kpGaugeDispT + kpGaugeHeight, 0xffffff, false);
	DrawBoxAA(kpGaugeDispL, kpGaugeDispT, kpGaugeDispL + hpLength, kpGaugeDispT + kpGaugeHeight, kpGaugeColor, true);
}
