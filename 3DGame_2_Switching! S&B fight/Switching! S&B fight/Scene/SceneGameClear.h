#pragma once
#include "SceneBase.h"

class SceneGameClear :
    public SceneBase
{
public:
	SceneGameClear();
	virtual ~SceneGameClear();

	virtual void Init();
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();

private:
	int m_graph;
	int m_movie;
};

