#pragma once
#include "SceneBase.h"

class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager() {}

	void Init();
	void Update();
	void Draw();
	void End();

private:
	std::shared_ptr<SceneBase> m_pScene;
};