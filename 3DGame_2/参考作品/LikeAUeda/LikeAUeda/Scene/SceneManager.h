#pragma once
#include <memory>

class SceneBase;
class Input;

/// <summary>
/// シーン管理クラス
/// </summary>
class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();
	void Init();
	void Update(Input& input);
	void Draw();

private:
	std::shared_ptr<SceneBase> m_pScene;
};

