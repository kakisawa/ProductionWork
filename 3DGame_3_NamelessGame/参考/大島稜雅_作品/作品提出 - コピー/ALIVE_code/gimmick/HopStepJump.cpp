#include "HopStepJump.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/Easing.h"

#include <string>
#include <stdio.h>
#include <algorithm>

namespace 
{
	//加える値
	constexpr int add_select_num = 1;

	//選択番号の最小値
	constexpr int min_select_num = 0;

	//加える時間
	constexpr float add_time = 1.0f;

	//総時間
	constexpr float total_time = 180.0f;

	//時間の最小値
	constexpr float min_time = 0.0f;

	//アルファ値を変更する時間
	constexpr float alpha_change_time = 60.0f;

	//アルファ値の最大値
	constexpr float max_alpha_value = 1.0f;

	//アルファ値の最小値
	constexpr float min_alpha_value = 0.0f;
}

//コンストラクタ
HopStepJump::HopStepJump(const int handle, const Material materialType, const ObjectInfo& objInfo):GimmickBase(handle,materialType,objInfo)
{
	//マテリアルの数の取得
	materialNum_ = MV1GetMaterialNum(handle);

	//当たり判定の設定
	model_->SetUseCollision(true, true);

	//当たり判定の設定
	isCollCheck_ = true;

	//時間の設定
	elapseddTime_ = total_time;

	//現在の番号
	currentNum_ = StrUtil::GetNumberFromString(objInfo.name, ".");
}

//デストラクタ
HopStepJump::~HopStepJump()
{
}

//更新
void HopStepJump::Update(Player& player)
{
	//タイマー
	elapseddTime_ = (std::max)(elapseddTime_ - add_time, min_time);

	//現在の番号を変更する
	ChangeCurrentNum();

	//アルファ値を変更する
	ChangeAlphaValue();

	//マテリアルの色を変える
	ChangeMaterialColor();

	//現在の番号を文字列に変える
	std::string num = std::to_string(currentNum_);

	//当たり判定用フレームの変更
	model_->SetCollFrame(num.c_str());
}

//描画
void HopStepJump::Draw()
{
	MV1DrawFrame(model_->GetModelHandle(), model_->GetColFrameIndex());
}

//現在の番号を変更する
void HopStepJump::ChangeCurrentNum()
{
	//現在の番号を変更する
	if (elapseddTime_ == min_time)
	{
		currentNum_ = (std::min)(currentNum_ + add_select_num, materialNum_);
		elapseddTime_ = total_time;
	}

	//マテリアルの数を超えたら0にする
	if (currentNum_ == materialNum_)
	{
		currentNum_ = min_select_num;
	}
}

//アルファ値を変更する
void HopStepJump::ChangeAlphaValue()
{
	//アルファ値を変更する
	if (elapseddTime_ >= total_time - alpha_change_time)
	{
		alphaValue_ = (std::min)(alphaValue_ + (max_alpha_value / alpha_change_time), max_alpha_value);
	}
	else if (elapseddTime_ <= alpha_change_time)
	{
		alphaValue_ = (std::max)(alphaValue_ - (max_alpha_value / alpha_change_time), min_alpha_value);
	}
}

//マテリアルの色を変える
void HopStepJump::ChangeMaterialColor()
{
	COLOR_F color = {};

	//現在の番号以外のマテリアルのカラーのアルファ値を0にする
	for (int i = 0; i < materialNum_; i++)
	{
		//マテリアルの色を取得する
		color = MV1GetMaterialDifColor(model_->GetModelHandle(), i);

		//マテリアルのアルファ値を変更する
		if (currentNum_ == i)
		{
			color.a = alphaValue_;
		}
		else
		{
			color.a = min_alpha_value;
		}

		//マテリアルの色を設定する
		MV1SetMaterialDifColor(model_->GetModelHandle(), i, color);
	}
}
