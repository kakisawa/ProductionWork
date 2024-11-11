#include "UISceneGame.h"
#include "../Object/Player.h"
#include "DxLib.h"

namespace {
	const VECTOR kWeaponSelectPos[3]{	// 武器セレクトUI座標
		VGet(1741.0f, 108.0f,0.0f),
		VGet(1741.0f,244.0f,0.0f),
		VGet(1741.0f,380.0f,0.0f)
	};
	
	const VECTOR kItemSelectPos[3]{		// アイテムセレクトUI座標
		VGet(1741.0f, 660.0f,0.0f),
		VGet(1741.0f, 796.0f,0.0f),
		VGet(1741.0f, 932.0f,0.0f)
	};


	const VECTOR kItemPos[3]{			// アイテムUI座標
		 VGet(1768.0f, 690, 0.0f),
		 VGet(1768.0f, 826, 0.0f),
		 VGet(1768.0f, 962, 0.0f)
	};

	VECTOR kDisplayBottom[2]{
		VGet(1547.0f, 0.0f,0.0f),
		VGet(1564.0f, 540.0f,0.0f)
	};

	//VECTOR kDisp
}

UISceneGame::UISceneGame():
	m_UI1(-1),
	m_cursorUI1(-1),
	m_cursorUI1Pos(kWeaponSelectPos[0]),
	m_cursorUI2Pos(kItemSelectPos[0])
{
}

UISceneGame::~UISceneGame()
{
}

void UISceneGame::Init()
{
	m_UI1 = LoadGraph("Data/Image/UI/アイテム・武器大本.png");
	m_cursorUI1 = LoadGraph("Data/Image/UI/武器カーソル.png");
	m_cursorUI2 = LoadGraph("Data/Image/UI/アイテムカーソル.png");

	m_itemBaseUI0 = LoadGraph("Data/Image/UI/アイテム_仮.png");
	m_itemBaseUI1= LoadGraph("Data/Image/UI/アイテム_地雷.png");
	m_itemBaseUI2 = LoadGraph("Data/Image/UI/アイテム_びっくり箱.png");
	m_itemBaseUI3 = LoadGraph("Data/Image/UI/アイテム_回復薬.png");
	
	m_displayBottomUI = LoadGraph("Data/Image/UI/表示下.png");
}

