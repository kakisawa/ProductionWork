#pragma once
#include "SceneBase.h"
#include "DxLib.h"
#include "../Rect.h"
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

    void EnemyCol();    // “G“–‚½‚è”»’èŠÇ—

    VECTOR GetEnemyAttackPos() { return m_enemyAttackPos; }
    int GetEnemyAttackNum() { return m_enemyAttckNum; }

private:
    int m_enemyNum;         // “G‚Ì”
    int m_enemyInterval;    // “G‚ª“|‚ê‚é‚Ü‚Å‚ÌƒCƒ“ƒ^[ƒoƒ‹
    int m_enemyAttckNum;    // ‚Ç‚Ì“G‚ª“|‚ê‚é‚©

    VECTOR m_enemyAttackPos;     // “G‚Ì“–‚½‚è”»’èÀ•W
    
    
    // “–‚½‚è”»’è‹éŒ`
    Rect m_colRect;

    vector<shared_ptr<Enemy>> m_pEnemy;
    shared_ptr<Player> m_pPlayer = make_shared<Player>();
    shared_ptr<Stage> m_pStage = make_shared<Stage>();
};