#pragma once
#include "SceneBase.h"

class Player;
class Enemy;
class Item;
class Camera;
class Map;
class UISceneGame;
class Time;
class Fade;
class SceneGame :
    public SceneBase
{
public:
    SceneGame();
    ~SceneGame();

    virtual void Init() override;
    virtual std::shared_ptr<SceneBase> Update(Input& input) override;
    virtual void Draw() override;
    virtual void End() override;

private:

    int m_gameClearHandle;  // ゲームクリアロゴ画像

    bool m_isPause;
    //bool m_is

    // カメラ
    std::shared_ptr<Camera> m_pCamera = std::make_shared<Camera>();
    // プレイヤー
    std::shared_ptr<Player> m_pPlayer = std::make_shared<Player>();
    // 敵
    std::shared_ptr<Enemy> m_pEnemy = std::make_shared<Enemy>();
    // アイテム
    std::shared_ptr<Item> m_pItem = std::make_shared<Item>();
    // マップ
    std::shared_ptr<Map> m_pMap = std::make_shared<Map>();
    // UI
    std::shared_ptr<UISceneGame> m_pUI = std::make_shared<UISceneGame>();
    // 制限時間
    std::shared_ptr<Time> m_pTime = std::make_shared<Time>();
    // フェード
    std::shared_ptr<Fade> m_pFade = std::make_shared<Fade>();
};