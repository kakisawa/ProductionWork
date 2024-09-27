#include "DxLib.h"
#include "Vec2.h"
#include "Font.h"
#include "UI.h"
#include "CharacterBase.h"
#include "UIBattle.h"
#include <string>

// �萔
namespace
{
	/*�����J�n��*/
	constexpr int kEnemyNameDispStart = 300;				// �G����\�����n�߂鎞��
	constexpr int kMatchNumDispStart = 200;					// ��������\�����n�߂鎞��
	constexpr int kFightTextDispStart = 80;					// "Fight"�̃e�L�X�g��\�����n�߂鎞��
	constexpr float kFightTextScele = 0.6f;					// "Fight"�̃e�L�X�g�T�C�Y
	const Vec2 kFightTextPos = { 960, 550 };				// "Fight"�̃e�L�X�g�ʒu
	const Vec2 kStartEnemyNamePos = { 950, 500 };			// �G�̖��O�\���ʒu
	const Vec2 kMatcheNumTextPos = { 850, 700 };			// ���݂̎������\���ʒu
	constexpr int kMatchNumTextWidth = 260;					// 1������̃e�L�X�g�̕�
	constexpr int kMatchNumTextInterval = 70;				// �e�L�X�g�̕\���Ԋu
	constexpr int kTextAdj = 60;							// �e�L�X�g�̕\���Ԋu����
	constexpr float kMatchNumTextScele = 0.4f;				// �������̃e�L�X�g�T�C�Y
	constexpr float kEnemyNameMinScale = 0.5f;				// �G���ŏ��T�C�Y
	constexpr float kEnemyNameMaxScale = 10.0f;				// �G���ő�T�C�Y
	constexpr float kEnemyNameChangeScale = 0.6f;			// �G���T�C�Y�ω���

	/*�N���A��*/
	const Vec2 kClearBgPos = { 200, 0 };					// �N���A�w�i�ʒu
	const Vec2 kGekihaTextPos = { 950, 500 };				// "���j"�e�L�X�g�ʒu
	constexpr int kGekihaDispTime = 120;				    // "���j"�e�L�X�g��\�����͂��߂鎞��
	constexpr float kGekihaTextMinScale = 1.0f;				// "���j"�e�L�X�g�ŏ��T�C�Y
	constexpr float kGekihaTextMaxScale = 10.0f;			// "���j"�e�L�X�g�ő�T�C�Y
	constexpr float kGekihaTextChangeScale = 0.6f;			// "���j"�e�L�X�g�T�C�Y�ω���
	constexpr int kMULAPal = 255;							// ��Z�u�����h�l

	/*�L�����N�^�[UI*/
	constexpr int kSilhouetteWidth = 268;					// �V���G�b�g�摜��
	constexpr int kSilhouetteHeight = 213;					// �L�����N�^�[�̃V���G�b�g�摜����
	constexpr int kHpColor = 0xff0000;						// HP�o�[�̐F
	constexpr int kDamageHpColor = 0xffd700;				// �_���[�W����HP�o�[�̐F
	constexpr int kpGaugeColor = 0x0000ff;					// �Q�[�W�o�[�̐F
	constexpr int kIntervalTime = 80;						// HP�o�[����������܂ł̎���

	/*�v���C���[UI*/
	const Vec2 kPSilhouettePos = { 130.0f, 100.0f };		 // �v���C���[�̃V���G�b�g�ʒu
	const Vec2 kPNameBackPos = { 150.0f, 58.0f };			 // �v���C���[�̖��O�̔w�i�ʒu
	const Vec2 kPNamePos = { 250.0f, 56.0f };				 // �v���C���[�̖��O�ʒu
	const Vec2 kPlayerHpBarLT = { 125.0f, 100.0f };			 // HP�o�[����ʒu
	const Vec2 kPlayerHpBarRB = { 950.0f, 130.0f };			 // HP�o�[�E���ʒu
	const Vec2 kPlayerCurrentHpLT = { 150.0f, 106.0f };		 // ���݂�HP����ʒu
	constexpr float kPlayerHpWidth = 765.0f;				 // HP�o�[�̉���
	constexpr float kPlayerHpHeight = 18.0f;				 // HP�o�[�̏c��
	const Vec2 kPlayerGaugeBarLT = { 125.0f, 145.0f };		 // �Q�[�W�o�[����ʒu
	const Vec2 kPlayerGaugeBarRB = { 800.0f, 170.0f };		 // �Q�[�W�o�[�E���ʒu
	const Vec2 kPlayerCurrentGaugeLT = { 145.0f, 150.0f };	 // ���݂̃Q�[�W�ʍ���ʒu
	constexpr float kPlayerGaugeWidth = 627.0f;				 // �Q�[�W�o�[�̉���
	constexpr float kPlayerGaugeHeight = 15.0f;				 // �Q�[�W�o�[�̏c��
	constexpr float kPSilhouetteScale = 0.7f;				 // �v���C���[�̃V���G�b�g�T�C�Y

