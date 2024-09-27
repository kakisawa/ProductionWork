#pragma once
#include "EnemyBase.h"
#include <functional>

/// <summary>
/// �`���[�g���A���Ŏg�p����G
/// </summary>
class EnemyTuto : public EnemyBase
{
public:
	EnemyTuto();
	virtual ~EnemyTuto();
	virtual void Init(std::shared_ptr<EffectManager> pEffect, VECTOR pos) override;
	virtual void Update(Player& player, Stage& stage, SceneStageBase& sceneStage) override;
	virtual void Draw() override;
	virtual void DrawUi() override;					// UI�̕\��
	virtual void OnDamage(float damage) override; 	// �_���[�W���󂯂��ۂ̏���
};