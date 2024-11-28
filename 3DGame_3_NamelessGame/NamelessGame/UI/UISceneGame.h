#pragma once
#include "DxLib.h"
#include <vector>

class Player;
class UISceneGame
{
public:
	UISceneGame();
	virtual ~UISceneGame();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Init(const Player& player);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player"></param>
	void Update(const Player& player);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了
	/// </summary>
	void End();

	/// <summary>
	/// HP・スタミナバーUIの更新
	/// </summary>
	/// <param name="player"></param>
	void UpdateBarUI(const Player& player);

	/// <summary>
	/// アイテムUIの更新
	/// </summary>
	/// <param name="player"></param>
	void UpdateItemUI(const Player& player);

	/// <summary>
	/// 武器UIの更新
	/// </summary>
	/// <param name="player"></param>
	void UpdateWeaponUI(const Player& player);

	/// <summary>
	/// 選択中アイテム名UIの表示
	/// </summary>
	/// <param name="player">プレイヤー情報</param>
	void SetUI_SelectItem(const Player& player);

	/// <summary>
	/// プレイヤーが獲得したアイテムのUIを設定する
	/// </summary>
	/// <param name="player">プレイヤー情報</param>
	void SetUI_GetItem(const Player& player);

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
	
	int m_staminaBgUI;		// スタミナバー背景UI
	int m_staminaUI;		// スタミナバーUI
	int m_hpBgUI;			// HP背景UI
	int m_hpUI_Red;			// HPUI_赤
	int m_hpUI_Green;		// HPUI_緑

	int m_enemyHpBgUI;		// 敵HP背景UI
	int m_enemyHpUI;		// 敵HPUI


	int m_useWeaponChara;		// 使用中の武器の表示文字
	int m_useItemChara;			// 使用中のアイテムの表示文字
	
	float m_playerHp_Green;		// プレイヤーの緑HPバー表示用
	float m_playerHp_Red;		// プレイヤーの赤HPバー表示用

	int m_playerStamina;		// プレイヤーのスタミナバー表示用

	std::vector<int> m_itemUI;

	VECTOR m_cursorUI1Pos;		// 武器カーソルUI座標
	VECTOR m_cursorUI2Pos;		// アイテムカーソルUI座標
};

