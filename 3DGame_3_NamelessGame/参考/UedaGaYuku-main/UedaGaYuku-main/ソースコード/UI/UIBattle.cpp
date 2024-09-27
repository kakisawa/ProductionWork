#include "DxLib.h"
#include "Vec2.h"
#include "Font.h"
#include "UI.h"
#include "CharacterBase.h"
#include "UIBattle.h"
#include <string>

// 定数
namespace
{
	/*試合開始時*/
	constexpr int kEnemyNameDispStart = 300;				// 敵名を表示し始める時間
	constexpr int kMatchNumDispStart = 200;					// 試合数を表示し始める時間
	constexpr int kFightTextDispStart = 80;					// "Fight"のテキストを表示し始める時間
	constexpr float kFightTextScele = 0.6f;					// "Fight"のテキストサイズ
	const Vec2 kFightTextPos = { 960, 550 };				// "Fight"のテキスト位置
	const Vec2 kStartEnemyNamePos = { 950, 500 };			// 敵の名前表示位置
	const Vec2 kMatcheNumTextPos = { 850, 700 };			// 現在の試合数表示位置
	constexpr int kMatchNumTextWidth = 260;					// 1つ当たりのテキストの幅
	constexpr int kMatchNumTextInterval = 70;				// テキストの表示間隔
	constexpr int kTextAdj = 60;							// テキストの表示間隔調整
	constexpr float kMatchNumTextScele = 0.4f;				// 試合数のテキストサイズ
	constexpr float kEnemyNameMinScale = 0.5f;				// 敵名最小サイズ
	constexpr float kEnemyNameMaxScale = 10.0f;				// 敵名最大サイズ
	constexpr float kEnemyNameChangeScale = 0.6f;			// 敵名サイズ変化量

	/*クリア時*/
	const Vec2 kClearBgPos = { 200, 0 };					// クリア背景位置
	const Vec2 kGekihaTextPos = { 950, 500 };				// "撃破"テキスト位置
	constexpr int kGekihaDispTime = 120;				    // "撃破"テキストを表示しはじめる時間
	constexpr float kGekihaTextMinScale = 1.0f;				// "撃破"テキスト最小サイズ
	constexpr float kGekihaTextMaxScale = 10.0f;			// "撃破"テキスト最大サイズ
	constexpr float kGekihaTextChangeScale = 0.6f;			// "撃破"テキストサイズ変化量
	constexpr int kMULAPal = 255;							// 乗算ブレンド値

	/*キャラクターUI*/
	constexpr int kSilhouetteWidth = 268;					// シルエット画像幅
	constexpr int kSilhouetteHeight = 213;					// キャラクターのシルエット画像高さ
	constexpr int kHpColor = 0xff0000;						// HPバーの色
	constexpr int kDamageHpColor = 0xffd700;				// ダメージ時のHPバーの色
	constexpr int kpGaugeColor = 0x0000ff;					// ゲージバーの色
	constexpr int kIntervalTime = 80;						// HPバーが減少するまでの時間

	/*プレイヤーUI*/
	const Vec2 kPSilhouettePos = { 130.0f, 100.0f };		 // プレイヤーのシルエット位置
	const Vec2 kPNameBackPos = { 150.0f, 58.0f };			 // プレイヤーの名前の背景位置
	const Vec2 kPNamePos = { 250.0f, 56.0f };				 // プレイヤーの名前位置
	const Vec2 kPlayerHpBarLT = { 125.0f, 100.0f };			 // HPバー左上位置
	const Vec2 kPlayerHpBarRB = { 950.0f, 130.0f };			 // HPバー右下位置
	const Vec2 kPlayerCurrentHpLT = { 150.0f, 106.0f };		 // 現在のHP左上位置
	constexpr float kPlayerHpWidth = 765.0f;				 // HPバーの横幅
	constexpr float kPlayerHpHeight = 18.0f;				 // HPバーの縦幅
	const Vec2 kPlayerGaugeBarLT = { 125.0f, 145.0f };		 // ゲージバー左上位置
	const Vec2 kPlayerGaugeBarRB = { 800.0f, 170.0f };		 // ゲージバー右下位置
	const Vec2 kPlayerCurrentGaugeLT = { 145.0f, 150.0f };	 // 現在のゲージ量左上位置
	constexpr float kPlayerGaugeWidth = 627.0f;				 // ゲージバーの横幅
	constexpr float kPlayerGaugeHeight = 15.0f;				 // ゲージバーの縦幅
	constexpr float kPSilhouetteScale = 0.7f;				 // プレイヤーのシルエットサイズ

