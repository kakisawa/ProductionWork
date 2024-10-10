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
    enum class nextScene {
        None,           // 無し
        TitleScene,     // タイトルシーン
        GameScene,      // ゲームシーン
        OptionScene,    // オプションシーン
        RankingScene,   // ランキングシーン
        GameEnd,        // ゲーム終了
    }m_nextScene;

};