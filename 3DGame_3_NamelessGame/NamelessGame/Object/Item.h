#pragma once
#include <array>
#include <DxLib.h>

namespace {
	constexpr int kItemKind = 7;	// アイテムの種類数
}

class Item
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Item();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Item();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 浮遊
	/// </summary>
	void Floating();



	// アイテムの種類
	enum class ItemKind {
		NoItem,			// アイテム無し
		IceFloor,		// 氷床
		SwivelChair,	// 回転椅子
		landmine,		// 地雷
		SurpriseBox,	// びっくり箱
		RecoveryMedic,	// 体力回復薬
		Ammunition,		// 弾薬
		SummonBox,		// 召喚BOX
	};

private:

	int m_model;	// アイテムのモデル

	VECTOR m_pos;	// アイテムの座標
};