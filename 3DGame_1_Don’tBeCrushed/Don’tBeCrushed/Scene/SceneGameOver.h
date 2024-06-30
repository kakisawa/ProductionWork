#pragma once
#include "SceneBase.h"

using namespace std;
class SceneGameOver :
	public SceneBase
{
public:
	SceneGameOver();
	virtual ~SceneGameOver();

	virtual void Init() {}
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End() {}

private:
};