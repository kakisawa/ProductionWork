#include "LoadCsv.h"
#include <fstream>
#include <sstream>
#include <vector>

LoadCsv* LoadCsv::m_instance = nullptr;

namespace
{
	const char* const kCharaInfoFileName = "Data/csv/CharaInfo.csv";

	/// <summary>
	/// ������𕪊�����
	/// </summary>
	/// <param name="input">������</param>
	/// <param name="delimiter">��؂镶��(,)</param>
	/// <returns>��������������</returns>
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

	// �v���C���[�̏ڍ�
	charaData.maxHp = std::stoi(strvec[1]);		// �ő�HP
	charaData.initPosX = std::stof(strvec[2]);	// �������WX
	charaData.initPosY = std::stof(strvec[3]);	// �������WY
	charaData.initPosZ = std::stof(strvec[4]);	// �������WZ
	charaData.modelSize = std::stof(strvec[5]);	// ���f���T�C�Y
	charaData.walkSpeed = std::stof(strvec[6]);	// �������x
	charaData.rotaSpeed = std::stof(strvec[7]);	// ��]���x
}