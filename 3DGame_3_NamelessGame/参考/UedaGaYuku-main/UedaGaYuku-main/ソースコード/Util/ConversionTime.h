#pragma once

// フレーム数からタイムに変換する
namespace Conversion
{
	int ChangeMin(int frame);		// 分に変換する
	int ChangeSec(int frame);		// 秒に変換する
	int ChangeMilliSec(int frame);	// ミリ秒に変換する
}