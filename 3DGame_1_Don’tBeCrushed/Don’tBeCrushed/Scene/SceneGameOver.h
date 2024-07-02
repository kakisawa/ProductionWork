#pragma once
#include "SceneBase.h"

using namespace std;
class SceneGameOver :
	public SceneBase
{
public:
	SceneGameOver();
	virtual ~SceneGameOver();

	virtual void Init() {}
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End() {}

private:
	int graph;
	int m_fadeAlpha;        // フェードイン、アウト

	bool m_isFadeIn;		// フェードイン用のフラグ
	bool m_isFadeOut;		// フェードアウト用のフラグ
	bool m_isSceneEnd;		// シーン遷移用のフラグ
};