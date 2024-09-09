#pragma once
#include "SceneBase.h"
#include <array>

class SceneRanking :
    public SceneBase
{
public:
	SceneRanking();
	virtual ~SceneRanking();

	virtual void Init();
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();

	void DrawExplanation();


private:
	std::array<int, 4> m_uiGraph{};	// UI用画像

	float m_scrollX_1;		// スクロール移動量

	struct Size             // 説明用画像のサイズ
	{
		int m_width;        // 横
		int m_height;       // 縦
	};
};