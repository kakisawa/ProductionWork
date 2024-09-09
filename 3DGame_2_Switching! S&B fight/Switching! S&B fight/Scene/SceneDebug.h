#pragma once
#include "SceneBase.h"

class SceneDebug:
	public SceneBase
{
public:
	SceneDebug();
	virtual ~SceneDebug(){}

	virtual void Init();
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End(){}

	void SwitchingScene();

private:

	enum class nextScene
	{
		kNone,
		kTitleScene,
		kOptionScene,
		kRankingScene,
		kGameScene,
		kGameClearScene,
		kGameOverScene,
		kGameEnd,
	};

	nextScene m_nextScene;
};

