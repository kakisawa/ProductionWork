#pragma once
#include "SceneBase.h"

class SceneOption :
    public SceneBase
{
public:
	SceneOption();
	virtual ~SceneOption();

	virtual void Init();
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();
};

