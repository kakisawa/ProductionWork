#pragma once
#include "SceneBase.h"

class SceneGameOver :
    public SceneBase
{
public:
	SceneGameOver();
	virtual ~SceneGameOver();

	virtual void Init();
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();

private:
	int m_graph;
};