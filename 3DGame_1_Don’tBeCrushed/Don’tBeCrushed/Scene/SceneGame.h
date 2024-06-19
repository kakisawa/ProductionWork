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

    void EnemyCol();    // �G�����蔻��Ǘ�

    VECTOR GetEnemyAttackPos() { return m_enemyAttackPos; }
    int GetEnemyAttackNum() { return m_enemyAttckNum; }

private:
    int m_enemyNum;         // �G�̐�
    int m_enemyInterval;    // �G���|���܂ł̃C���^�[�o��
    int m_enemyAttckNum;    // �ǂ̓G���|��邩

    VECTOR m_enemyAttackPos;     // �G�̓����蔻����W
    
    
    // �����蔻���`
    Rect m_colRect;

    vector<shared_ptr<Enemy>> m_pEnemy;
    shared_ptr<Player> m_pPlayer = make_shared<Player>();
    shared_ptr<Stage> m_pStage = make_shared<Stage>();
};