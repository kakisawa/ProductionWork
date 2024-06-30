#pragma once
#include <memory>

using namespace std;
class SceneBase;
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
	shared_ptr<SceneBase> m_pScene;
};