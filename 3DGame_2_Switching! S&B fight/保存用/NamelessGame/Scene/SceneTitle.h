#pragma once
#include "SceneBase.h"

using namespace std;
class SceneTitle :
    public SceneBase
{
public:
	SceneTitle();
	virtual ~SceneTitle();

	virtual void Init();
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();

	/// <summary>
	/// Ÿ‚ÌƒV[ƒ“‚ğ‘I‘ğ
	/// </summary>
	void SwitchingScene();

	enum class nextScene
	{
		kNone,
		kGameScene,
		kOptionScene,
		kRankingScene,
	};

private:
	int m_titleGraph;
	int graph;
	int graph2;
	int graph3;

	

	nextScene m_nextScene;
};
