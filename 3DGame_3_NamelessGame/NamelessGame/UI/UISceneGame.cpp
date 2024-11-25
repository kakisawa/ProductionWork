#include "UISceneGame.h"
#include "../Object/Player.h"
#include "DxLib.h"



namespace {
	const VECTOR kWeaponSelectPos[3]{	// 武器セレクトカーソルUI座標
		VGet(1741.0f, 108.0f,0.0f),		// 一つ目
		VGet(1741.0f,244.0f,0.0f),		// 二つ目
		VGet(1741.0f,380.0f,0.0f)		// 三つ目
	};

	const VECTOR kItemSelectPos[3]{		// アイテムセレクトカーソルUI座標
		VGet(1741.0f, 660.0f,0.0f),		// 一つ目
		VGet(1741.0f, 796.0f,0.0f),		// 二つ目
		VGet(1741.0f, 932.0f,0.0f)		// 三つ目
	};


	const VECTOR kItemPos[3]{			// アイテムUI座標
		 VGet(1768.0f, 690, 0.0f),		// 一つ目
		 VGet(1768.0f, 826, 0.0f),		// 二つ目
		 VGet(1768.0f, 962, 0.0f)		// 三つ目
	};

	VECTOR kDisplayBgPos[2]{			// 選択中のアイテム・武器文字背景UI座標
		VGet(1490.0f, 0.0f,0.0f),		// 武器
		VGet(1540.0f, 535.0f,0.0f)		// アイテム
	};

	const VECTOR kBarPos[2]{

	}

	VECTOR kDisplayItemPos = VGet(1650, 580, 0.0f);			// 選択中のアイテム文字UI座標

	VECTOR kDisplayWeaponPos = VGet(1610, 33, 0.0f);		// 選択中の武器文字UI座標

}

UISceneGame::UISceneGame() :
	m_UI1(-1),
	m_cursorUI1(-1),
	m_cursorUI2(-1),
	m_displayBgUI(-1),
	m_itemBaseUI0(-1),
	m_itemBaseUI1(-1),
	m_itemBaseUI2(-1),
	m_itemBaseUI3(-1),
	m_itemCharaUI0(-1),
	m_itemCharaUI1(-1),
	m_itemCharaUI2(-1),
	m_itemCharaUI3(-1),
	m_itemCharaUI4(-1),
	m_itemCharaUI5(-1),
	m_weaponCharaUI1(-1),
	m_weaponCharaUI2(-1),
	m_weaponCharaUI3(-1),
	m_staminaBgUI(-1),
	m_staminaUI(-1),
	m_hpBgUI(-1),
	m_hpUI_Green(-1),
	m_hpUI_Red(-1),
	m_useWeaponChara(0),
	m_useItemChara(0),
	m_cursorUI1Pos(kWeaponSelectPos[0]),
	m_cursorUI2Pos(kItemSelectPos[0])
{
	m_itemUI.resize(3,-1);
}

UISceneGame::~UISceneGame()
{
}

void UISceneGame::Init()
{
	m_UI1 = LoadGraph("Data/Image/SceneGame/アイテム・武器大本.png");
	m_cursorUI1 = LoadGraph("Data/Image/SceneGame/武器カーソル.png");
	m_cursorUI2 = LoadGraph("Data/Image/SceneGame/アイテムカーソル.png");

	m_itemBaseUI0 = LoadGraph("Data/Image/SceneGame/アイテム_仮.png");
	m_itemBaseUI1 = LoadGraph("Data/Image/SceneGame/アイテム_地雷.png");
	m_itemBaseUI2 = LoadGraph("Data/Image/SceneGame/アイテム_びっくり箱.png");
	m_itemBaseUI3 = LoadGraph("Data/Image/SceneGame/アイテム_回復薬.png");

	m_displayBgUI = LoadGraph("Data/Image/SceneGame/表示下.png");

	m_itemCharaUI0 = LoadGraph("Data/Image/SceneGame/文字UI_仮.png");
	m_itemCharaUI1 = LoadGraph("Data/Image/SceneGame/文字UI_地雷.png");
	m_itemCharaUI2 = LoadGraph("Data/Image/SceneGame/文字UI_びっくり箱.png");
	m_itemCharaUI3 = LoadGraph("Data/Image/SceneGame/文字UI_回復薬.png");
	m_itemCharaUI4 = LoadGraph("Data/Image/SceneGame/文字UI_氷床.png");
	m_itemCharaUI5 = LoadGraph("Data/Image/SceneGame/文字UI_回転椅子.png");

	m_weaponCharaUI1 = LoadGraph("Data/Image/SceneGame/文字UI_ハンドガン.png");
	m_weaponCharaUI2 = LoadGraph("Data/Image/SceneGame/文字UI_マシンガン.png");
	m_weaponCharaUI3 = LoadGraph("Data/Image/SceneGame/文字UI_ナイフ.png");

	m_staminaBgUI=LoadGraph("Data/Image/SceneGame/スタミナ背景バー.png");
	m_staminaUI=LoadGraph("Data/Image/SceneGame/スタミナ.png");
	m_hpBgUI= LoadGraph("Data/Image/SceneGame/HP背景バー.png");
	m_hpUI_Red= LoadGraph("Data/Image/SceneGame/HP_赤.png");
	m_hpUI_Green=LoadGraph("Data/Image/SceneGame/HP_緑.png");
}

