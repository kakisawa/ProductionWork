#pragma once
#include "SceneStageBase.h"
#include <memory>

class Player;
class Camera;
class Stage;
class EnemyBase;
class CharacterBase;

/// <summary>
/// ステージ2
/// </summary>
class SceneStage2 : public SceneStageBase
{
public:
	SceneStage2() {};
	SceneStage2(std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera, std::shared_ptr<Stage> pStage);
	virtual ~SceneStage2();
	virtual void Init() override;
	virtual std::shared_ptr<SceneBase> Update(Input& input) override;
	virtual void Draw() override;

private:
	void UpdateSound();	// サウンド更新
};