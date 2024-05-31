#pragma once
#include "SceneBase.h"
#include <vector>


class Enemy;
class Player;
class SceneGame :
    public SceneBase
{
public:
    SceneGame();
    virtual ~SceneGame();

    void Init();
    shared_ptr<SceneBase> Update();
    void Draw();

    void End();

private:
    int m_enemyNum;

    int m_enemyCount;       // どの敵が倒れてくるかランダムカウント
    int m_enemyInterval;    // 敵が倒れるまでのインターバル
    

    vector<shared_ptr<Enemy>> m_pEnemy;
    shared_ptr<Player> m_pPlayer = make_shared<Player>();
};