void UISceneGame::Update(Player& player)
{

	// 所持アイテムが何もなかったら、UIも表示しないようにする
	for (int i = 0; i < 3; i++) {
		if (player.m_item[i] == Item::ItemKind::NoItem)
		{
			m_itemUI[i] = -1;
		}
	}

	// プレイヤーが獲得したアイテムのUIを設定する関数
	SetUI_GetItem(player);


	// プレイヤーの武器選択情報を獲得しての処理
	if (player.GetWeaponKind() == Player::WeaponKind::HandGun)
	{
		m_cursorUI1Pos = kWeaponSelectPos[0];	// カーソルの位置移動
		m_useWeaponChara = m_weaponCharaUI1;	// 選択中武器名UIの表示

	}
	if (player.GetWeaponKind() == Player::WeaponKind::MachineGun) {
		m_cursorUI1Pos = kWeaponSelectPos[1];	// カーソルの位置移動
		m_useWeaponChara = m_weaponCharaUI2;	// 選択中武器名UIの表示
	}

	if (player.GetWeaponKind() == Player::WeaponKind::Knife) {
		m_cursorUI1Pos = kWeaponSelectPos[2];	// カーソルの位置移動
		m_useWeaponChara = m_weaponCharaUI3;	// 選択中武器名UIの表示
	}



	// プレイヤーのアイテム選択情報を獲得しての処理

	for (int i = 0; i < 3; i++)
	{
		if (player.GetItemFrame() == i)
		{
			m_cursorUI2Pos = kItemSelectPos[i];		// カーソルの位置移動
		}
	}

	//if (player.GetItemFrame() == 0)
	//{
	//	m_cursorUI2Pos = kItemSelectPos[0];		// カーソルの位置移動
	//}
	//else if (player.GetItemFrame() == 1)
	//{
	//	m_cursorUI2Pos = kItemSelectPos[1];		// カーソルの位置移動
	//}
	//else if (player.GetItemFrame() == 2)
	//{
	//	m_cursorUI2Pos = kItemSelectPos[2];		// カーソルの位置移動
	//}

	// アイテムを所持していなかったらアイテム画像を消し、先には進まない
	if (player.item() == Item::ItemKind::NoItem) {
		m_useItemChara = -1;
		return;
	}

	SetUI_SelectItem(player);
}