	/*�GUI*/
	const Vec2 kESilhouettePos = { 1700.0f, 870.0f };		 // �G�L�����N�^�[�̃V���G�b�g�ʒu
	const Vec2 kENameBackPos = { 1350.0f, 928.0f };			 // �G�L�����N�^�[�̖��O�̔w�i�ʒu
	const Vec2 kENamePos = { 1460.0f, 924.0f };				 // �G�̖��O�ʒu
	const Vec2 kEnemyHpBarLT = { 850.0f, 970.0f };			 // HP�o�[����ʒu
	const Vec2 kEnemyHpBarRB = { 1850.0f, 1000.0f };		 // HP�o�[�E���ʒu
	const Vec2 kEnemyCurrentHpLT = { 885.0f, 976.0f };		 // ���݂�HP����ʒu
	constexpr float kEnemyHpWidth = 920.0f;					 // HP�o�[�̉���
	constexpr float kEnemyHpHeight = 17.0f;					 // HP�o�[�̏c��
	constexpr float kESilhouetteScale = 1.0f;				 // �G�̃V���G�b�g�T�C�Y

	/*����������*/
	const Vec2 kOperationFramePos = { 1720.0f, 280.0f };	 // �g�\���ʒu
	const Vec2 kOperationTextPos = { 1730.0f, 300.0f };		 // �e�L�X�g�̕\���ʒu
	const Vec2 kOperationButtonPos = { 1880.0f, 320.0f };	 // �{�^���ʒu
	constexpr float kOperationWidth = 250.0f;				 // �g�̉���
	constexpr float kOperationHeight = 390.0f;				 // �g�̏c��
	constexpr int kOperationBackColor = 0x000000;			 // �g�̔w�i�F
	constexpr int kOperationBackAlpha = 200;				 // ���l
	constexpr int kButtonSize = 32;						 	 // �{�^���摜�̃T�C�Y
	constexpr float kOperationButtonScale = 1.0f;		 	 // �{�^���̊g�嗦
	constexpr float kOperationInterval = 40.0f;				 // �\���Ԋu
	constexpr int kTextColor = 0xffffff;					 // �e�L�X�g�̐F

	/*�K�E�Z*/
	const Vec2 kSpecialAttackButtonPos = { 760.0f, 200.0f }; // �{�^���ʒu 
	constexpr float kSpecialAttackButtonScale = 1.2f;		 // �{�^���g�嗦
	const Vec2 kSpecialAttackTextPos = { 620.0f, 180.0f };	 // �e�L�X�g�ʒu
	constexpr int kSpecialAttackTextColor = 0x1470cc;		 // �K�E�Z����Ă�ꍇ�̃e�L�X�g�̐F
	constexpr int kNotSpecialTextColor = 0x808080;			 // �K�E�Z�����ĂȂ��ꍇ�̃e�L�X�g�̐F
	constexpr int kSpecialAttackTextEdgeColor = 0x0a3866;	 // �e�L�X�g���̐F
	constexpr int kMaxPal = 255;							 // �ő���Z�l
	constexpr float kSpecialTextMinScale = 1.0f;			 // "�K�E�Z"�e�L�X�g�ŏ��T�C�Y
	constexpr float kSpecialTextMaxScale = 8.0f;			 // "�K�E�Z"�e�L�X�g�ő�T�C�Y
	constexpr float kSpecialTextChangeScale = 0.6f;			 // "�K�E�Z"�e�L�X�g�T�C�Y�ω���

