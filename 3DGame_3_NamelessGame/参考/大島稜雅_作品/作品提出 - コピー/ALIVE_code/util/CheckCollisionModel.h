#pragma once
#include <DxLib.h>
#include <memory>
#include <vector>
#include <list>
#include <deque>

class Player;
class Model;
class ObjectManager;

namespace {
	//最大衝突数
	constexpr int max_hit_coll = 2048;
}

class CheckCollisionModel
{
private:

	struct CollModelState {
		MV1_COLL_RESULT_POLY_DIM hitDim = {};		//108byte
		std::shared_ptr<Model> model = {};			//わからん
	};

	struct CollResultPoly {
		MV1_COLL_RESULT_POLY* hitDim = {};			//104byte
		std::shared_ptr<Model> model = {};			//わからん
	};

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CheckCollisionModel();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CheckCollisionModel();

	/// <summary>
	/// 自分から一定範囲のオブジェクトとの衝突判定を得る
	/// </summary>
	/// <param name="player">プレイヤーを参照</param>
	/// <param name="moveVec">プレイヤーの移動量</param>
	/// <param name="models">プレイヤーと衝突判定を行うモデル</param>
	void CheckCollisionPersonalArea(const std::shared_ptr<Player>& player, const std::shared_ptr<ObjectManager>& objManager);

	/// <summary>
	/// 衝突した壁と床のポリゴン数を数える
	/// </summary>
	void CheckWallAndFloor();

	/// <summary>
	/// 衝突したモデルのポリゴンが壁かを判断し、移動ベクトルを補正する
	/// </summary>
	/// <param name="player">プレイヤーを参照</param>
	void CheckCollisionWall(const std::shared_ptr<Player>& player);

	/// <summary>
	/// 移動後座標で壁との衝突判定
	/// </summary>
	/// <param name="j">回数</param>
	/// <param name="player">プレイヤーを参照</param>
	void CollisionDetectionWithWallAfterMovement(int& j, const std::shared_ptr<Player>& player);

	/// <summary>
	/// 衝突したモデルのポリゴンが床かを判断する
	/// </summary>
	/// <param name="player">プレイヤーを参照</param>
	/// <param name="moveVec">プレイヤーの移動量</param>
	/// <param name="jumpFlag">プレイヤーのジャンプフラグ</param>
	/// <param name="playerHeight">プレイヤーの高さ</param>
	void CheckCollisionFloor(const std::shared_ptr<Player>& player);

	/// <summary>
	/// checkCollisionPersonalArea、checkCollisionWall、checkCollisionFloorを呼び出す。
	/// 最後、上記の関数で取得した値を消去する
	/// </summary>
	/// <param name="player">プレイヤーを参照する</param>
	void CheckCollision(const std::shared_ptr<Player>& player, const std::shared_ptr<ObjectManager>& objManager);

	/// <summary>
	/// 衝突したオブジェクトが乗り越えられるか判断する
	/// </summary>
	void CheckStepDifference(const std::shared_ptr<Player>& player);

	/// <summary>
	/// プレイヤーの下に影もどきを描画したいために
	/// プレイヤーの真下の一番近いポリゴンの高さを取得する
	/// </summary>
	/// <param name="player">プレイヤーのポインタ</param>
	/// <param name="objManager">オブジェクトマネージャーのポインタ</param>
	void FindThePolygonBelowThePlayer(const std::shared_ptr<Player>& player, const std::shared_ptr<ObjectManager>& objManager);

	/// <summary>
	/// 死体との衝突判定
	/// </summary>
	/// <param name="player">プレイヤーのポインタ</param>
	/// <param name="objManager">オブジェクトマネージャーのポインタ</param>
	void CheckCollCorpseModel(const std::shared_ptr<Player>& player, const std::shared_ptr<ObjectManager>& objManager);

private:

	int hitWallNum = 0;									//壁と衝突したポリゴンの数
	int hitFloorNum = 0;								//床と衝突したポリゴンの数

	float objectHeightY_ = 0;							//衝突したオブジェクトの高さを保管する

	bool isGoUpStep_ = false;							//段差を上ることが出来る
	bool moveFlag = false;								//現在移動しているかのフラグ
	bool hitFlag_ = false;								//衝突したかのフラグ

	VECTOR oldPos = {};									//現在のプレイヤーの座標
	VECTOR nowPos_ = {};									//プレイヤーの移動量と現在の座標を足して結果

	//モデルとの当たり判定用メソッド
	std::list<CollModelState> hitDim_;					//球とモデルの衝突判定用
	std::list<HITRESULT_LINE> hitLineResult_ = {};		//ポジゴンと線の衝突判定用

	CollResultPoly wallHitDim_[max_hit_coll] = {};		//壁の衝突結果用配列
	CollResultPoly floorHitDim_[max_hit_coll] = {};		//床の衝突結果用配列
};

