#include "EffekseerForDXLib.h"
#include "Input.h"
#include "Player.h"
#include "EnemyBase.h"
#include "EffectManager.h"

// 定数
namespace
{
	constexpr int kEffectNum = 2;				// エフェクトの種類
	constexpr float kAttackEffectScale = 4.0f;	// 攻撃エフェクト拡大率
	constexpr int kAttackEffectTime = 80;		// 攻撃エフェクトの再生時間
	constexpr float kGaurdEffectScale = 8.0f;	// ガードエフェクトの拡大率
	constexpr int kGaurdEffectTime = 30;		// ガードエフェクトの再生時間
	constexpr float kGaurdEffectSpeed= 3.0f;	// ガードエフェクトの再生速度
}


/// <summary>
/// コンストラクタ
/// </summary>
EffectManager::EffectManager():
	m_attackEffectTime(0),
	m_guardEffectTime(0)
{
	emitter.emitterHandle.resize(kEffectNum);
	emitter.emitterHandle[EffectKind::kAttack] = LoadEffekseerEffect("data/Effect/attack.efk");
	emitter.emitterHandle[EffectKind::kGuard] = LoadEffekseerEffect("data/Effect/guard.efk");
}


/// <summary>
/// デストラクタ
/// </summary>
EffectManager::~EffectManager()
{
	for (auto& handle : emitter.emitterHandle)
	{
		DeleteEffekseerEffect(handle);
	}
}


/// <summary>
/// 初期化
/// </summary>
void EffectManager::Init()
{
	Effekseer_InitDistortion();	// エフェクトの歪みを適用する
	ClearEffect();
	m_attackEffectTime = 0;
	m_guardEffectTime = 0;
}


///// <summary>
///// 更新
///// </summary>
void EffectManager::Update()
{
	Effekseer_Sync3DSetting();	// 3Dの情報をDxLibとEffekseerで合わせる
	UpdateEffekseer3D();

	m_attackEffectTime--;
	m_guardEffectTime--;
}


/// <summary>
/// 描画
/// </summary>
void EffectManager::Draw()
{
	DrawEffekseer3D();
}


/// <summary>
/// 画面上のエフェクトを削除する
/// </summary>
void EffectManager::ClearEffect()
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
void EffectManager::PlayDamageEffect(const VECTOR& pos)
{
	// エフェクトが1回のみ表示されるようにする
	if (m_attackEffectTime > 0) return;

	m_attackEffectTime = kAttackEffectTime;
	emitter.effects.push_back({ PlayEffekseer3DEffect(emitter.emitterHandle[EffectKind::kAttack]), {}});
	auto& effect = emitter.effects.back();

	// エフェクトの表示位置を設定
	SetPosPlayingEffekseer3DEffect(effect.handle, pos.x, pos.y, pos.z);
	SetScalePlayingEffekseer3DEffect(effect.handle, kAttackEffectScale, kAttackEffectScale, kAttackEffectScale);
}


/// <summary>
/// ガードエフェクトを再生する
/// </summary>
/// <param name="pos">エフェクト位置</param>
void EffectManager::PlayGuardEffect(const VECTOR& pos)
{
	// ガード中はエフェクトが1回のみ表示されるようにする
	if (m_guardEffectTime > 0) return;

	m_guardEffectTime = kGaurdEffectTime;
	emitter.effects.push_back({ PlayEffekseer3DEffect(emitter.emitterHandle[EffectKind::kGuard]), {} });
	auto& effect = emitter.effects.back();

	// エフェクトの表示位置を設定
	SetPosPlayingEffekseer3DEffect(effect.handle, pos.x, pos.y, pos.z);
	SetScalePlayingEffekseer3DEffect(effect.handle, kGaurdEffectScale, kGaurdEffectScale, kGaurdEffectScale);
	SetSpeedPlayingEffekseer3DEffect(effect.handle, kGaurdEffectSpeed);
}
