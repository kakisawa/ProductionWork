﻿#include "UISceneGame.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
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

	const VECTOR kBarPos[6]{			// バーUI座標
		VGet(220.0f,21.0f,0.0f),		// HP背景バー
		VGet(224.0f,28.0f,0.0f),		// HPバー(緑/赤)
		VGet(220.0f,84.0f,0.0f),		// スタミナ背景バー
		VGet(224.0f,89.0f,0.0f),		// スタミナバー
		VGet(343.0f,1022.0f,0.0f),		// 敵HP背景バー
		VGet(347.0f,1026.0f,0.0f),		// 敵HPバー
	};

	VECTOR kDisplayItemPos = VGet(1650, 580, 0.0f);			// 選択中のアイテム文字UI座標
	VECTOR kDisplayWeaponPos = VGet(1610, 33, 0.0f);		// 選択中の武器文字UI座標


	const char* const kItemCharaUI[9]{						// 文字UI画像
		"Data/Image/SceneGame/文字UI_仮.png",
		"Data/Image/SceneGame/文字UI_地雷.png",
		"Data/Image/SceneGame/文字UI_びっくり箱.png",
		"Data/Image/SceneGame/文字UI_回復薬.png",
		"Data/Image/SceneGame/文字UI_氷床.png",
		"Data/Image/SceneGame/文字UI_回転椅子.png",
		"Data/Image/SceneGame/文字UI_ハンドガン.png",
		"Data/Image/SceneGame/文字UI_マシンガン.png",
		"Data/Image/SceneGame/文字UI_ナイフ.png",
	};

	const char* const kPlayerToolUI[8]{
		"Data/Image/SceneGame/アイテム・武器大本.png",
		"Data/Image/SceneGame/武器カーソル.png",
		"Data/Image/SceneGame/アイテムカーソル.png",
		"Data/Image/SceneGame/表示下.png",
		"Data/Image/SceneGame/アイテム_仮.png",
		"Data/Image/SceneGame/アイテム_地雷.png",
		"Data/Image/SceneGame/アイテム_びっくり箱.png",
		"Data/Image/SceneGame/アイテム_回復薬.png",
	};

	const char* const kBarUI[7]{
		"Data/Image/SceneGame/HP背景バー.png",
		"Data/Image/SceneGame/HP_赤.png",
		"Data/Image/SceneGame/HP_緑.png",
		"Data/Image/SceneGame/スタミナ背景バー.png",
		"Data/Image/SceneGame/スタミナ.png",
		"Data/Image/SceneGame/敵HP背景バー.png",
		"Data/Image/SceneGame/敵HPバー.png",
	};
}

UISceneGame::UISceneGame() :
	m_useWeaponChara(0),
	m_useItemChara(0),
	m_playerHp_Green(1205),
	m_playerHp_Red(1205),
	m_playerStamina(0),
	m_enemyHp(1205),
	m_cursorUI1Pos(kWeaponSelectPos[0]),
	m_cursorUI2Pos(kItemSelectPos[0])
{
	// 文字UI画像読み込み
	for (int i = 0; i < m_itemCharaUIHnadle.size(); i++)
	{
		m_itemCharaUIHnadle[i] = LoadGraph(kItemCharaUI[i]);
	}

	// プレイヤー使用ツール用UI画像読み込み
	for (int i = 0; i < m_playerToolUIHandle.size(); i++)
	{
		m_playerToolUIHandle[i] = LoadGraph(kPlayerToolUI[i]);
	}

	// バーUI画像の削除
	for (int i = 0; i < m_barUIHandle.size(); i++)
	{
		m_barUIHandle[i] = LoadGraph(kBarUI[i]);
	}
}

UISceneGame::~UISceneGame()
{
}

void UISceneGame::Init(const Player& player, const Enemy& enemy)
{
	m_playerHp_Red = static_cast<float>(player.GetHp());
	m_enemyHp = static_cast<float>(enemy.GetHp());
}

void UISceneGame::Update(const Player& player,const Enemy& enemy)
{
	UpdateBarUI(player,enemy);
	
	UpdateItemUI(player);
	UpdateWeaponUI(player);

	SetUI_RemainingBullets(player);
}

