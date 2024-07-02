#pragma once
#include "SceneBase.h"
#include "DxLib.h"
#include <vector>
#include "../Collision.h"

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

    int GetEnemyAttackNum() { return m_enemyAttckNum; }     // �|���G�̔ԍ�
    VECTOR GetEnemyAttackPos() { return m_enemyAttackPos; } // �G�̓����蔻����W
   
private:
    int m_enemyNum;         // �G�̐�
    int m_enemyInterval;    // �G���|���܂ł̃C���^�[�o��
    int m_enemyAttckNum;    // �|���G�̔ԍ�
    int m_gameTime;         // �Q�[���̎c�莞��
    int m_fadeAlpha;		// �t�F�[�h�C���E�A�E�g

    bool m_col;				// �G�Ƃ̓����蔻��t���O
    bool m_isFadeIn;		// �t�F�[�h�C���p�t���O
    bool m_isFadeOut;		// �t�F�[�h�A�E�g�p�t���O
    bool m_isSceneEnd;		// �V�[���J�ڗp�t���O

    VECTOR m_enemyAttackPos;// �G�̓����蔻����W
    Collision m_colRect;    // �����蔻���`

    vector<shared_ptr<Enemy>> m_pEnemy;
    shared_ptr<Player> m_pPlayer = make_shared<Player>();
    shared_ptr<Stage> m_pStage = make_shared<Stage>();
    shared_ptr<Camera> m_pCamera = make_shared<Camera>();
};