#pragma once
#include "SceneBase.h"

class SceneStageBase;

/// <summary>
/// ゲームオーバーシーン
/// </summary>
class SceneGameover : public SceneBase
{
public:
	SceneGameover():m_textHandle(-1), m_backHandle(-1) {};
	SceneGameover(std::shared_ptr<SceneBase> pScene);
	virtual ~SceneGameover();
	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();

private:
	std::shared_ptr<SceneBase> m_pPrevScene;	// 前に実行していたシーン
	int m_textHandle;	// テキストの画像
	int m_backHandle;	// 背景の画像

	// 選択項目
	enum Select
	{
		kRetry,			// リトライ
		kStageSelect,	// ステージ選択
		kTitle,			// タイトル
		kSelectNum,		// 選択数
	};
};