void UISceneGame::Draw()
{
	// アイテム・武器大本UI
	DrawGraph(1740, 91, m_playerToolUIHandle[0], true);

	// HPバー・スタミナ背景バーUI
	DrawGraphF(kBarPos[0].x, kBarPos[0].y, m_barUIHandle[0], true);
	DrawGraphF(kBarPos[2].x, kBarPos[2].y, m_barUIHandle[3], true);
	// HPバー・スタミナバーUI
	DrawRectGraphF2(kBarPos[3].x, kBarPos[3].y, 0.0f, 0.0f,
		1172.0f, 37.0f, m_barUIHandle[4], true);
	DrawRectGraphF2(kBarPos[1].x, kBarPos[1].y, 0, 0,
		(1205.0f * (m_playerHp_Red * 0.01f)), 37.0f, m_barUIHandle[1], true);
	DrawRectGraphF2(kBarPos[1].x, kBarPos[1].y, 0.0f, 0.0f,
		(1205.0f * (m_playerHp_Green * 0.01f)), 37.0f, m_barUIHandle[2], true);

	// 敵HP背景バーUI
	DrawGraphF(kBarPos[4].x, kBarPos[4].y, m_barUIHandle[5], true);
	// 敵HPバー
	DrawRectGraphF2(kBarPos[5].x, kBarPos[5].y, 0.0f, 0.0f,
		(1205*(m_enemyHp*0.001f)),37.0f, m_barUIHandle[6], true);

	// 選択中のアイテム・武器名背景UI
	for (int i = 0; i < 2; i++)
	{
		DrawGraphF(kDisplayBgPos[i].x, kDisplayBgPos[i].y, m_playerToolUIHandle[3], true);
	}

	// 選択中の武器文字UI
	DrawGraphF(kDisplayWeaponPos.x, kDisplayWeaponPos.y, m_useWeaponChara, true);
	// 選択中のアイテム文字UI
	DrawGraphF(kDisplayItemPos.x, kDisplayItemPos.y, m_useItemChara, true);


	// 獲得したアイテムUI
	for (int i = 0; i < m_itemUI.size(); i++)
	{
		DrawGraphF(kItemPos[i].x, kItemPos[i].y, m_itemUI[i], true);
	}

	// 選択中の武器カーソル
	DrawGraphF(m_cursorUI1Pos.x, m_cursorUI1Pos.y, m_playerToolUIHandle[1], true);
	// 選択中のアイテムカーソル
	DrawGraphF(m_cursorUI2Pos.x, m_cursorUI2Pos.y, m_playerToolUIHandle[2], true);


	DrawFormatString(0, 0, 0xffffff, "m_playerRemainingBullets_handgun=%d", m_playerRemainingBullets_handgun);
	DrawFormatString(0, 20, 0xffffff, "m_playerRemainingBullets_machinegun=%d", m_playerRemainingBullets_machinegun);
}

void UISceneGame::End()
{
	// 画像データの削除

	// 文字UI画像の削除
	for (int i = 0; i < m_itemCharaUIHnadle.size(); i++)
	{
		DeleteGraph(m_itemCharaUIHnadle[i]);
	}

	// プレイヤー使用ツール用UI画像の削除
	for (int i = 0; i < m_playerToolUIHandle.size(); i++)
	{
		DeleteGraph(m_playerToolUIHandle[i]);
	}

	// バーUI画像の削除
	for (int i = 0; i < m_barUIHandle.size(); i++)
	{
		DeleteGraph(m_barUIHandle[i]);
	}
}

void UISceneGame::UpdateBarUI(const Player& player, const Enemy& enemy)
{
	// プレイヤーのHPバー管理
	m_playerHp_Green = static_cast<float>(player.GetHp());

	if (m_playerHp_Green < m_playerHp_Red)
	{
		m_playerHp_Red -= 0.5f;
	}

	// 敵のHPバー管理
	m_enemyHp = static_cast<float>(enemy.GetHp());
}

void UISceneGame::UpdateItemUI(const Player& player)
{
	// 所持アイテムが何もなかったら、UIも表示しないようにする
	for (int i = 0; i < m_itemUI.size(); i++) {
		if (player.m_item[i] == Item::ItemKind::NoItem)
		{
			m_itemUI[i] = -1;
		}
	}

	// プレイヤーが獲得したアイテムのUIを設定する関数
	SetUI_GetItem(player);

	// プレイヤーのアイテム選択情報を獲得しての処理

	for (int i = 0; i < 3; i++)
	{
		if (player.GetItemFrame() == i)
		{
			m_cursorUI2Pos = kItemSelectPos[i];		// カーソルの位置移動
		}
	}

	// アイテムを所持していなかったらアイテム画像を消し、先には進まない
	if (player.item() == Item::ItemKind::NoItem) {
		m_useItemChara = -1;
		return;
	}

	SetUI_SelectItem(player);
}

