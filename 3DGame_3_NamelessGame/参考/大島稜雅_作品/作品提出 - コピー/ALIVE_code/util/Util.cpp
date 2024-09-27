#include "Util.h"
#include <stdio.h>
#include <algorithm>

namespace MathUtil 
{
	//二点間の距離のサイズを取得する
	float GetSizeOfDistanceTwoPoints(const VECTOR& pos, const VECTOR& targetPos)
	{
		VECTOR distance = {};
		distance = VSub(pos, targetPos);

		return VSize(distance);
	}

	//VECTOR型、度数法を弧度法に変換する
	VECTOR VECTORDegreeToRadian(VECTOR rot)
	{
		rot.x = rot.x * DX_PI_F / 180.0f;
		rot.y = rot.y * DX_PI_F / 180.0f;
		rot.z = rot.z * DX_PI_F / 180.0f;

		return rot;
	}

	//VECTOR型、弧度法を度数法に変換する
	VECTOR VECTORRadianToDegree(VECTOR rot)
	{
		rot.x = rot.x / DX_PI_F * 180.0f;
		rot.y = rot.y / DX_PI_F * 180.0f;
		rot.z = rot.z / DX_PI_F * 180.0f;

		return rot;
	}

	//float型、度数法を弧度法に変換する
	float DegreeToRadian(float angle)
	{
		angle = angle * DX_PI_F / 180.0f;

		return angle;
	}

	//float型、弧度法を度数法に変換する
	float RadianToDegree(float angle)
	{
		angle = angle / DX_PI_F * 180.0f;

		return angle;
	}

}

namespace StrUtil
{
	//文字列から数値を取得する
	int StrUtil::GetNumberFromString(const std::string& name, const std::string& sign)
	{
		//文字列の数を取得
		int strSize = static_cast<int>(name.size());

		//文字列の数が0以下だったら0を返す
		if (strSize <= 0)
		{
			return 0;
		}

		//文字列から第二引数がある番号を取得する
		int searchStrNum = static_cast<int>(name.find(sign));

		//上記で取得した番号から一文字先までを取得
		std::string str = name.substr(searchStrNum + 1, strSize);

		//文字列を数値に変換する
		int num = atoi(str.c_str());

		return num;
	}

	//文字列と数値を結合した文字列を取得する
	std::string GetConcatenateNumAndStrings(const std::string& name, const std::string& sign, int num)
	{
		//数値を文字列にする
		std::string value = std::to_string(num);

		//nameに記号(sign)と文字列に変換したvalue(num)を連結
		std::string str = name + sign + value;

		return str;
	}

	//文字列からsign以降の文字列を削除した文字列を取得する
	std::string GetStringWithPartsAfterTheSymbolDeleted(const std::string& name,const std::string& sign)
	{
		//文字列を保管
		std::string str = name;

		//singが文字列に含まれているか探す
		if (name.find(sign) != -1) 
		{
			//sign以降の文字列を削除した文字列を取得する
			str = str.erase(name.find(sign));
		}

		return str;
	}

	//文字列から記号以降の文字列を取得する
	std::string GetStringAfterSign(const std::string& name,const std::string& sign)
	{
		//文字列のサイズを取得
		int size = static_cast<int>(name.size());

		//「.」が文字列の何番目かを取得する
		int dotNum = static_cast<int>(name.find(sign));

		//「.」以降から一文字先までの文字列を取得する
		std::string str = name.substr(dotNum + 1, size);

		return str;
	}

	//文字列から記号以前の文字列を取得する
	std::string GetStringBeforeSign(const std::string& name,const std::string& sign)
	{
		//「.」が文字列の何番目かを取得する
		int dotNum = static_cast<int>(name.find(sign));

		//「.」以降から最後までの文字列を取得する
		std::string str = name.substr(0, dotNum);

		return str;
	}
}