	/*敵UI*/
	const Vec2 kESilhouettePos = { 1700.0f, 870.0f };		 // 敵キャラクターのシルエット位置
	const Vec2 kENameBackPos = { 1350.0f, 928.0f };			 // 敵キャラクターの名前の背景位置
	const Vec2 kENamePos = { 1460.0f, 924.0f };				 // 敵の名前位置
	const Vec2 kEnemyHpBarLT = { 850.0f, 970.0f };			 // HPバー左上位置
	const Vec2 kEnemyHpBarRB = { 1850.0f, 1000.0f };		 // HPバー右下位置
	const Vec2 kEnemyCurrentHpLT = { 885.0f, 976.0f };		 // 現在のHP左上位置
	constexpr float kEnemyHpWidth = 920.0f;					 // HPバーの横幅
	constexpr float kEnemyHpHeight = 17.0f;					 // HPバーの縦幅
	constexpr float kESilhouetteScale = 1.0f;				 // 敵のシルエットサイズ

	/*操作説明画面*/
	const Vec2 kOperationFramePos = { 1720.0f, 280.0f };	 // 枠表示位置
	const Vec2 kOperationTextPos = { 1730.0f, 300.0f };		 // テキストの表示位置
	const Vec2 kOperationButtonPos = { 1880.0f, 320.0f };	 // ボタン位置
	constexpr float kOperationWidth = 250.0f;				 // 枠の横幅
	constexpr float kOperationHeight = 390.0f;				 // 枠の縦幅
	constexpr int kOperationBackColor = 0x000000;			 // 枠の背景色
	constexpr int kOperationBackAlpha = 200;				 // α値
	constexpr int kButtonSize = 32;						 	 // ボタン画像のサイズ
	constexpr float kOperationButtonScale = 1.0f;		 	 // ボタンの拡大率
	constexpr float kOperationInterval = 40.0f;				 // 表示間隔
	constexpr int kTextColor = 0xffffff;					 // テキストの色

	/*必殺技*/
	const Vec2 kSpecialAttackButtonPos = { 760.0f, 200.0f }; // ボタン位置 
	constexpr float kSpecialAttackButtonScale = 1.2f;		 // ボタン拡大率
	const Vec2 kSpecialAttackTextPos = { 620.0f, 180.0f };	 // テキスト位置
	constexpr int kSpecialAttackTextColor = 0x1470cc;		 // 必殺技を放てる場合のテキストの色
	constexpr int kNotSpecialTextColor = 0x808080;			 // 必殺技が放てない場合のテキストの色
	constexpr int kSpecialAttackTextEdgeColor = 0x0a3866;	 // テキスト縁の色
	constexpr int kMaxPal = 255;							 // 最大加算値
	constexpr float kSpecialTextMinScale = 1.0f;			 // "必殺技"テキスト最小サイズ
	constexpr float kSpecialTextMaxScale = 8.0f;			 // "必殺技"テキスト最大サイズ
	constexpr float kSpecialTextChangeScale = 0.6f;			 // "必殺技"テキストサイズ変化量

	/*チュートリアル*/
	const Vec2 kTutoTextPos = { 1300.0f, 770.0f };			// テキスト位置
	const Vec2 kTutoButtonPos = { 1390.0f, 790.0f };		// ボタン位置
	constexpr float kTutoButtonScale = 1.2f;				// ボタン拡大率
}


/// <summary>
/// 引数つきコンストラクタ
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
/// デストラクタ
/// </summary>
UIBattle::~UIBattle()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
}


/// <summary>
/// 更新
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
/// ダメージを受けた際にタイマーをセットする
/// </summary>
void UIBattle::SetDamageTimer()
{
	if (m_intervalTime < 0)
	{
		m_intervalTime = kIntervalTime;
	}
}


/// <summary>
/// ダメージを受けた時の処理
/// </summary>
/// <param name="damage">受けたダメージ量</param>
void UIBattle::OnDamage(float damage)
{
	m_damage += damage;
	SetDamageTimer();
}


/// <summary>
/// スタート演出をリセットする
/// </summary>
void UIBattle::ResetStartProduction()
{
	m_enemyNameScale = kEnemyNameMaxScale;
}


