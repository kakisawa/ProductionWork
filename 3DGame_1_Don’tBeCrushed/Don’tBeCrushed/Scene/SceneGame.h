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

    int GetEnemyAttackNum() { return m_enemyAttckNum; }     // 倒れる敵の番号
    VECTOR GetEnemyAttackPos() { return m_enemyAttackPos; } // 敵の当たり判定座標
   
private:
    int m_enemyNum;         // 敵の数
    int m_enemyInterval;    // 敵が倒れるまでのインターバル
    int m_enemyAttckNum;    // 倒れる敵の番号
    int m_gameTime;         // ゲームの残り時間
    int m_fadeAlpha;		// フェードイン・アウト

    bool m_col;				// 敵との当たり判定フラグ
    bool m_isFadeIn;		// フェードイン用フラグ
    bool m_isFadeOut;		// フェードアウト用フラグ
    bool m_isSceneEnd;		// シーン遷移用フラグ

    VECTOR m_enemyAttackPos;// 敵の当たり判定座標
    Collision m_colRect;    // 当たり判定矩形

    vector<shared_ptr<Enemy>> m_pEnemy;
    shared_ptr<Player> m_pPlayer = make_shared<Player>();
    shared_ptr<Stage> m_pStage = make_shared<Stage>();
    shared_ptr<Camera> m_pCamera = make_shared<Camera>();
};