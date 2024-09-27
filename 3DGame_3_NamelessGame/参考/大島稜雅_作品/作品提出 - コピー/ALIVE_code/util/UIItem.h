#pragma once
#include <string>
#include <DxLib.h>

class UIItem
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	UIItem();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~UIItem();

	/// <summary>
	/// サイズ調整、アルファ値の変更があるのUI描画
	/// </summary>
	/// <param name="scale">サイズ</param>
	/// <param name="alpha">アルファ値</param>
	void AlphaChangeDraw(const float scale, const int alpha);

	/// <summary>
	/// ポジションが変わるUIの描画
	/// </summary>
	/// <param name="centerPosX">ポジションX</param>
	/// <param name="centerPosY">ポジションY</param>
	void ChangePosDraw(const float centerPosX, const float centerPosY);

	/// <summary>
	/// 画像を3D空間に描画する
	/// </summary>
	/// <param name="drawPos">描画座標</param>
	/// <param name="alpha">アルファ値</param>
	/// <param name="size">サイズ</param>
	void DrawBillboard(const VECTOR& drawPos, const int alpha, const float size);

	/// <summary>
	/// UI画像を作成するための位置情報等を設定する
	/// </summary>
	/// <param name="centerPosX">ポジションのX</param>
	/// <param name="centerPosY">ポジションのY</param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="str">文字列</param>
	/// <param name="fontHandle">フォントハンドル</param>
	void CreateUIGraphSetUp(const float centerPosX, const  float centerPosY, const int width, const int height, const std::string& str, const int fontHandle);

	/// <summary>
	/// UI画像の作成
	/// </summary>
	/// <param name="fontHandle">フォントハンドル</param>
	void CreateUIGraph(const int fontHandle);

	/// <summary>
	/// 文字列を取得する
	/// </summary>
	/// <returns>文字列</returns>
	const std::string& GetString() const { return UIString_; }
private:
	
	int makeScreenHandle_ = 0;				//作成したスクリーン
	int makeScreenWidth_ = 0;				//スクリーンの横幅
	int makeScreenHeight_ = 0;				//スクリーンの縦幅

	float centerPosX_ = 0;					//中心位置X
	float centerPosY_ = 0;					//中心位置Y
	float scale_ = 1.0f;					//拡縮率

	std::string UIString_ = {};				//UIの文字列

};

