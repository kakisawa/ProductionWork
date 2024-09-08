#pragma once
#include "SceneBase.h"
#include "SceneDebug.h"
#include <memory>

class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager(){}

	void Init();
	void Update();
	void Draw();
	void End();

private:
	shared_ptr<SceneBase> m_pScene;
};