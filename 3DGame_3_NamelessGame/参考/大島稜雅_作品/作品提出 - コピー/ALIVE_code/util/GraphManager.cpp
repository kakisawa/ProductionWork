#include "GraphManager.h"

#include "DrawFunctions.h"

#include "../util/ExternalFile.h"

#include <fstream>
#include <cassert>
#include <DxLib.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

//�f�X�g���N�^
GraphManager::~GraphManager()
{
	for (const auto& graph : graphHandle_)
	{
		DeleteGraph(graph.second);
	}
}

//�摜�̃t�@�C���p�X��ǂݍ���
void GraphManager::LoadGraphFilePath()
{
	//�ǂݍ��ރt�@�C���̃p�X�𐶐�
	std::string path = "data/jsonFile/graphPath.json";

	//�t�@�C�����J��
	std::ifstream ifs(path.c_str());
	assert(ifs);

	//�悭�킩���Ă��Ȃ�
	json json_;
	ifs >> json_;

	//�t�@�C�����̎擾
	for (const auto& place : json_["place"])
	{
		for (const auto& name : place["name"])
		{
			for (const auto& path : name)
			{
				graphFilePathInfo_[place["type"]].push_back(path);
			}
		}
	}

	//����
	ifs.close();
}


//�摜�̓ǂݍ���
void GraphManager::Load()
{
	//�摜�����[�h����
	for (const auto& place : graphFilePathInfo_)
	{
		for (const auto& name : place.second)
		{
			std::string path = "data/" + place.first + "/" + name + ".png";
			graphHandle_[name] = Graph::LoadGraph(path.c_str());
		}
	}
}

//�摜�̎擾
int GraphManager::GetGraph(const std::string& name)
{
	return graphHandle_[name];
}
