#pragma once
#include "SceneBase.h"
#include <memory>

/// <summary>
/// クリアシーン
/// </summary>
class SceneClear : public SceneBase
{
public:
	SceneClear():m_totalClearTime(0),m_stageKind(0),m_textHandle(-1), m_clearBgModel(-1){};
	SceneClear(int stageKind, std::vector<int> clearTime);
	virtual ~SceneClear();
	void Init();
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();

private:
	int m_totalClearTime;			// トータルのクリアタイム
	std::vector<int> m_clearTime;	// クリアタイム
	int m_stageKind;				// クリアしたステージの種類
	int m_textHandle;				// テキストの画像
	int m_clearBgModel;				// クリア背景の3Dモデル
};
