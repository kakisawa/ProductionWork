#include "DxLib.h"
#include "UIBattle.h"

// 定数
namespace
{
	// プレイヤーUI
	constexpr float kpHpDispL = 30.0f;		// HPバー左表示位置
	constexpr float kpHpDispT = 40.0f;		// HPバー上表示位置
	constexpr float kpHpWidth = 400.0f;		// HPバーの横幅
	constexpr float kpHpHeight = 20.0f;		// HPバーの縦幅
	constexpr int kpHpColor = 0xff0000;		// HPバーの色
	constexpr float kpGaugeDispL = 30.0f;	// ゲージバー左表示位置
	constexpr float kpGaugeDispT = 80.0f;	// ゲージバー上表示位置
	constexpr float kpGaugeWidth = 400.0f;	// ゲージバーの横幅
	constexpr float kpGaugeHeight = 20.0f;  // ゲージバーの縦幅
	constexpr int kpGaugeColor = 0x0000ff;	// HPバーの色
}

/// <summary>
/// コンストラクタ
/// </summary>
UIBattle::UIBattle()
{
}

/// <summary>
/// デストラクタ
/// </summary>
UIBattle::~UIBattle()
{
}


/// <summary>
/// 初期化
/// </summary>
void UIBattle::Init()
{
}


/// <summary>
/// 更新
/// </summary>
void UIBattle::Update()
{
}


/// <summary>
/// プレイヤーのHPバー表示
/// </summary>
/// <param name="currentHp">現在のHP</param>
/// <param name="MaxHp">最大HP</param>
void UIBattle::DrawPlayerHP(float currentHp, float MaxHp)
{
	// 表示するゲージ量を計算する
	float hpRatio = currentHp / MaxHp;
	float hpLength = kpHpWidth* hpRatio;

	// TODO:バーの背景部分は画像にする
	DrawBoxAA(kpHpDispL, kpHpDispT, kpHpDispL + kpHpWidth, kpHpDispT + kpHpHeight, 0xffffff, false);
	DrawBoxAA(kpHpDispL, kpHpDispT, kpHpDispL + hpLength, kpHpDispT + kpHpHeight, kpHpColor, true);
}


/// <summary>
/// プレイヤーのゲージバー表示
/// </summary>
/// <param name="currentGauge">現在のゲージ量</param>
/// <param name="MaxGauge">最大ゲージ量</param>
void UIBattle::DrawPlayerGauge(float currentGauge, float MaxGauge)
{
	// 表示するゲージ量を計算する
	float hpRatio = currentGauge / MaxGauge;
	float hpLength = kpGaugeWidth * hpRatio;

	// TODO:バーの背景部分は画像にする
	DrawBoxAA(kpGaugeDispL, kpGaugeDispT, kpGaugeDispL + kpGaugeWidth, kpGaugeDispT + kpGaugeHeight, 0xffffff, false);
	DrawBoxAA(kpGaugeDispL, kpGaugeDispT, kpGaugeDispL + hpLength, kpGaugeDispT + kpGaugeHeight, kpGaugeColor, true);
}
