#include "ModelManager.h"

#include "../util/ExternalFile.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <cassert>

using json = nlohmann::json;

//�f�X�g���N�^
ModelManager::~ModelManager()
{
	for (auto& type : modelHandle_)
	{
		MV1DeleteModel(type.second);
	}
}

//���f���t�@�C���p�X��ǂݍ���
void ModelManager::LoadModelFilePath()
{
	//�ǂݍ��ރt�@�C���̃p�X�𐶐�
	std::string path = "data/jsonFile/modelPath.json";

	//�t�@�C�����J��
	std::ifstream ifs(path.c_str());
	assert(ifs);

	//�悭�킩���Ă��Ȃ�
	json json_;
	ifs >> json_;

	//�t�@�C�����̎擾
	for (const auto& scene : json_["scene"])
	{
		for (const auto& name : scene["name"])
		{
			for (const auto& path : name)
			{
				modelFilePathInfo_[scene["type"]].push_back(path);
			}
		}
	}

	//����
	ifs.close();
}

//���f���̃��[�h
void ModelManager::LoadModel()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//���f���̃��[�h
	for (const auto& type : modelFilePathInfo_)
	{
		for (const auto& name : type.second)
		{
			std::string path = "data/model/" + type.first + "/mv1/" + name + ".mv1";
			modelHandle_[name] = MV1LoadModel(path.c_str());
		}
	}
}

//3D���f���n���h�����擾����
int ModelManager::GetModelHandle(const std::string& name)
{
	return modelHandle_[name];
}
