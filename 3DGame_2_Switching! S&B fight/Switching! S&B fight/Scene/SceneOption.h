#pragma once
#include "SceneBase.h"
#include <array>

class SceneOption :
    public SceneBase
{
public:
	SceneOption();
	virtual ~SceneOption();

	virtual void Init();
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();

	void DrawSelect();
	void DrawExplanation();

private:
	enum Select
	{
		kBgmVolume,   // BGM
		kSeVolume,    // SE
		kBack,
	};

	struct Cursor
	{
		VECTOR m_selectBox1;	// 四角始点
		VECTOR m_selectBox2;	// 四角終点
	}c1;

	struct Size             // 説明用画像のサイズ
	{
		int m_width;        // 横
		int m_height;       // 縦
	};

	// スクロール移動量
	float m_scrollX;

	std::array<int, 6> m_uiGraph{};	// UI用画像

	int m_select;


	// 背景クレジット
	// 著作者：<a href = "https://jp.freepik.com/free-photo/view-of-romantic-castle-bedroom_72464236.htm#query=%E5%9F%8E%E3%81%AE%E5%AF%9D%E5%AE%A4&position=0&from_view=keyword&track=ais_hybrid&uuid=99596e2f-76e1-482f-afa0-97f16ef53fab">Freepik< / a>
};

