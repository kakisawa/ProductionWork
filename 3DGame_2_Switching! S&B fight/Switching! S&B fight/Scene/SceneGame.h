#pragma once
#include "SceneBase.h"
#include "DxLib.h"

#include <memory>
#include <string>
#include <vector>

class GameMap;
class Player;
class EnemyRight;
class EnemyLeft;
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

	VECTOR m_pos;

	shared_ptr<GameMap> m_pMap = make_shared<GameMap>();
	shared_ptr<Player> m_pPlayer = make_shared<Player>();
	shared_ptr<EnemyRight> m_pEnemyRight = make_shared<EnemyRight>();
	shared_ptr<EnemyLeft> m_pEnemyLeft = make_shared<EnemyLeft>();
	shared_ptr<Camera> m_pCamera = make_shared<Camera>();
};