	/*�`���[�g���A��*/
	const Vec2 kTutoTextPos = { 1300.0f, 770.0f };			// �e�L�X�g�ʒu
	const Vec2 kTutoButtonPos = { 1390.0f, 790.0f };		// �{�^���ʒu
	constexpr float kTutoButtonScale = 1.2f;				// �{�^���g�嗦
}


/// <summary>
/// �������R���X�g���N�^
/// </summary>
UIBattle::UIBattle(float maxHp, int charType) :
	m_decreaseHp(maxHp),
	m_currentHp(maxHp),
	m_maxHp(maxHp),
	m_damage(0.0f),
	m_intervalTime(0),
	m_enemyNameScale(kEnemyNameMaxScale),
	m_gekihaTextScale(kGekihaTextMaxScale),
	m_specialTextScale(),
	m_currentEnemy(charType)
{
	m_handle.resize(HandleKind::kHandleNum);
	m_handle[HandleKind::kTutoName] = LoadGraph("data/UI/Name/Akagi.png");
	m_handle[HandleKind::kNinjaName] = LoadGraph("data/UI/Name/Bob.png");
	m_handle[HandleKind::kCiefName] = LoadGraph("data/UI/Name/Sato.png");
	m_handle[HandleKind::kOldManName] = LoadGraph("data/UI/Name/Abe.png");
	m_handle[HandleKind::kNameBack] = LoadGraph("data/UI/nameBack.png");
	m_handle[HandleKind::kGaugeBar] = LoadGraph("data/UI/gauge.png");
	m_handle[HandleKind::kSilhouette] = LoadGraph("data/UI/silhouette.png");
	m_handle[HandleKind::kFightText] = LoadGraph("data/UI/fight!.png");
	m_handle[HandleKind::kGekihaText] = LoadGraph("data/UI/gekiha.png");
	m_handle[HandleKind::kClearBg] = LoadGraph("data/UI/clearBg.png");
	m_handle[HandleKind::kGameoverBg] = LoadGraph("data/UI/gameoverBg.png");
	m_handle[HandleKind::kNumText] = LoadGraph("data/UI/number.png");
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
UIBattle::~UIBattle()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
}


/// <summary>
/// �X�V
/// </summary>
void UIBattle::UpdateHpBar()
{
	if (m_intervalTime >= 0)
	{
		m_intervalTime--;
	}
	if (m_intervalTime < 0)
	{
		m_currentHp -= m_damage;
		m_damage = 0.0f;
	}
}


/// <summary>
/// �_���[�W���󂯂��ۂɃ^�C�}�[���Z�b�g����
/// </summary>
void UIBattle::SetDamageTimer()
{
	if (m_intervalTime < 0)
	{
		m_intervalTime = kIntervalTime;
	}
}


/// <summary>
/// �_���[�W���󂯂����̏���
/// </summary>
/// <param name="damage">�󂯂��_���[�W��</param>
void UIBattle::OnDamage(float damage)
{
	m_damage += damage;
	SetDamageTimer();
}


/// <summary>
/// �X�^�[�g���o�����Z�b�g����
/// </summary>
void UIBattle::ResetStartProduction()
{
	m_enemyNameScale = kEnemyNameMaxScale;
}


