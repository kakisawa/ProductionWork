#include "LoadCsv.h"
#include <fstream>
#include <sstream>
#include <vector>

LoadCsv* LoadCsv::m_instance = nullptr;

namespace
{
	const char* const kCharaInfoFileName = "Data/csv/CharaInfo.csv";

	/// <summary>
	/// 文字列を分割する
	/// </summary>
	/// <param name="input">文字列</param>
	/// <param name="delimiter">区切る文字(,)</param>
	/// <returns>分割した文字列</returns>
	std::vector<std::string> split(std::string& input, char delimiter)
	{
		std::istringstream stream(input);
		std::string field;
		std::vector<std::string> result;
		while (getline(stream, field, delimiter))
		{
			result.push_back(field);
		}
		return result;
	}
}

void LoadCsv::LoadData(Player::CharaData& charaData)
{
	std::ifstream ifs(kCharaInfoFileName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();
	}

	// プレイヤーの詳細
	charaData.maxHp = std::stoi(strvec[1]);		// 最大HP
	charaData.initPosX = std::stof(strvec[2]);	// 初期座標X
	charaData.initPosY = std::stof(strvec[3]);	// 初期座標Y
	charaData.initPosZ = std::stof(strvec[4]);	// 初期座標Z
	charaData.modelSize = std::stof(strvec[5]);	// モデルサイズ
	charaData.walkSpeed = std::stof(strvec[6]);	// 歩く速度
	charaData.rotaSpeed = std::stof(strvec[7]);	// 回転速度
}