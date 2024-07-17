#pragma once
#include "SceneBase.h"
#include "DxLib.h"

class Player;
class Camera;
class SceneGame :
    public SceneBase
{
public:
	SceneGame();
	virtual ~SceneGame();

	virtual void Init();
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();

private:
	int m_model;
	int m_arrowModel[4];

	VECTOR m_pos;

	shared_ptr<Player> m_pPlayer = make_shared<Player>();
	shared_ptr<Camera> m_pCamera = make_shared<Camera>();
};