/// <summary>
/// �X�^�[�g���o�\��
/// </summary>
/// <param name="time">���o�o�ߎ���</param>
/// <param name="matchNum">���݂̎�����</param>
/// <param name="maxMatch">�ő厎����</param>
/// <param name="enemyKind">�G�̎��</param>
void UIBattle::DrawStartProduction(int time, int matchNum, int maxMatch)
{
	// �G�̖��O��\��
	if (time < kEnemyNameDispStart && time > kFightTextDispStart)
	{
		// �G���̃T�C�Y�����񂾂񏬂�������
		m_enemyNameScale -= kEnemyNameChangeScale;
		m_enemyNameScale = std::max(kEnemyNameMinScale, m_enemyNameScale);

		int sizeW, sizeH;
		GetGraphSize(m_handle[m_currentEnemy], &sizeW, &sizeH);
		DrawRectRotaGraphF(kStartEnemyNamePos.x, kStartEnemyNamePos.y, 0, 0, sizeW, sizeH, m_enemyNameScale, 0.0f, m_handle[m_currentEnemy], true);
	}
	// ��������\��
	if (time < kMatchNumDispStart && time > kFightTextDispStart)
	{
		int sizeW, sizeH;
		GetGraphSize(m_handle[HandleKind::kNumText], &sizeW, &sizeH);

		// ���݂̎������\��
		DrawRectRotaGraphF(kMatcheNumTextPos.x, kMatcheNumTextPos.y,
			kMatchNumTextWidth * matchNum, 0, kMatchNumTextWidth, sizeH,
			kMatchNumTextScele, 0.0f, m_handle[HandleKind::kNumText], true);
		// '/'�\��
		DrawRectRotaGraphF(kMatcheNumTextPos.x + kMatchNumTextInterval, kMatcheNumTextPos.y,
			sizeW - kMatchNumTextWidth, 0, kMatchNumTextWidth, sizeH,
			kMatchNumTextScele, 0.0f, m_handle[HandleKind::kNumText], true);
		// �ő厎������\��
		DrawRectRotaGraphF(kMatcheNumTextPos.x + kMatchNumTextInterval * 2 + kTextAdj, kMatcheNumTextPos.y,
			kMatchNumTextWidth * (maxMatch - 1), 0, kMatchNumTextWidth, sizeH,
			kMatchNumTextScele, 0.0f, m_handle[HandleKind::kNumText], true);
	}
	// "Fight!"�̕�����\��
	if (time < kFightTextDispStart && time > 0)
	{
		int sizeW, sizeH;
		GetGraphSize(m_handle[HandleKind::kFightText], &sizeW, &sizeH);
		DrawRectRotaGraphF(kFightTextPos.x, kFightTextPos.y, 0, 0, sizeW, sizeH, kFightTextScele, 0.0f, m_handle[HandleKind::kFightText], true);
	}
}


/// <summary>
/// �N���A���o�����Z�b�g����
/// </summary>
void UIBattle::ResetClearProduction()
{
	m_gekihaTextScale = kGekihaTextMaxScale;
}


/// <summary>
/// �N���A���o�\��
/// </summary>
/// <param name="time">���݂̃N���A���o����</param>
void UIBattle::DrawClearProduction(int time)
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, kMULAPal);
	DrawGraphF(kClearBgPos.x, kClearBgPos.y, m_handle[HandleKind::kClearBg], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (time < kGekihaDispTime)
	{
		// �e�L�X�g�̃T�C�Y�����񂾂񏬂�������
		m_gekihaTextScale -= kGekihaTextChangeScale;
		m_gekihaTextScale = std::max(kGekihaTextMinScale, m_gekihaTextScale);

		int sizeW, sizeH;
		GetGraphSize(m_handle[HandleKind::kGekihaText], &sizeW, &sizeH);
		DrawRectRotaGraphF(kGekihaTextPos.x, kGekihaTextPos.y, 0, 0, sizeW, sizeH, m_gekihaTextScale, 0.0f, m_handle[HandleKind::kGekihaText], true);
	}
}


/// <summary>
/// �Q�[���I�[�o�[���o��\��
/// </summary>
void UIBattle::DrawGameoverProduciton()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, kMULAPal);
	DrawGraph(0 ,0, m_handle[HandleKind::kGameoverBg], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


/// <summary>
/// �v���C���[�̖��O��\��
/// </summary>
void UIBattle::DrawPlayerName()
{
	DrawGraphF(kPNameBackPos.x, kPNameBackPos.y, m_handle[HandleKind::kNameBack], true);
	DrawStringFToHandle(kPNamePos.x, kPNamePos.y, "Ueda", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kCharaName)]);
}


