#pragma once
#include "EnemyBase.h"

/// <summary>
/// シェフ
/// </summary>
class EnemyChef : public EnemyBase
{
public:
	EnemyChef();
	virtual ~EnemyChef();
	virtual void Init(std::shared_ptr<EffectManager> pEffect, VECTOR pos) override;
	virtual void Update(Player& player, Stage& stage, SceneStageBase& sceneStage) override;
	virtual void Draw() override;
	virtual void DrawUi() override;	// UIの表示
};

