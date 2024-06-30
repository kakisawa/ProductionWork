#pragma once
#include "SceneBase.h"

using namespace std;
class SceneGameClear :
	public SceneBase
{
public:
	SceneGameClear();
	virtual ~SceneGameClear();

	virtual void Init() {}
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End() {}

private:
};