/// <summary>
/// �v���C���[��HP�o�[�\��
/// </summary>
/// <param name="currentHp">���݂�HP</param>
/// <param name="MaxHp">�ő�HP</param>
void UIBattle::DrawPlayerHP(float currentHp)
{
	// �\������Q�[�W�ʂ��v�Z����
	float hpRatio = currentHp / m_maxHp;
	float decreaseHpRatio = (currentHp + m_damage) / m_maxHp;
	float hpLength = kPlayerHpWidth * hpRatio;
	float decreaseHpLength = kPlayerHpWidth * decreaseHpRatio;

	// �o�[�̔w�i����
	DrawExtendGraphF(kPlayerHpBarLT.x, kPlayerHpBarLT.y, kPlayerHpBarRB.x, kPlayerHpBarRB.y, m_handle[HandleKind::kGaugeBar], true);
	// �_���[�W���󂯂��ۂɕ\������
	if (m_intervalTime > 0)
	{
		DrawBoxAA(kPlayerCurrentHpLT.x, kPlayerCurrentHpLT.y, kPlayerCurrentHpLT.x + decreaseHpLength, kPlayerCurrentHpLT.y + kPlayerHpHeight, kDamageHpColor, true);
	}
	// ���݂�HP�o�[
	DrawBoxAA(kPlayerCurrentHpLT.x, kPlayerCurrentHpLT.y, kPlayerCurrentHpLT.x + hpLength, kPlayerCurrentHpLT.y + kPlayerHpHeight, kHpColor, true);
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
	float hpLength = kPlayerGaugeWidth * hpRatio;

	// �o�[�̔w�i����
	DrawExtendGraphF(kPlayerGaugeBarLT.x, kPlayerGaugeBarLT.y, kPlayerGaugeBarRB.x, kPlayerGaugeBarRB.y, m_handle[HandleKind::kGaugeBar], true);
	DrawBoxAA(kPlayerCurrentGaugeLT.x, kPlayerCurrentGaugeLT.y, kPlayerCurrentGaugeLT.x + hpLength, kPlayerCurrentGaugeLT.y + kPlayerGaugeHeight, kpGaugeColor, true);
}


/// <summary>
/// �G�̖��O��\��
/// </summary>
void UIBattle::DrawEnemyName(int charType)
{
	DrawGraphF(kENameBackPos.x, kENameBackPos.y, m_handle[HandleKind::kNameBack], true);
	if (charType == HandleKind::kTutoName)
	{
		DrawStringFToHandle(kENamePos.x, kENamePos.y, "Akagi", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kCharaName)]);
	}
	if (charType == HandleKind::kNinjaName)
	{
		DrawStringFToHandle(kENamePos.x, kENamePos.y, "Bob", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kCharaName)]);
	}
	if (charType == HandleKind::kCiefName)
	{
		DrawStringFToHandle(kENamePos.x, kENamePos.y, "Sato", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kCharaName)]);
	}
	if (charType == HandleKind::kOldManName)
	{
		DrawStringFToHandle(kENamePos.x, kENamePos.y, "Abe", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kCharaName)]);
	}
}


/// <summary>
/// �G��HP�o�[��\��
/// </summary>
/// <param name="currentHp">���݂�HP</param>
void UIBattle::DrawEnemyHp(float currentHp)
{
	// �\������Q�[�W�ʂ��v�Z����
	float hpRatio = currentHp / m_maxHp;
	float decreaseHpRatio = (currentHp + m_damage) / m_maxHp;
	float hpLength = kEnemyHpWidth * hpRatio;
	float decreaseHpLength = kEnemyHpWidth * decreaseHpRatio;

	// �o�[�̔w�i����
	DrawExtendGraphF(kEnemyHpBarLT.x, kEnemyHpBarLT.y, kEnemyHpBarRB.x, kEnemyHpBarRB.y, m_handle[HandleKind::kGaugeBar], true);
	// �_���[�W���󂯂����̃o�[
	if (m_intervalTime > 0)
	{
		DrawBoxAA(kEnemyCurrentHpLT.x, kEnemyCurrentHpLT.y, kEnemyCurrentHpLT.x + decreaseHpLength, kEnemyCurrentHpLT.y + kEnemyHpHeight, kDamageHpColor, true);
	}
	DrawBoxAA(kEnemyCurrentHpLT.x, kEnemyCurrentHpLT.y, kEnemyCurrentHpLT.x + hpLength, kEnemyCurrentHpLT.y + kEnemyHpHeight, kHpColor, true);
}


/// <summary>
/// �v���C���[�̃V���G�b�g�`��
/// </summary>
void UIBattle::DrawPlayerSilhouette()
{
	DrawRectRotaGraphF(kPSilhouettePos.x, kPSilhouettePos.y, 0, 0, kSilhouetteWidth, kSilhouetteHeight, kPSilhouetteScale, 0.0f, m_handle[HandleKind::kSilhouette], true);
}


