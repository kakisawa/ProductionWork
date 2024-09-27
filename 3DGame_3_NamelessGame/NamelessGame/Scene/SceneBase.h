#pragma once
#include "DxLib.h"
#include <memory>

class SceneBase:
	public std::enable_shared_from_this<SceneBase>
{
public:
	SceneBase(){}
	virtual ~SceneBase(){}

	virtual void Init(){}
	virtual std::shared_ptr<SceneBase> Update() = 0;
	virtual void Draw(){}
	virtual void End(){}

protected:
};