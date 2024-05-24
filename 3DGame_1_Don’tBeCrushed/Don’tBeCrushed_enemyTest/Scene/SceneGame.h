#pragma once
#include "SceneBase.h"
#include <vector>

class Enemy;
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

    vector<shared_ptr<Enemy>> m_pEnemy;
};