void UISceneGame::Draw()
{
	// アイテム・武器大本UI
	DrawGraph(1740, 91, m_UI1, true);

	// 選択中のアイテム・武器名背景UI
	for (int i = 0; i < 2; i++)
	{
		DrawGraphF(kDisplayBgPos[0].x, kDisplayBgPos[0].y, m_displayBgUI, true);
		DrawGraphF(kDisplayBgPos[1].x, kDisplayBgPos[1].y, m_displayBgUI, true);
	}

	// 選択中の武器文字UI
	DrawGraphF(kDisplayWeaponPos.x, kDisplayWeaponPos.y, m_useWeaponChara, true);
	// 選択中のアイテム文字UI
	DrawGraphF(kDisplayItemPos.x, kDisplayItemPos.y, m_useItemChara, true);


	// 獲得したアイテムUI
	for (int i = 0; i < 3; i++)
	{
		DrawGraphF(kItemPos[0].x, kItemPos[0].y, m_itemUI[0], true);
		DrawGraphF(kItemPos[1].x, kItemPos[1].y, m_itemUI[1], true);
		DrawGraphF(kItemPos[2].x, kItemPos[2].y, m_itemUI[2], true);
	}

	// 選択中の武器カーソル
	DrawGraphF(m_cursorUI1Pos.x, m_cursorUI1Pos.y, m_cursorUI1, true);
	// 選択中のアイテムカーソル
	DrawGraphF(m_cursorUI2Pos.x, m_cursorUI2Pos.y, m_cursorUI2, true);
}

void UISceneGame::End()
{

	// 画像データの削除
	DeleteGraph(m_UI1);
	DeleteGraph(m_cursorUI1);
	DeleteGraph(m_cursorUI2);

	DeleteGraph(m_itemBaseUI0);
	DeleteGraph(m_itemBaseUI1);
	DeleteGraph(m_itemBaseUI2);
	DeleteGraph(m_itemBaseUI3);

	DeleteGraph(m_displayBgUI);

	DeleteGraph(m_itemCharaUI0);
	DeleteGraph(m_itemCharaUI1);
	DeleteGraph(m_itemCharaUI2);
	DeleteGraph(m_itemCharaUI3);
	DeleteGraph(m_itemCharaUI4);
	DeleteGraph(m_itemCharaUI5);

	DeleteGraph(m_staminaBgUI);
	DeleteGraph(m_staminaUI);
	DeleteGraph(m_hpBgUI);
	DeleteGraph(m_hpUI_Red);
	DeleteGraph(m_hpUI_Green);
}

void UISceneGame::SetUI_SelectItem(Player& player)
{
	// 選択中アイテム名UIの表示
	if (player.item() == Item::ItemKind::IceFloor)
	{
		m_useItemChara = m_itemCharaUI4;
	}
	if (player.item() == Item::ItemKind::SwivelChair)
	{
		m_useItemChara = m_itemCharaUI5;
	}
	if (player.item() == Item::ItemKind::landmine)
	{
		m_useItemChara = m_itemCharaUI1;
	}
	if (player.item() == Item::ItemKind::SurpriseBox)
	{
		m_useItemChara = m_itemCharaUI2;
	}
	if (player.item() == Item::ItemKind::RecoveryMedic)
	{
		m_useItemChara = m_itemCharaUI3;
	}

	// 後々消す予定
	if (player.item() == Item::ItemKind::Ammunition)
	{
		m_useItemChara = m_itemCharaUI0;	// まだやってない
	}
	if (player.item() == Item::ItemKind::SummonBox)
	{
		m_useItemChara = m_itemCharaUI0;	// まだやってない
	}
}

void UISceneGame::SetUI_GetItem(Player& player)
{
	for (int i = 0; i < 3; i++)
	{
		// アイテム枠
		if (player.m_item[i] == Item::ItemKind::IceFloor)
		{
			m_itemUI[i] = m_itemBaseUI0;		// 仮
		}
		else if (player.m_item[i] == Item::ItemKind::SwivelChair)
		{
			m_itemUI[i] = m_itemBaseUI0;		// 仮
		}
		else if (player.m_item[i] == Item::ItemKind::landmine)
		{
			m_itemUI[i] = m_itemBaseUI1;
		}
		else if (player.m_item[i] == Item::ItemKind::SurpriseBox)
		{
			m_itemUI[i] = m_itemBaseUI2;
		}
		else if (player.m_item[i] == Item::ItemKind::RecoveryMedic)
		{
			m_itemUI[i] = m_itemBaseUI3;
		}

		// 後で削除予定
		else if (player.m_item[i] == Item::ItemKind::Ammunition)
		{
			m_itemUI[i] = m_itemBaseUI0;		// 仮
		}
		else if (player.m_item[i] == Item::ItemKind::SummonBox)
		{
			m_itemUI[i] = m_itemBaseUI0;		// 仮
		}
	}
}