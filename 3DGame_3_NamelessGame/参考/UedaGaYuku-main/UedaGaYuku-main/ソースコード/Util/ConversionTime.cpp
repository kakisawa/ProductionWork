#include "ConversionTime.h"

namespace Conversion
{
	/// <summary>
	/// 分に変換する
	/// </summary>
	/// <param name="frame">フレーム数</param>
	/// <returns>分</returns>
	int ChangeMin(int frame)
	{
		return (frame / 60) / 60;
	}

	/// <summary>
	/// 秒に変換する
	/// </summary>
	/// <param name="frame">フレーム数</param>
	/// <returns>秒</returns>
	int ChangeSec(int frame)
	{
		return (frame / 60) % 60;
	}

	/// <summary>
	/// ミリ秒に変換する
	/// </summary>
	/// <param name="frame">フレーム数</param>
	/// <returns>ミリ秒</returns>
	int ChangeMilliSec(int frame)
	{
		return (frame * 1000 / 60) % 1000;
	}
}