void UISceneGame::Update(Player& player)
{
	if (player.GetWeaponKind() == Player::WeaponKind::HandGun)
	{
		m_cursorUI1Pos = kWeaponSelectPos[0];
	}
	else if (player.GetWeaponKind() == Player::WeaponKind::MachineGun) {
		m_cursorUI1Pos = kWeaponSelectPos[1];

	}
	else if (player.GetWeaponKind() == Player::WeaponKind::Knife) {
		m_cursorUI1Pos = kWeaponSelectPos[2];
	}



	if (player.GetItemFrame() == 0)
	{
		m_cursorUI2Pos = kItemSelectPos[0];
	}
	else if (player.GetItemFrame() == 1) {
		m_cursorUI2Pos = kItemSelectPos[1];

	}
	else if (player.GetItemFrame() == 2) {
		m_cursorUI2Pos = kItemSelectPos[2];
	}



	for (int i = 0; i < 3; i++) {
		if (player.m_useItem[i] == Item::ItemKind::NoItem)
		{
			m_itemUI[i] = -1;
		}
	}
	

	// アイテム枠1
	if (player.m_useItem[0] == Item::ItemKind::IceFloor)
	{
		// 仮
		m_itemUI[0] = m_itemBaseUI0;
	}
	else if (player.m_useItem[0] == Item::ItemKind::SwivelChair)
	{
		// 仮
		m_itemUI[0] = m_itemBaseUI0;
	}
	else if (player.m_useItem[0] == Item::ItemKind::landmine)
	{
		
		m_itemUI[0] = m_itemBaseUI1;
	}
	else if (player.m_useItem[0] == Item::ItemKind::SurpriseBox)
	{
		m_itemUI[0] = m_itemBaseUI2;
	}
	else if (player.m_useItem[0] == Item::ItemKind::RecoveryMedic)
	{
		m_itemUI[0] = m_itemBaseUI3;
	}
	else if (player.m_useItem[0] == Item::ItemKind::Ammunition)
	{
		// 仮
		m_itemUI[0] = m_itemBaseUI0;
	}
	else if (player.m_useItem[0] == Item::ItemKind::SummonBox)
	{
		// 仮
		m_itemUI[0] = m_itemBaseUI0;
	}
	
	// アイテム枠2
	if (player.m_useItem[1] == Item::ItemKind::IceFloor)
	{
		// 仮
		m_itemUI[1] = m_itemBaseUI0;
	}
	else if (player.m_useItem[1] == Item::ItemKind::SwivelChair)
	{
		// 仮
		m_itemUI[1] = m_itemBaseUI0;
	}
	else if (player.m_useItem[1] == Item::ItemKind::landmine)
	{
		
		m_itemUI[1] = m_itemBaseUI1;
	}
	else if (player.m_useItem[1] == Item::ItemKind::SurpriseBox)
	{
		m_itemUI[1] = m_itemBaseUI2;
	}
	else if (player.m_useItem[1] == Item::ItemKind::RecoveryMedic)
	{
		m_itemUI[1] = m_itemBaseUI3;
	}
	else if (player.m_useItem[1] == Item::ItemKind::Ammunition)
	{
		// 仮
		m_itemUI[1] = m_itemBaseUI0;
	}
	else if (player.m_useItem[1] == Item::ItemKind::SummonBox)
	{
		// 仮
		m_itemUI[1] = m_itemBaseUI0;
	}

	// アイテム枠3
	if (player.m_useItem[2] == Item::ItemKind::IceFloor)
	{
		// 仮
		m_itemUI[2] = m_itemBaseUI0;
	}
	else if (player.m_useItem[2] == Item::ItemKind::SwivelChair)
	{
		// 仮
		m_itemUI[2] = m_itemBaseUI0;
	}
	else if (player.m_useItem[2] == Item::ItemKind::landmine)
	{
		m_itemUI[2] = m_itemBaseUI1;
	}
	else if (player.m_useItem[2] == Item::ItemKind::SurpriseBox)
	{
		m_itemUI[2] = m_itemBaseUI2;
	}
	else if (player.m_useItem[2] == Item::ItemKind::RecoveryMedic)
	{
		m_itemUI[2] = m_itemBaseUI3;
	}
	else if (player.m_useItem[2] == Item::ItemKind::Ammunition)
	{
		// 仮
		m_itemUI[2] = m_itemBaseUI0;
	}
	else if (player.m_useItem[2] == Item::ItemKind::SummonBox)
	{
		// 仮
		m_itemUI[2] = m_itemBaseUI0;
	}

}

void UISceneGame::Draw()
{
	DrawGraph(1740, 91, m_UI1, true);
	
	DrawGraph(kDisplayBottom[0].x, kDisplayBottom[0].y, m_displayBottomUI, true);
	DrawGraph(kDisplayBottom[1].x, kDisplayBottom[1].y, m_displayBottomUI, true);

	DrawGraph(itemPos1.x, itemPos1.y, m_itemUI[0], true);
	DrawGraph(itemPos2.x, itemPos2.y, m_itemUI[1], true);
	DrawGraph(itemPos3.x, itemPos3.y, m_itemUI[2], true);

	DrawGraph(m_cursorUI1Pos.x, m_cursorUI1Pos.y, m_cursorUI1, true);
	DrawGraph(m_cursorUI2Pos.x, m_cursorUI2Pos.y, m_cursorUI2, true);
}

void UISceneGame::End()
{
	DeleteGraph(m_UI1);
	DeleteGraph(m_cursorUI1);
	DeleteGraph(m_cursorUI2);

	DeleteGraph(m_itemBaseUI0);
	DeleteGraph(m_itemBaseUI1);
	DeleteGraph(m_itemBaseUI2);
	DeleteGraph(m_itemBaseUI3);

	DeleteGraph(m_displayBottomUI);
}
