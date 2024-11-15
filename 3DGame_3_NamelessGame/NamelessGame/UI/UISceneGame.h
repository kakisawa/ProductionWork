#pragma once
#include "DxLib.h"
#include <vector>

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

	/// <summary>
	/// 選択中アイテム名UIの表示
	/// </summary>
	/// <param name="player">プレイヤー情報</param>
	void SetUI_SelectItem(Player& player);

	/// <summary>
	/// プレイヤーが獲得したアイテムのUIを設定する
	/// </summary>
	/// <param name="player">プレイヤー情報</param>
	void SetUI_GetItem(Player& player);

private:
	int m_UI1;				// アイテム・武器大本UI
	int m_cursorUI1;		// 選択中の武器カーソルUI
	int m_cursorUI2;		// 選択中のアイテムカーソルUI

	int m_itemBaseUI0;		// (仮)アイテムUI
	int m_itemBaseUI1;		// 地雷UI
	int m_itemBaseUI2;		// びっくり箱UI
	int m_itemBaseUI3;		// 回復薬UI

	int m_displayBgUI;		// 選択中のアイテム・武器文字背景UI

	int m_itemCharaUI0;		// (仮)文字UI
	int m_itemCharaUI1;		// 地雷文字UI
	int m_itemCharaUI2;		// びっくり箱文字UI
	int m_itemCharaUI3;		// 回復薬文字UI
	int m_itemCharaUI4;		// 氷床文字UI
	int m_itemCharaUI5;		// 回転椅子文字UI

	int m_weaponCharaUI1;	// ハンドガン文字UI
	int m_weaponCharaUI2;	// マシンガン文字UI
	int m_weaponCharaUI3;	// ナイフ文字UI

	int m_useWeaponChara;	// 使用中の武器の表示文字
	int m_useItemChara;		// 使用中のアイテムの表示文字
	//int m_itemUI;		// 所持できるアイテムの量(3つ)

	std::vector<int> m_itemUI;

	VECTOR m_cursorUI1Pos;	// 武器カーソルUI座標
	VECTOR m_cursorUI2Pos;	// アイテムカーソルUI座標
};

