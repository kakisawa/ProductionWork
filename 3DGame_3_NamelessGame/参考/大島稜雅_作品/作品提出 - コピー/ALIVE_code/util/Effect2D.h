#pragma once
#include <DxLib.h>
#include <vector>

class Effect2D
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">画像</param>
	/// <param name="size">描画サイズ</param>
	/// <param name="pos">描画場所</param>
	Effect2D(const std::vector<int>& handle,const float size,const VECTOR& pos);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Effect2D();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 画像を3D空間に描画する
	/// </summary>
	void DrawBillboard();

	/// <summary>
	/// 存在しているかどうか取得する
	/// </summary>
	/// <returns>true：している　false：していない</returns>
	bool IsEnable() const { return isEnabled_; }
private:

	int currentNum_ = 0;			//現在の番号

	float drawSize_ = 0.0f;			//描画サイズ

	bool isEnabled_ = false;		//存在しているか

	VECTOR drawPos_ = {};			//描画場所

	std::vector<int> handle_;		//画像ハンドル
};

