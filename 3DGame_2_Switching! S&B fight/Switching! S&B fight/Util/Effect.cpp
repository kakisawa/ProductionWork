#include "EffekseerForDXLib.h"
#include "Pad.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy/EnemyLeft.h"
#include "../Object/Enemy/EnemyRight.h"
#include "Effect.h"

// 定数
namespace
{
	constexpr int kEffectNum = 3;				// エフェクトの種類
	constexpr float kAttackEffectScale = 4.0f;	// 攻撃エフェクト拡大率
	constexpr int kAttackEffectTime = 10;		// 攻撃エフェクトの再生時間
	constexpr float kAttackBowEffectScale = 2.0f;	// 弓攻撃エフェクト拡大率
	constexpr int kAttackBowEffectTime = 2;		// 弓攻撃エフェクトの再生時間

	constexpr float kDeathEffectScale = 5.0f;	// 死亡時エフェクトの拡大率
	constexpr int kDeathEffectTime = 10.0;		// 死亡時エフェクトの再生時間
}


/// <summary>
/// コンストラクタ
/// </summary>
Effect::Effect() :
	m_attackEffectTime(0),
	m_deathEffectTime(0),
	m_attackHandle(-1)
{
	emitter.emitterHandle.resize(kEffectNum);
	emitter.emitterHandle[EffectKind::kAttack] = LoadEffekseerEffect("data/Effect/attack.efk");
	emitter.emitterHandle[EffectKind::kAttackBow] = LoadEffekseerEffect("data/Effect/BowAttack.efk");
	emitter.emitterHandle[EffectKind::kDeath] = LoadEffekseerEffect("data/Effect/Use/death2.efk");
}

/// <summary>
/// デストラクタ
/// </summary>
Effect::~Effect()
{
	for (auto& handle : emitter.emitterHandle)
	{
		DeleteEffekseerEffect(handle);
	};
}

/// <summary>
/// 初期化
/// </summary>
void Effect::Init()
{
	Effekseer_InitDistortion();	// エフェクトの歪みを適用する
	ClearEffect();
	m_attackEffectTime = 0;
	m_attackBowEffectTime = 0;
	m_deathEffectTime = 0;
}

///// <summary>
///// 更新
///// </summary>
void Effect::Update()
{
	Effekseer_Sync3DSetting();	// 3Dの情報をDxLibとEffekseerで合わせる
	UpdateEffekseer3D();

	m_attackEffectTime--;
	m_attackBowEffectTime--;
	m_deathEffectTime--;
}


/// <summary>
/// 描画
/// </summary>
void Effect::Draw()
{
	DrawEffekseer3D();
}


/// <summary>
/// 画面上のエフェクトを削除する
/// </summary>
void Effect::ClearEffect()
{
	for (auto& e : emitter.effects)
	{
		StopEffekseer3DEffect(e.handle);
	}
	emitter.effects.clear();
}


/// <summary>
/// 攻撃エフェクトを再生する
/// </summary>
/// <param name="pos">エフェクト位置</param>
void Effect::PlayDamageEffect(const VECTOR& pos)
{
	// エフェクトが1回のみ表示されるようにする
	if (m_attackEffectTime > 0) return;

	m_attackEffectTime = kAttackEffectTime;
	emitter.effects.push_back({ PlayEffekseer3DEffect(emitter.emitterHandle[EffectKind::kAttack]), {} });
	auto& effect = emitter.effects.back();

	// エフェクトの表示位置を設定
	SetPosPlayingEffekseer3DEffect(effect.handle, pos.x, pos.y, pos.z);
	SetScalePlayingEffekseer3DEffect(effect.handle, kAttackEffectScale, kAttackEffectScale, kAttackEffectScale);
}

void Effect::PlayerDamageBowEffect(const VECTOR& pos)
{	
	// エフェクトが1回のみ表示されるようにする
	if (m_attackBowEffectTime > 0) return;

	m_attackBowEffectTime = kAttackBowEffectTime;
	emitter.effects.push_back({ PlayEffekseer3DEffect(emitter.emitterHandle[EffectKind::kAttackBow]), {} });
	auto& effect = emitter.effects.back();

	// エフェクトの表示位置を設定
	SetPosPlayingEffekseer3DEffect(effect.handle, pos.x, pos.y, pos.z);
	SetScalePlayingEffekseer3DEffect(effect.handle, kAttackBowEffectScale, kAttackBowEffectScale, kAttackBowEffectScale);
}


/// <summary>
/// 死亡エフェクトを再生する
/// </summary>
/// <param name="pos">エフェクト位置</param>
void Effect::PlayDeathEffect(const VECTOR& pos)
{
	// 死亡時はエフェクトが1回のみ表示されるようにする
	if (m_deathEffectTime > 0) return;

	m_deathEffectTime = kDeathEffectTime;
	emitter.effects.push_back({ PlayEffekseer3DEffect(emitter.emitterHandle[EffectKind::kDeath]), {} });
	auto& effect = emitter.effects.back();

	// エフェクトの表示位置を設定
	SetPosPlayingEffekseer3DEffect(effect.handle, pos.x, pos.y, pos.z);
	SetScalePlayingEffekseer3DEffect(effect.handle, kDeathEffectScale, kDeathEffectScale, kDeathEffectScale);
}
