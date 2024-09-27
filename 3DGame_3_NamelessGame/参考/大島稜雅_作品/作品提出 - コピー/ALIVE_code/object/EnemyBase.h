#pragma once

#include "PlayerData.h"
#include "CharacterBase.h"

#include <list>
#include <memory>
#include <unordered_map>

class Aster;
class ShotManager;

class EnemyBase : public CharacterBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="materialType">マテリアルのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	EnemyBase(const int handle, const Material materialType, const ObjectInfo& objInfo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EnemyBase();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void Update(Player& player) override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// プレイヤーを追跡する
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	void TrackingUpdate(const VECTOR& playerPos);

	/// <summary>
	/// プレイヤーを索敵する
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	void SearchForPlayer(const VECTOR& playerPos);

	/// <summary>
	/// ルート通りに移動する
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	void RoutingUpdate(const VECTOR& playerPos);

	/// <summary>
	/// 敵からプレイヤーの直線距離にオブジェクトがあるか
	/// </summary>
	/// <param name="playerPos">プレイヤーのポジション</param>
	/// <returns>オブジェクトがあるか</returns>
	bool IsThereAnObject(const VECTOR& playerPos);

	/// <summary>
	/// 敵が弾を撃つ処理
	/// </summary>
	/// <param name="shotManager">弾を管理するクラスのポインタ</param>
	/// <param name="playerPos">プレイヤーのポジション</param>
	/// <param name="height">プレイヤーの高さ</param>
	void Shot(const std::shared_ptr<ShotManager>& shotManager, const VECTOR& playerPos, const float height);

	/// <summary>
	/// 回転行列と拡縮行列を乗算した行列を取得する
	/// </summary>
	/// <returns>回転行列と拡縮行列を乗算した行列</returns>
	MATRIX CombiningRotAndScallMat(const VECTOR& distance);

	/// <summary>
	/// 落ち影の頂点のポジションを取得する
	/// </summary>
	/// <param name="angle">角度</param>
	/// <returns>ポジション</returns>
	VECTOR VertexPosition(const float angle);

	/// <summary>
	/// 落ち影もどきの描画
	/// </summary>
	void DrawPolygon3D();
protected:

	bool isDetection_ = false;						//プレイヤーを検知したか
	bool isThrow_ = false;							//投げるモーション中か

	VECTOR initFrontVec_ = {};						//初期正面ベクトル
	VECTOR frontVec_ = {};							//敵の正面ベクトルを入れる
	VECTOR pushVec_ = {};							//プレイヤーが敵にぶつかったときに押すベクトル

	std::shared_ptr<Aster> Aster_;					//Asterのポインタ
};

