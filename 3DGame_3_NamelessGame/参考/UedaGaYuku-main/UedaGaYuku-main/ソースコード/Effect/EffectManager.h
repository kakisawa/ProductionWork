#pragma once
#include <vector>

class Player;
class EnemyBase;

/// <summary>
/// エフェクト管理クラス
/// </summary>
class EffectManager
{
public:
	EffectManager();
	~EffectManager();
	void Init();
	void Update();
	void Draw();
	void ClearEffect();							// 画面上のエフェクトを停止する
	void PlayDamageEffect(const VECTOR& pos);	// 攻撃エフェクトを再生する
	void PlayGuardEffect(const VECTOR& pos);	// ガードエフェクトを再生する

private:
	int m_attackEffectTime;	// 攻撃エフェクトを再生する時間
	int m_guardEffectTime;	// ガードエフェクトを再生する時間

	struct Effect
	{
		int handle;
		VECTOR pos;
	};

	// パーティクルを生み出すもの
	struct EffectEmitter
	{
		std::vector<int> emitterHandle;
		std::vector<Effect> effects;
	};
	EffectEmitter emitter;

	// エフェクトの種類
	enum EffectKind
	{
		kAttack,	// 攻撃
		kGuard,		// ガード
	};
};