/// <summary>
/// スタート演出表示
/// </summary>
/// <param name="time">演出経過時間</param>
/// <param name="matchNum">現在の試合数</param>
/// <param name="maxMatch">最大試合数</param>
/// <param name="enemyKind">敵の種類</param>
void UIBattle::DrawStartProduction(int time, int matchNum, int maxMatch)
{
	// 敵の名前を表示
	if (time < kEnemyNameDispStart && time > kFightTextDispStart)
	{
		// 敵名のサイズをだんだん小さくする
		m_enemyNameScale -= kEnemyNameChangeScale;
		m_enemyNameScale = std::max(kEnemyNameMinScale, m_enemyNameScale);

		int sizeW, sizeH;
		GetGraphSize(m_handle[m_currentEnemy], &sizeW, &sizeH);
		DrawRectRotaGraphF(kStartEnemyNamePos.x, kStartEnemyNamePos.y, 0, 0, sizeW, sizeH, m_enemyNameScale, 0.0f, m_handle[m_currentEnemy], true);
	}
	// 試合数を表示
	if (time < kMatchNumDispStart && time > kFightTextDispStart)
	{
		int sizeW, sizeH;
		GetGraphSize(m_handle[HandleKind::kNumText], &sizeW, &sizeH);

		// 現在の試合数表示
		DrawRectRotaGraphF(kMatcheNumTextPos.x, kMatcheNumTextPos.y,
			kMatchNumTextWidth * matchNum, 0, kMatchNumTextWidth, sizeH,
			kMatchNumTextScele, 0.0f, m_handle[HandleKind::kNumText], true);
		// '/'表示
		DrawRectRotaGraphF(kMatcheNumTextPos.x + kMatchNumTextInterval, kMatcheNumTextPos.y,
			sizeW - kMatchNumTextWidth, 0, kMatchNumTextWidth, sizeH,
			kMatchNumTextScele, 0.0f, m_handle[HandleKind::kNumText], true);
		// 最大試合数を表示
		DrawRectRotaGraphF(kMatcheNumTextPos.x + kMatchNumTextInterval * 2 + kTextAdj, kMatcheNumTextPos.y,
			kMatchNumTextWidth * (maxMatch - 1), 0, kMatchNumTextWidth, sizeH,
			kMatchNumTextScele, 0.0f, m_handle[HandleKind::kNumText], true);
	}
	// "Fight!"の文字を表示
	if (time < kFightTextDispStart && time > 0)
	{
		int sizeW, sizeH;
		GetGraphSize(m_handle[HandleKind::kFightText], &sizeW, &sizeH);
		DrawRectRotaGraphF(kFightTextPos.x, kFightTextPos.y, 0, 0, sizeW, sizeH, kFightTextScele, 0.0f, m_handle[HandleKind::kFightText], true);
	}
}


/// <summary>
/// クリア演出をリセットする
/// </summary>
void UIBattle::ResetClearProduction()
{
	m_gekihaTextScale = kGekihaTextMaxScale;
}


/// <summary>
/// クリア演出表示
/// </summary>
/// <param name="time">現在のクリア演出時間</param>
void UIBattle::DrawClearProduction(int time)
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, kMULAPal);
	DrawGraphF(kClearBgPos.x, kClearBgPos.y, m_handle[HandleKind::kClearBg], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (time < kGekihaDispTime)
	{
		// テキストのサイズをだんだん小さくする
		m_gekihaTextScale -= kGekihaTextChangeScale;
		m_gekihaTextScale = std::max(kGekihaTextMinScale, m_gekihaTextScale);

		int sizeW, sizeH;
		GetGraphSize(m_handle[HandleKind::kGekihaText], &sizeW, &sizeH);
		DrawRectRotaGraphF(kGekihaTextPos.x, kGekihaTextPos.y, 0, 0, sizeW, sizeH, m_gekihaTextScale, 0.0f, m_handle[HandleKind::kGekihaText], true);
	}
}


/// <summary>
/// ゲームオーバー演出を表示
/// </summary>
void UIBattle::DrawGameoverProduciton()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, kMULAPal);
	DrawGraph(0 ,0, m_handle[HandleKind::kGameoverBg], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


/// <summary>
/// プレイヤーの名前を表示
/// </summary>
void UIBattle::DrawPlayerName()
{
	DrawGraphF(kPNameBackPos.x, kPNameBackPos.y, m_handle[HandleKind::kNameBack], true);
	DrawStringFToHandle(kPNamePos.x, kPNamePos.y, "Ueda", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kCharaName)]);
}


