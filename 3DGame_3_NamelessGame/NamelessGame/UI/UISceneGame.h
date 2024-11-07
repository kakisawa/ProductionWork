#pragma once
#include "DxLib.h"

class Player;
class UISceneGame
{
public:
	UISceneGame();
	virtual ~UISceneGame();

	void Init();
	void Update(Player& player);
	void Draw();
	void End();

private:
	int m_UI1;
	int m_cursorUI1;
	int m_cursorUI2;

	int m_itemBaseUI0;
	int m_itemBaseUI1;
	int m_itemBaseUI2;
	int m_itemBaseUI3;

	int m_displayBottomUI;

	int m_itemUI[3];

	VECTOR m_itemUI1Pos;
	VECTOR m_itemUI2Pos;
	VECTOR m_itemUI3Pos;

	VECTOR m_cursorUI1Pos;
	VECTOR m_cursorUI2Pos;
};

