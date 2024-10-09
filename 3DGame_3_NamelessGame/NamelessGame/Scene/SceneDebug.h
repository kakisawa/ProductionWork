#pragma once
#include "SceneBase.h"

class SceneDebug :
    public SceneBase
{
public:
	SceneDebug();
	virtual ~SceneDebug() {}

	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End() {}

	void SwitchingScene();

private:
	
	struct selectBox
	{
		VECTOR selectPos;

	}m_selectBox;

	enum class nextScene
	{
		kTitleScene,
		kOptionScene,
		kRankingScene,
		kGameScene,
		kGameClearScene,
		kGameOverScene,
		kGameEnd,
	}m_nextScene;
};