/// <summary>
/// プレイヤーのHPバー表示
/// </summary>
/// <param name="currentHp">現在のHP</param>
/// <param name="MaxHp">最大HP</param>
void UIBattle::DrawPlayerHP(float currentHp)
{
	// 表示するゲージ量を計算する
	float hpRatio = currentHp / m_maxHp;
	float decreaseHpRatio = (currentHp + m_damage) / m_maxHp;
	float hpLength = kPlayerHpWidth * hpRatio;
	float decreaseHpLength = kPlayerHpWidth * decreaseHpRatio;

	// バーの背景部分
	DrawExtendGraphF(kPlayerHpBarLT.x, kPlayerHpBarLT.y, kPlayerHpBarRB.x, kPlayerHpBarRB.y, m_handle[HandleKind::kGaugeBar], true);
	// ダメージを受けた際に表示する
	if (m_intervalTime > 0)
	{
		DrawBoxAA(kPlayerCurrentHpLT.x, kPlayerCurrentHpLT.y, kPlayerCurrentHpLT.x + decreaseHpLength, kPlayerCurrentHpLT.y + kPlayerHpHeight, kDamageHpColor, true);
	}
	// 現在のHPバー
	DrawBoxAA(kPlayerCurrentHpLT.x, kPlayerCurrentHpLT.y, kPlayerCurrentHpLT.x + hpLength, kPlayerCurrentHpLT.y + kPlayerHpHeight, kHpColor, true);
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
	float hpLength = kPlayerGaugeWidth * hpRatio;

	// バーの背景部分
	DrawExtendGraphF(kPlayerGaugeBarLT.x, kPlayerGaugeBarLT.y, kPlayerGaugeBarRB.x, kPlayerGaugeBarRB.y, m_handle[HandleKind::kGaugeBar], true);
	DrawBoxAA(kPlayerCurrentGaugeLT.x, kPlayerCurrentGaugeLT.y, kPlayerCurrentGaugeLT.x + hpLength, kPlayerCurrentGaugeLT.y + kPlayerGaugeHeight, kpGaugeColor, true);
}


/// <summary>
/// 敵の名前を表示
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
/// 敵のHPバーを表示
/// </summary>
/// <param name="currentHp">現在のHP</param>
void UIBattle::DrawEnemyHp(float currentHp)
{
	// 表示するゲージ量を計算する
	float hpRatio = currentHp / m_maxHp;
	float decreaseHpRatio = (currentHp + m_damage) / m_maxHp;
	float hpLength = kEnemyHpWidth * hpRatio;
	float decreaseHpLength = kEnemyHpWidth * decreaseHpRatio;

	// バーの背景部分
	DrawExtendGraphF(kEnemyHpBarLT.x, kEnemyHpBarLT.y, kEnemyHpBarRB.x, kEnemyHpBarRB.y, m_handle[HandleKind::kGaugeBar], true);
	// ダメージを受けた分のバー
	if (m_intervalTime > 0)
	{
		DrawBoxAA(kEnemyCurrentHpLT.x, kEnemyCurrentHpLT.y, kEnemyCurrentHpLT.x + decreaseHpLength, kEnemyCurrentHpLT.y + kEnemyHpHeight, kDamageHpColor, true);
	}
	DrawBoxAA(kEnemyCurrentHpLT.x, kEnemyCurrentHpLT.y, kEnemyCurrentHpLT.x + hpLength, kEnemyCurrentHpLT.y + kEnemyHpHeight, kHpColor, true);
}


/// <summary>
/// プレイヤーのシルエット描画
/// </summary>
void UIBattle::DrawPlayerSilhouette()
{
	DrawRectRotaGraphF(kPSilhouettePos.x, kPSilhouettePos.y, 0, 0, kSilhouetteWidth, kSilhouetteHeight, kPSilhouetteScale, 0.0f, m_handle[HandleKind::kSilhouette], true);
}


/// <summary>
/// 敵のシルエット描画
/// </summary>
/// <param name="charType">敵の種類</param>
void UIBattle::DrawEnemySilhouette(int charType)
{
	DrawRectRotaGraphF(kESilhouettePos.x, kESilhouettePos.y, kSilhouetteWidth * charType, 0, kSilhouetteWidth, kSilhouetteHeight, kESilhouetteScale, 0.0f, m_handle[HandleKind::kSilhouette], true);
}


/// <summary>
/// 必殺技の表示をリセットする
/// </summary>
void UIBattle::ResetSpecialAttack()
{
	m_specialTextScale = kSpecialTextMaxScale;
}


