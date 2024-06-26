#pragma once
#include "SceneBase.h"
#include "DxLib.h"
#include "../Collision.h"
#include <vector>

class Enemy;
class Player;
class Stage;
class Camera;
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

    void EnemyCol();    // 敵当たり判定管理

    VECTOR GetEnemyAttackPos() { return m_enemyAttackPos; }
    int GetEnemyAttackNum() { return m_enemyAttckNum; }

private:
    int m_enemyNum;         // 敵の数
    int m_enemyInterval;    // 敵が倒れるまでのインターバル
    int m_enemyAttckNum;    // どの敵が倒れるか

    VECTOR m_enemyAttackPos;     // 敵の当たり判定座標
    
    
    // 当たり判定矩形
    Collision m_colRect;

    vector<shared_ptr<Enemy>> m_pEnemy;
    shared_ptr<Player> m_pPlayer = make_shared<Player>();
    shared_ptr<Stage> m_pStage = make_shared<Stage>();
    shared_ptr<Camera> m_pCamera = make_shared<Camera>();
};