#pragma once
#include <list>
#include <memory>
#include <DxLib.h>

class Model;
class Shot;
class Player;

class ShotManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ShotManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ShotManager();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 弾がプレイヤーと衝突したかチェックする
	/// </summary>
	/// <param name="player"></param>
	void Hit(Player& player);

	/// <summary>
	/// 弾を撃つ
	/// </summary>
	/// <param name="framePos">初期ポジション</param>
	/// <param name="playerPos">プレイヤーのポジション</param>
	/// <param name="height">プレイヤーの高さ</param>
	void Fire(const VECTOR& framePos, const VECTOR& playerPos, const float height);

private:
	std::list<std::shared_ptr<Shot>> shots_;		//モデルポインタのリスト
};