/// <summary>
/// 必殺技のテキスト表示
/// </summary>
/// <param name="currentGauge">現在の必殺技ゲージ量</param>
/// <param name="maxGauge">最大ゲージ量</param>
void UIBattle::DrawSpecialAttack(float currentGauge, float maxGauge)
{
	// 必殺技を発動できる場合
	if (currentGauge >= maxGauge)
	{
		m_specialTextScale -= kSpecialTextChangeScale;
		m_specialTextScale = std::max(kSpecialTextMinScale, m_specialTextScale);

		DrawExtendStringFToHandle(kSpecialAttackTextPos.x, kSpecialAttackTextPos.y, m_specialTextScale, m_specialTextScale, "必殺技",
			kSpecialAttackTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kSpecialAttack)], kSpecialAttackTextEdgeColor);
		SetDrawBlendMode(DX_BLENDMODE_ADD, kMaxPal);
		DrawExtendStringFToHandle(kSpecialAttackTextPos.x, kSpecialAttackTextPos.y, m_specialTextScale, m_specialTextScale, "必殺技",
			kSpecialAttackTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kSpecialAttack)]);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// ボタン画像表示
		DrawRectRotaGraphF(kSpecialAttackButtonPos.x, kSpecialAttackButtonPos.y, kButtonSize * ButtonKind::kBButton, 0, 
			kButtonSize, kButtonSize, kSpecialAttackButtonScale, 0.0f, m_buttonHandle, true);

	}
	// 必殺技を出せない場合
	else
	{
		m_specialTextScale = kSpecialTextMinScale;

		DrawExtendStringFToHandle(kSpecialAttackTextPos.x, kSpecialAttackTextPos.y, m_specialTextScale, m_specialTextScale, "必殺技",
			kNotSpecialTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kSpecialAttack)]);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}


/// <summary>
/// 操作説明を表示
/// </summary>
void UIBattle::DrawOperation()
{
	// 背景を薄く表示する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kOperationBackAlpha);
	DrawBoxAA(kOperationFramePos.x, kOperationFramePos.y, kOperationFramePos.x + kOperationWidth, kOperationFramePos.y + kOperationHeight, kOperationBackColor, true);
	DrawBoxAA(kOperationFramePos.x, kOperationFramePos.y, kOperationFramePos.x + kOperationWidth, kOperationFramePos.y + kOperationHeight, 0xff0000, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// テキスト表示
	DrawStringFToHandle(kOperationTextPos.x, kOperationTextPos.y + kOperationInterval * OperationOrder::kMove,
		"移動", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOperation)]);
	DrawStringFToHandle(kOperationTextPos.x, kOperationTextPos.y + kOperationInterval * OperationOrder::kCameraMove,
		"カメラ移動", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOperation)]);
	DrawStringFToHandle(kOperationTextPos.x, kOperationTextPos.y + kOperationInterval * OperationOrder::kPunch,
		"パンチ", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOperation)]);
	DrawStringFToHandle(kOperationTextPos.x, kOperationTextPos.y + kOperationInterval * OperationOrder::kKick,
		"キック", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOperation)]);
	DrawStringFToHandle(kOperationTextPos.x, kOperationTextPos.y + kOperationInterval * OperationOrder::kSpecialAttack,
		"必殺技", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOperation)]);
	DrawStringFToHandle(kOperationTextPos.x, kOperationTextPos.y + kOperationInterval * OperationOrder::kAvoid,
		"回避", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOperation)]);
	DrawStringFToHandle(kOperationTextPos.x, kOperationTextPos.y + kOperationInterval * OperationOrder::kGuard,
		"ガード", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOperation)]);
	DrawStringFToHandle(kOperationTextPos.x, kOperationTextPos.y + kOperationInterval * OperationOrder::kRockon,
		"ロックオン", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOperation)]);
	DrawStringFToHandle(kOperationTextPos.x, kOperationTextPos.y + kOperationInterval * OperationOrder::kPause,
		"ポーズ", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kOperation)]);

	// ボタン画像表示
	DrawRectRotaGraphF(kOperationButtonPos.x, kOperationButtonPos.y + kOperationInterval * OperationOrder::kMove,
		kButtonSize * ButtonKind::kRStick, 0, kButtonSize, kButtonSize, kOperationButtonScale, 0.0f, m_buttonHandle, true);		// Rスティック
	DrawRectRotaGraphF(kOperationButtonPos.x, kOperationButtonPos.y + kOperationInterval * OperationOrder::kCameraMove,
		kButtonSize * ButtonKind::kLStick, 0, kButtonSize, kButtonSize, kOperationButtonScale, 0.0f, m_buttonHandle, true);		// Lスティック
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
/// チュートリアル画面のボタン表示
/// </summary>
void UIBattle::DrawTutoButtonText()
{
	// テキスト表示
	DrawStringFToHandle(kTutoTextPos.x, kTutoTextPos.y,
		"次へ", kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kTutoButton)]);

	// ボタン画像表示
	DrawRectRotaGraphF(kTutoButtonPos.x, kTutoButtonPos.y, kButtonSize * ButtonKind::kAButton, 0, kButtonSize, kButtonSize, kTutoButtonScale, 0.0f, m_buttonHandle, true);
}
