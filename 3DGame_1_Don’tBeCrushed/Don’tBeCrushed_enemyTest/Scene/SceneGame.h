#pragma once
#include "SceneBase.h"
#include <vector>

class Enemy;
class Player;
class Stage;
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
    int m_enemyNum;         // �G�̐�
    int m_enemyInterval;    // �G���|���܂ł̃C���^�[�o��

    vector<shared_ptr<Enemy>> m_pEnemy;
    shared_ptr<Player> m_pPlayer = make_shared<Player>();
    shared_ptr<Stage> m_pStage = make_shared<Stage>();
};