void UISceneGame::UpdateWeaponUI(const Player& player)
{
	// プレイヤーの武器選択情報を獲得しての処理
	if (player.GetWeaponKind() == Player::WeaponKind::HandGun)
	{
		m_cursorUI1Pos = kWeaponSelectPos[0];	// カーソルの位置移動
		m_useWeaponChara = m_itemCharaUIHnadle[6];	// 選択中武器名UIの表示

	}
	if (player.GetWeaponKind() == Player::WeaponKind::MachineGun) {
		m_cursorUI1Pos = kWeaponSelectPos[1];	// カーソルの位置移動
		m_useWeaponChara = m_itemCharaUIHnadle[7];	// 選択中武器名UIの表示
	}

	if (player.GetWeaponKind() == Player::WeaponKind::Knife) {
		m_cursorUI1Pos = kWeaponSelectPos[2];	// カーソルの位置移動
		m_useWeaponChara = m_itemCharaUIHnadle[8];	// 選択中武器名UIの表示
	}
}

void UISceneGame::SetUI_SelectItem(const Player& player)
{
	// 選択中アイテム名UIの表示
	if (player.item() == Item::ItemKind::IceFloor)
	{
		m_useItemChara = m_itemCharaUIHnadle[4];
	}
	if (player.item() == Item::ItemKind::SwivelChair)
	{
		m_useItemChara = m_itemCharaUIHnadle[5];
	}
	if (player.item() == Item::ItemKind::landmine)
	{
		m_useItemChara = m_itemCharaUIHnadle[1];
	}
	if (player.item() == Item::ItemKind::SurpriseBox)
	{
		m_useItemChara = m_itemCharaUIHnadle[2];
	}
	if (player.item() == Item::ItemKind::RecoveryMedic)
	{
		m_useItemChara = m_itemCharaUIHnadle[3];
	}

	// 後々消す予定
	if (player.item() == Item::ItemKind::Ammunition)
	{
		m_useItemChara = m_itemCharaUIHnadle[0];	// まだやってない
	}
	if (player.item() == Item::ItemKind::SummonBox)
	{
		m_useItemChara = m_itemCharaUIHnadle[0];	// まだやってない
	}
}

void UISceneGame::SetUI_GetItem(const Player& player)
{
	for (int i = 0; i < m_itemUI.size(); i++)
	{
		// アイテム枠
		if (player.m_item[i] == Item::ItemKind::IceFloor)
		{
			m_itemUI[i] = m_playerToolUIHandle[4];		// 仮
		}
		else if (player.m_item[i] == Item::ItemKind::SwivelChair)
		{
			m_itemUI[i] = m_playerToolUIHandle[4];		// 仮
		}
		else if (player.m_item[i] == Item::ItemKind::landmine)
		{
			m_itemUI[i] = m_playerToolUIHandle[5];
		}
		else if (player.m_item[i] == Item::ItemKind::SurpriseBox)
		{
			m_itemUI[i] = m_playerToolUIHandle[6];
		}
		else if (player.m_item[i] == Item::ItemKind::RecoveryMedic)
		{
			m_itemUI[i] = m_playerToolUIHandle[7];
		}

		// 後で削除予定
		else if (player.m_item[i] == Item::ItemKind::Ammunition)
		{
			m_itemUI[i] = m_playerToolUIHandle[4];		// 仮
		}
		else if (player.m_item[i] == Item::ItemKind::SummonBox)
		{
			m_itemUI[i] = m_playerToolUIHandle[4];		// 仮
		}
	}
}

void UISceneGame::SetUI_RemainingBullets(const Player& player)
{
	m_playerRemainingBullets_handgun = player.GetRemainingBullets_handgun();
	m_playerRemainingBullets_machinegun = player.GetRemainingBullets_machinegun();
}

void UISceneGame::SetUI_RemainingBulletsHandle(GunType type, int num, VECTOR pos)
{
	if (type == GunType::HandGun)
	{
		if (num / 100)
		{
			//balanceBullets[GunType::HandGun].m_playerHundredsHandle;
		}
		else if (num / 100 <= 0)
		{

		}
	}

	

}
