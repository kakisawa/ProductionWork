#include "LightBulb.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/Easing.h"

#include <algorithm>

namespace 
{
	//角度を倍にする
	constexpr int angle_scale = 2;

	//時間調整用
	constexpr int time_adjustment = 4;

	//総時間
	constexpr float total_time = 360.0f;
}

//コンストラクタ
LightBulb::LightBulb(const int handle, const  ObjectInfo& info)
{
	//モデルの設定
	model_ = std::make_shared<Model>(handle,Material::Iron);
	model_->SetPos(info.pos);
	model_->SetRot(info.rot);
	model_->SetScale(info.scale);

	rot_ = info.rot;
}

//デストラクタ
LightBulb::~LightBulb()
{
}

//更新
void LightBulb::Update()
{
	//時間計測
	elapsedTime_ = (std::min)(elapsedTime_ + 1,total_time);

	//角度の取得
	angle_ = Easing::InOutSine(elapsedTime_, total_time, targetAngle_, angle_);

	if (elapsedTime_ >= total_time / time_adjustment)
	{
		//経過時間のリセット
		elapsedTime_ = 0.0f;

		//目標角度を変更する
		targetAngle_ -= targetAngle_ * angle_scale;
	}

	//Zの回転を弧度法に変更する
	rot_.z = MathUtil::DegreeToRadian(angle_);

	//回転の設定
	model_->SetRot(rot_);
}

//描画
void LightBulb::Draw()
{
	model_->Draw();
}

//回転ベクトルの取得
VECTOR LightBulb::GetRotVec() const
{
	MATRIX mtxRotZ = MGetRotZ(rot_.z);

	VECTOR frontVec = VTransform({ 0,-1,0 }, mtxRotZ);

	return frontVec;
}

//フレームのポジションの取得
VECTOR LightBulb::GetFramePos() const
{
	VECTOR pos = model_->GetFrameLocalPosition("LightBulb");
	return pos;
}
