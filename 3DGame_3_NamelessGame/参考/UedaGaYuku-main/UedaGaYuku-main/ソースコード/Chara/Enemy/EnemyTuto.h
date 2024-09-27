#pragma once
#include "EnemyBase.h"
#include <functional>

/// <summary>
/// チュートリアルで使用する敵
/// </summary>
class EnemyTuto : public EnemyBase
{
public:
	EnemyTuto();
	virtual ~EnemyTuto();
	virtual void Init(std::shared_ptr<EffectManager> pEffect, VECTOR pos) override;
	virtual void Update(Player& player, Stage& stage, SceneStageBase& sceneStage) override;
	virtual void Draw() override;
	virtual void DrawUi() override;					// UIの表示
	virtual void OnDamage(float damage) override; 	// ダメージを受けた際の処理
};