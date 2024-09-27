#include "Effect2D.h"
#include <algorithm>

namespace
{
	//総時間
	constexpr int total_time = 60;

	//中心座標
	constexpr float center_pos_x = 0.5f;
	constexpr float center_pos_y = 0.5f;
}

//コンストラクタ
Effect2D::Effect2D(const std::vector<int>& handle,const float size,const VECTOR& pos)
{
	//ハンドルの取得
	handle_ = handle;

	//存在しているフラグをたてる
	isEnabled_ = true;

	//描画サイズ
	drawSize_ = size;

	//描画場所
	drawPos_ = pos;
}

//デストラクタ
Effect2D::~Effect2D()
{
}

//更新
void Effect2D::Update()
{
	//画像の更新
	currentNum_ = (std::min)(currentNum_ + 1, total_time);

	//現在の番号が画像の枚数と同数になったら存在を消す
	if (currentNum_ == handle_.size() - 1) {
		isEnabled_ = false;
	}
}

//画像を3D空間に描画する
void Effect2D::DrawBillboard()
{
	DrawBillboard3D(drawPos_, center_pos_x, center_pos_y, drawSize_, 0.0f, handle_[currentNum_], true);
}
