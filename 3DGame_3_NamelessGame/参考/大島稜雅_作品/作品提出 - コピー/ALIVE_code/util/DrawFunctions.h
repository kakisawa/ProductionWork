#pragma once

namespace Graph {

	///<summary>
	///グラフの読み込み
	///</summary>
	/// <param name="path">読み込みたいグラフのファイルパス</param>
	int LoadGraph(const char* path);

    /// <summary>
	/// 読み込んだグラフを引数の値によって分割描画する
	/// </summary>
	/// <param name="x,y">表示する座標XY</param>
	/// <param name="left,top">くり抜きたい画像の左上座標XY</param>
	/// <param name="width,height">画像１枚当たりの横幅、横幅のサイズ</param>
	/// <param name="scale">画像の拡大率</param>
	/// <param name="angle">画像の回転率</param>
	/// <param name="handle">読み込んだ画像</param>
	/// <param name="transFlag">透過するか</param>
	/// <param name="tumFlag">反転させるか</param>
	int DrawRectRotaGraph(const float x, const float y, const int left, const int top, const int width, const int height, const float scale, const float angle, const int handle, const  bool transFlag, const  bool tumFlag);

}

