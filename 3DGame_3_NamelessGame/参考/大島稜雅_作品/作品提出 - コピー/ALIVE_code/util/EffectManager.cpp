#include "EffectManager.h"
#include "Effect2D.h"

#include "../util/ExternalFile.h"

#include <fstream>
#include <cassert>
#include <algorithm>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

//�R���X�g���N�^
EffectManager::EffectManager()
{
}

//�f�X�g���N�^
EffectManager::~EffectManager()
{
	for (const auto& table : handle_)
	{
		for (const auto& graph : table.second)
		{
			DeleteGraph(graph);
		}
	}

	for (const auto& graph : arrayHandle_)
	{
		DeleteGraph(graph);
	}
}

//�����摜�̃t�@�C���p�X��ǂݍ���
void EffectManager::LoadDivGraphFilePath()
{
	//�ǂݍ��ރt�@�C���̃p�X�𐶐�
	std::string path = "data/jsonFile/divGraphPath.json";

	//�t�@�C�����J��
	std::ifstream ifs(path.c_str());
	assert(ifs);

	//�悭�킩���Ă��Ȃ�
	json json_;
	ifs >> json_;

	//�t�@�C�����̎擾
	for (const auto& place : json_["place"])
	{
		for (const auto& info : place["info"])
		{
			DivGraphData data = {};

			data.name = info["path"];

			data.divXNum = info["divXNum"];
			data.divYNum = info["divYNum"];

			data.divXSize = info["divXSize"];
			data.divYSize = info["divYSize"];

			divGraphFilePathInfo_[place["type"]].push_back(data);
		}
	}

	//����
	ifs.close();
}

//�摜�̓ǂݍ���
void EffectManager::Load()
{
	//�����摜�̓ǂݍ���
	for (const auto& place : divGraphFilePathInfo_)
	{
		for (const auto& info : place.second)
		{
			//�p�X�̍쐬
			std::string path = "data/" + place.first + "/" + info.name + ".png";

			//���������̍쐬
			int totalDivNum = info.divXNum * info.divYNum;

			//�摜�̕����ǂݍ���
			LoadDivGraph(path.c_str(), totalDivNum, info.divXNum, info.divYNum, info.divXSize, info.divYSize, arrayHandle_);

			for (int i = 0; i < totalDivNum; i++)
			{
				handle_[info.name].push_back(arrayHandle_[i]);
				arrayHandle_[i] = 0;
			}
		}
	}
}

//�X�V
void EffectManager::Update()
{
	effect2D_.remove_if([](std::shared_ptr<Effect2D> effectPoint) {return !effectPoint->IsEnable(); });

	//�X�V
	for (const auto& effect : effect2D_)
	{
		effect->Update();
	}
}

//�`��
void EffectManager::Draw()
{
	//�`��
	for (const auto& effect : effect2D_)
	{
		effect->DrawBillboard();
	}
}

//�G�t�F�N�g�̒ǉ�
void EffectManager::AddEffect(const std::string& name, const float size, const VECTOR& pos)
{
	effect2D_.push_back(std::make_shared<Effect2D>(handle_[name], size, pos));
}