﻿#pragma once
#include "SceneBase.h"
class Fade;
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

    /// <summary>
    /// カーソル情報の変更
    /// </summary>
    /// <param name="num"></param>
    void ChangeCursorInfo(int num);

private:
    int m_sceneSelectGraph;             // 選択中のセレクト画像情報を入れる用
    int m_selectGraphX, m_selectGraphY; // 選択中のセレクト画像のサイズ獲得用

    int m_sceneSelectUI1;           // シーンセレクトUI1
    int m_sceneSelectUI2;           // シーンセレクトUI2
    int m_sceneSelectUI3;           // シーンセレクトUI3
    int m_sceneSelectUI4;           // シーンセレクトUI4
    int m_sceneSelectUI5;           // シーンセレクトUI5

    int m_nextSceneGrapgh;          // セレクト中のシーンの画像
    int m_nextSceneintroduction;    // セレクト中のシーンの説明

    int m_cursorUI;                 // カーソル画像


    struct Cursor			// 選択中のセレクト
    {
        VECTOR m_selectBox1;	// 四角始点
        VECTOR m_selectBox2;	// 四角終点
    }c1;



    enum class nextScene {
        None,               // 無し
        TitleScene,         // タイトルシーン
        GameScene,          // ゲームシーン
        ExplanationScene,   // 操作説明シーン
        OptionScene,        // オプションシーン
        RankingScene,       // ランキングシーン
        GameEnd,            // ゲーム終了
    }m_nextScene;

    std::shared_ptr<Fade> m_pFade = std::make_shared<Fade>();
};