#pragma once
#include "SceneBase.h"

class SceneRanking :
    public SceneBase
{
public:
	SceneRanking();
	virtual ~SceneRanking();

	virtual void Init();
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();
};