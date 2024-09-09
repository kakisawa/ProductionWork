#pragma once
#include "SceneBase.h"

using namespace std;
class SceneTitle :
    public SceneBase
{
public:
	SceneTitle();
	virtual ~SceneTitle();

	virtual void Init();
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();

private:
	int graph;
	int graph2;
	int graph3;
};
