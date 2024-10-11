#pragma once
#include <vector>

class Player;
class EnemyBase;

/// <summary>
/// エフェクト管理クラス
/// </summary>
class Effect
{
public:
	Effect();
	~Effect();
	void Init();
	void Update();
	void Draw();
	void ClearEffect();								// 画面上のエフェクトを停止する
	void PlayDamageEffect(const VECTOR& pos);		// 攻撃エフェクトを再生する
	void PlayerDamageBowEffect(const VECTOR& pos);	// 弓攻撃エフェクトを再生する
	void PlayDeathEffect(const VECTOR& pos);		// ガードエフェクトを再生する

private:
	int m_attackEffectTime;		// 攻撃エフェクトを再生する時間
	int m_attackBowEffectTime;	// 弓攻撃エフェクトを再生する時間
	int m_deathEffectTime;		// 死亡時エフェクトを再生する時間

	int m_attackHandle;

	struct Effect_
	{
		int handle;
		VECTOR m_pos;
	};

	// パーティクルを生み出すもの
	struct EffectEmitter
	{
		std::vector<int> emitterHandle;
		std::vector<Effect_> effects;
	};

	EffectEmitter emitter;

	// エフェクトの種類
	enum EffectKind
	{
		kAttack,	// 攻撃
		kAttackBow,	// 弓攻撃
		kDeath,		// 死亡時
	};
};