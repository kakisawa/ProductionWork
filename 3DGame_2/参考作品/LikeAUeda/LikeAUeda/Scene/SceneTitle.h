#pragma once
#include "SceneBase.h"

class Player;
class Camera;
class Stage;

/// <summary>
/// ƒ^ƒCƒgƒ‹‰æ–Ê
/// </summary>
class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	virtual  ~SceneTitle();
	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();
};