/// <summary>
/// �G�̃V���G�b�g�`��
/// </summary>
/// <param name="charType">�G�̎��</param>
void UIBattle::DrawEnemySilhouette(int charType)
{
	DrawRectRotaGraphF(kESilhouettePos.x, kESilhouettePos.y, kSilhouetteWidth * charType, 0, kSilhouetteWidth, kSilhouetteHeight, kESilhouetteScale, 0.0f, m_handle[HandleKind::kSilhouette], true);
}


/// <summary>
/// �K�E�Z�̕\�������Z�b�g����
/// </summary>
void UIBattle::ResetSpecialAttack()
{
	m_specialTextScale = kSpecialTextMaxScale;
}


/// <summary>
/// �K�E�Z�̃e�L�X�g�\��
/// </summary>
/// <param name="currentGauge">���݂̕K�E�Z�Q�[�W��</param>
/// <param name="maxGauge">�ő�Q�[�W��</param>
void UIBattle::DrawSpecialAttack(float currentGauge, float maxGauge)
{
	// �K�E�Z�𔭓��ł���ꍇ
	if (currentGauge >= maxGauge)
	{
		m_specialTextScale -= kSpecialTextChangeScale;
		m_specialTextScale = std::max(kSpecialTextMinScale, m_specialTextScale);

		DrawExtendStringFToHandle(kSpecialAttackTextPos.x, kSpecialAttackTextPos.y, m_specialTextScale, m_specialTextScale, "�K�E�Z",
			kSpecialAttackTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kSpecialAttack)], kSpecialAttackTextEdgeColor);
		SetDrawBlendMode(DX_BLENDMODE_ADD, kMaxPal);
		DrawExtendStringFToHandle(kSpecialAttackTextPos.x, kSpecialAttackTextPos.y, m_specialTextScale, m_specialTextScale, "�K�E�Z",
			kSpecialAttackTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kSpecialAttack)]);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �{�^���摜�\��
		DrawRectRotaGraphF(kSpecialAttackButtonPos.x, kSpecialAttackButtonPos.y, kButtonSize * ButtonKind::kBButton, 0, 
			kButtonSize, kButtonSize, kSpecialAttackButtonScale, 0.0f, m_buttonHandle, true);

	}
	// �K�E�Z���o���Ȃ��ꍇ
	else
	{
		m_specialTextScale = kSpecialTextMinScale;

		DrawExtendStringFToHandle(kSpecialAttackTextPos.x, kSpecialAttackTextPos.y, m_specialTextScale, m_specialTextScale, "�K�E�Z",
			kNotSpecialTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kSpecialAttack)]);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}


