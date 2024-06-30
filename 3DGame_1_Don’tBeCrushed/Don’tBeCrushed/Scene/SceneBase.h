#pragma once
#include <memory>
using namespace std;
class SceneBase:
	public enable_shared_from_this<SceneBase>
{
public:
	SceneBase(){}
	virtual ~SceneBase(){}
	virtual void Init(){}
	virtual shared_ptr<SceneBase> Update() = 0;
	virtual void Draw(){}
	virtual void End(){}
};