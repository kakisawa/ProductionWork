#pragma once
#include "SceneBase.h"

/// <summary>
/// ポーズ画面
/// </summary>
class ScenePause : public SceneBase
{
public:
	ScenePause(std::shared_ptr<SceneBase> pScene);
	virtual ~ScenePause();
	virtual void Init() override;
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw() override;

private:
	std::shared_ptr<SceneBase> m_pPrevScene;	// 前に実行していたシーン

	// 選択項目
	enum Select
	{
		kBack,			// ゲームに戻る
		kOption,		// オプション
		kStageSelect,	// ステージ選択に戻る
		kSelectNum		// 選択数
	};
};

