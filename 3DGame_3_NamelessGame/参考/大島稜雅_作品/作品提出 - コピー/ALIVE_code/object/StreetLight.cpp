#include "StreetLight.h"

#include "Player.h"

#include "../util/Util.h"
#include "../util/Model.h"

#include <string>
#include <DxLib.h>

namespace
{
	//ライトの外側の角度
	constexpr float light_outer_angle = 130.0f;

	//ライトの内側の角度
	constexpr float light_inner_angle = 60.0f;

	//ライトの範囲
	constexpr float light_range = 1000.0f;

	//距離関係なく減衰するパラメーター
	constexpr float attenuation_regardless_of_distance = 0.0f;

	//距離に比例して減衰するパラメーター
	constexpr float attenuation_proportional_to_distance = 0.0009f;

	//距離の2乗に比例して減衰するパラメーター
	constexpr float attenuation_proportional_to_the_square_of_the_distance = 0.0f;

	//ライトを有効にする距離
	constexpr float light_enable_distance = 2500.0f;

	//ライトを置くフレームの名前
	const std::string light_put_frame_name = "LightPos";

	//ライトの方向
	constexpr VECTOR light_dir = { 0.0f, -1.0f, 0.0f };

	//スポットライトの色
	constexpr COLOR_F spot_light_color = { 0.87f, 0.72f, 0.52f, 1.0f };
}

//コンストラクタ
StreetLight::StreetLight(const int handle, const Material materialType, const ObjectInfo objInfo) : OrnamentBase(handle, materialType, objInfo)
{
	//ライトの位置を取得
	VECTOR lightPos = model_->GetFrameLocalPosition(light_put_frame_name);

	//ライトの外側の角度をラジアンに変換
	float outerAngle = MathUtil::DegreeToRadian(light_outer_angle);

	//ライトの内側の角度をラジアンに変換
	float innerAngle = MathUtil::DegreeToRadian(light_inner_angle);

	//スポットライトのハンドルを作成
	lightHandle_ = CreateSpotLightHandle(lightPos,light_dir,
										 outerAngle, innerAngle,
										 light_range,
										 attenuation_regardless_of_distance,
										 attenuation_proportional_to_distance,
										 attenuation_proportional_to_the_square_of_the_distance);
	//スポットライトの色を設定
	SetLightDifColorHandle(lightHandle_, spot_light_color);

	//ライトを無効にする
	SetLightEnableHandle(lightHandle_, false);
}

//デストラクタ
StreetLight::~StreetLight()
{
	//ライトのハンドルを削除
	DeleteLightHandle(lightHandle_);
}

//更新
void StreetLight::Update(Player& player)
{
	//ライトの位置を取得
	VECTOR lightPos = model_->GetFrameLocalPosition(light_put_frame_name);

	//ライトの位置を更新
	SetLightPositionHandle(lightHandle_, lightPos);

	//プレイヤーの位置を取得
	VECTOR playerPos = player.GetStatus().pos;

	//プレイヤーとライトの距離を取得
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(pos_, playerPos);

	//ライトを点灯する
	LightOn(distanceSize);
}

//プレイヤーの一定範囲内に入ったらライトを点灯する
void StreetLight::LightOn(float distanceSize)
{
	//プレイヤーとライトの距離がライトを有効にする距離より大きい場合
	if (distanceSize > light_enable_distance)
	{
		//ライトを無効にする
		SetLightEnableHandle(lightHandle_, false);
	}
	else
	{
		//ライトを有効にする
		SetLightEnableHandle(lightHandle_, true);
	}
}
