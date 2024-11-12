#pragma once
#include "SceneBase.h"
class SceneSelect :
    public SceneBase
{
public:
    SceneSelect();
    ~SceneSelect();

    void Init() override;
    virtual std::shared_ptr<SceneBase> Update(Input& input) override;
    void Draw() override;
    virtual void End() override;

    /// <summary>
    /// 次のシーン切り替え
    /// </summary>
    /// <param name="input">input</param>
    void SwitchingScene(Input& input);

private:
    int m_sceneSelectUI1;           // シーンセレクトUI1
    int m_sceneSelectUI2;           // シーンセレクトUI2
    int m_sceneSelectUI3;           // シーンセレクトUI3
    int m_sceneSelectUI4;           // シーンセレクトUI4
    int m_sceneSelectUI5;           // シーンセレクトUI5

    int m_nextSceneGrapgh;          // セレクト中のシーンの画像
    int m_nextSceneintroduction;    // セレクト中のシーンの説明



    enum class nextScene {
        None,           // 無し
        TitleScene,     // タイトルシーン
        GameScene,      // ゲームシーン
        OptionScene,    // オプションシーン
        RankingScene,   // ランキングシーン
        GameEnd,        // ゲーム終了
    }m_nextScene;

};