#pragma once
#include "OrnamentBase.h"

class StreetLight final : public OrnamentBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="materialType">マテリアルのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	StreetLight(const int handle, const Material materialType, const ObjectInfo objInfo);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~StreetLight();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void Update(Player& player) override;

	/// <summary>
	/// プレイヤーの一定範囲内に入ったらライトを点灯する
	/// </summary>
	/// <param name="distanceSize">プレイヤーと街灯の距離の大きさ</param>
	void LightOn(float distanceSize);

private:
	int lightHandle_ = -1;		//ライトのハンドル
};

