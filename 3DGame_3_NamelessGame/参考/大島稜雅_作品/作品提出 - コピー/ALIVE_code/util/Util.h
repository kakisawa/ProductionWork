#pragma once
#include <DxLib.h>
#include <string>

namespace MathUtil 
{
	/// <summary>
	/// 二点間の距離のサイズを取得する
	/// </summary>
	/// <param name="pos">ポジション</param>
	/// <param name="targetPos">目標のポジション</param>
	/// <returns>二つのポジションの距離のサイズ</returns>
	float GetSizeOfDistanceTwoPoints(const VECTOR& pos, const VECTOR& targetPos);

	/// <summary>
	/// VECTOR型、度数法を弧度法に変換する
	/// </summary>
	/// <param name="rot">回転</param>
	/// <returns>弧度法に直した回転</returns>
	VECTOR VECTORDegreeToRadian(VECTOR rot);

	/// <summary>
	/// VECTOR型、弧度法を度数法に変換する
	/// </summary>
	/// <param name="rot">回転</param>
	/// <returns>度数法に直した回転</returns>
	VECTOR VECTORRadianToDegree(VECTOR rot);

	/// <summary>
	/// float型、度数法を弧度法に変換する
	/// </summary>
	/// <param name="angle">角度</param>
	/// <returns>弧度</returns>
	float DegreeToRadian(float angle);

	/// <summary>
	/// float型、弧度法を度数法に変換する
	/// </summary>
	/// <param name="angle">角度</param>
	/// <returns>度数</returns>
	float RadianToDegree(float angle);
}

namespace StrUtil 
{
	/// <summary>
	/// 文字列から数値を取得する
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="sign">記号</param>
	/// <returns>数値</returns>
	int GetNumberFromString(const std::string& name,const std::string& sign);

	/// <summary>
	/// 文字列と数値を結合した文字列を取得する
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="sign">記号</param>
	/// <param name="num">数値</param>
	/// <returns>文字列</returns>
	std::string GetConcatenateNumAndStrings(const std::string& name,const std::string& sign,const int num);

	/// <summary>
	/// 文字列からsign以降の文字列を削除した文字列を取得する
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="sign">記号</param>
	/// <returns>文字列</returns>
	std::string GetStringWithPartsAfterTheSymbolDeleted(const std::string& name, const std::string& sign);

	/// <summary>
	/// 文字列から記号以降の文字列を取得する
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="sign">記号</param>
	/// <returns>文字列</returns>
	std::string GetStringAfterSign(const std::string& name,const std::string& sign);

	/// <summary>
	/// 文字列から記号以前の文字列を取得する
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="sign">記号</param>
	/// <returns>文字列</returns>
	std::string GetStringBeforeSign(const std::string& name,const std::string& sign);
}