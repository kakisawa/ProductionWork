#include "ModelManager.h"

#include "../util/ExternalFile.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <cassert>

using json = nlohmann::json;

//デストラクタ
ModelManager::~ModelManager()
{
	for (auto& type : modelHandle_)
	{
		MV1DeleteModel(type.second);
	}
}

//モデルファイルパスを読み込む
void ModelManager::LoadModelFilePath()
{
	//読み込むファイルのパスを生成
	std::string path = "data/jsonFile/modelPath.json";

	//ファイルを開く
	std::ifstream ifs(path.c_str());
	assert(ifs);

	//よくわかっていない
	json json_;
	ifs >> json_;

	//ファイル名の取得
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

	//閉じる
	ifs.close();
}

//モデルのロード
void ModelManager::LoadModel()
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//モデルのロード
	for (const auto& type : modelFilePathInfo_)
	{
		for (const auto& name : type.second)
		{
			std::string path = "data/model/" + type.first + "/mv1/" + name + ".mv1";
			modelHandle_[name] = MV1LoadModel(path.c_str());
		}
	}
}

//3Dモデルハンドルを取得する
int ModelManager::GetModelHandle(const std::string& name)
{
	return modelHandle_[name];
}
