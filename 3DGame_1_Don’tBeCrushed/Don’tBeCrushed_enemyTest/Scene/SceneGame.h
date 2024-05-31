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

    int m_enemyCount;       // �ǂ̓G���|��Ă��邩�����_���J�E���g
    int m_enemyInterval;    // �G���|���܂ł̃C���^�[�o��
    

    vector<shared_ptr<Enemy>> m_pEnemy;
    shared_ptr<Player> m_pPlayer = make_shared<Player>();
};