/// <summary>
/// ���������\��
/// </summary>
void UIBattle::DrawOperation()
{
	// �w�i�𔖂��\������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kOperationBackAlpha);
	DrawBoxAA(kOperationFramePos.x, kOperationFramePos.y, kOperationFramePos.x + kOperationWidth, kOperationFramePos.y + kOperationHeight, kOperationBackColor, true);
	DrawBoxAA(kOperationFramePos.x, kOperationFramePos.y, kOperationFramePos.x + kOperationWidth, kOperationFramePos.y + kOperationHeight, 0xff0000, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �e�L�X�g�\��
	DrawStringFToHandle(kOperationTextPos.x, kOperationTextPos.y + kOperationInterval * OperationOrder::kMove,
		"�ړ�", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOperation)]);
	DrawStringFToHandle(kOperationTextPos.x, kOperationTextPos.y + kOperationInterval * OperationOrder::kCameraMove,
		"�J�����ړ�", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOperation)]);
	DrawStringFToHandle(kOperationTextPos.x, kOperationTextPos.y + kOperationInterval * OperationOrder::kPunch,
		"�p���`", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOperation)]);
	DrawStringFToHandle(kOperationTextPos.x, kOperationTextPos.y + kOperationInterval * OperationOrder::kKick,
		"�L�b�N", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOperation)]);
	DrawStringFToHandle(kOperationTextPos.x, kOperationTextPos.y + kOperationInterval * OperationOrder::kSpecialAttack,
		"�K�E�Z", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOperation)]);
	DrawStringFToHandle(kOperationTextPos.x, kOperationTextPos.y + kOperationInterval * OperationOrder::kAvoid,
		"���", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOperation)]);
	DrawStringFToHandle(kOperationTextPos.x, kOperationTextPos.y + kOperationInterval * OperationOrder::kGuard,
		"�K�[�h", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOperation)]);
	DrawStringFToHandle(kOperationTextPos.x, kOperationTextPos.y + kOperationInterval * OperationOrder::kRockon,
		"���b�N�I��", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOperation)]);
	DrawStringFToHandle(kOperationTextPos.x, kOperationTextPos.y + kOperationInterval * OperationOrder::kPause,
		"�|�[�Y", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOperation)]);

	// �{�^���摜�\��
	DrawRectRotaGraphF(kOperationButtonPos.x, kOperationButtonPos.y + kOperationInterval * OperationOrder::kMove,
		kButtonSize * ButtonKind::kRStick, 0, kButtonSize, kButtonSize, kOperationButtonScale, 0.0f, m_buttonHandle, true);		// R�X�e�B�b�N
	DrawRectRotaGraphF(kOperationButtonPos.x, kOperationButtonPos.y + kOperationInterval * OperationOrder::kCameraMove,
		kButtonSize * ButtonKind::kLStick, 0, kButtonSize, kButtonSize, kOperationButtonScale, 0.0f, m_buttonHandle, true);		// L�X�e�B�b�N
	DrawRectRotaGraphF(kOperationButtonPos.x, kOperationButtonPos.y + kOperationInterval * OperationOrder::kPunch,
		kButtonSize * ButtonKind::kXButton, 0, kButtonSize, kButtonSize, kOperationButtonScale, 0.0f, m_buttonHandle, true);	// X
	DrawRectRotaGraphF(kOperationButtonPos.x, kOperationButtonPos.y + kOperationInterval * OperationOrder::kKick,
		kButtonSize * ButtonKind::kYButton, 0, kButtonSize, kButtonSize, kOperationButtonScale, 0.0f, m_buttonHandle, true);	// Y
	DrawRectRotaGraphF(kOperationButtonPos.x, kOperationButtonPos.y + kOperationInterval * OperationOrder::kSpecialAttack,
		kButtonSize * ButtonKind::kBButton, 0, kButtonSize, kButtonSize, kOperationButtonScale, 0.0f, m_buttonHandle, true);	// B
	DrawRectRotaGraphF(kOperationButtonPos.x, kOperationButtonPos.y + kOperationInterval * OperationOrder::kAvoid,
		kButtonSize * ButtonKind::kAButton, 0, kButtonSize, kButtonSize, kOperationButtonScale, 0.0f, m_buttonHandle, true);	// A
	DrawRectRotaGraphF(kOperationButtonPos.x, kOperationButtonPos.y + kOperationInterval * OperationOrder::kGuard,
		kButtonSize * ButtonKind::kLBButton, 0, kButtonSize, kButtonSize, kOperationButtonScale, 0.0f, m_buttonHandle, true);	// LB
	DrawRectRotaGraphF(kOperationButtonPos.x, kOperationButtonPos.y + kOperationInterval * OperationOrder::kRockon,
		kButtonSize * ButtonKind::kRBButton, 0, kButtonSize, kButtonSize, kOperationButtonScale, 0.0f, m_buttonHandle, true);	// RB
	DrawRectRotaGraphF(kOperationButtonPos.x, kOperationButtonPos.y + kOperationInterval * OperationOrder::kPause,
		kButtonSize * ButtonKind::kMenuButton, 0, kButtonSize, kButtonSize, kOperationButtonScale, 0.0f, m_buttonHandle, true);	// Menu
}


/// <summary>
/// �`���[�g���A����ʂ̃{�^���\��
/// </summary>
void UIBattle::DrawTutoButtonText()
{
	// �e�L�X�g�\��
	DrawStringFToHandle(kTutoTextPos.x, kTutoTextPos.y,
		"����", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kTutoButton)]);

	// �{�^���摜�\��
	DrawRectRotaGraphF(kTutoButtonPos.x, kTutoButtonPos.y, kButtonSize * ButtonKind::kAButton, 0, kButtonSize, kButtonSize, kTutoButtonScale, 0.0f, m_buttonHandle, true);
}
