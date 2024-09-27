#include "GraphManager.h"

#include "DrawFunctions.h"

#include "../util/ExternalFile.h"

#include <fstream>
#include <cassert>
#include <DxLib.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

//デストラクタ
GraphManager::~GraphManager()
{
	for (const auto& graph : graphHandle_)
	{
		DeleteGraph(graph.second);
	}
}

//画像のファイルパスを読み込む
void GraphManager::LoadGraphFilePath()
{
	//読み込むファイルのパスを生成
	std::string path = "data/jsonFile/graphPath.json";

	//ファイルを開く
	std::ifstream ifs(path.c_str());
	assert(ifs);

	//よくわかっていない
	json json_;
	ifs >> json_;

	//ファイル名の取得
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

	//閉じる
	ifs.close();
}


//画像の読み込み
void GraphManager::Load()
{
	//画像をロードする
	for (const auto& place : graphFilePathInfo_)
	{
		for (const auto& name : place.second)
		{
			std::string path = "data/" + place.first + "/" + name + ".png";
			graphHandle_[name] = Graph::LoadGraph(path.c_str());
		}
	}
}

//画像の取得
int GraphManager::GetGraph(const std::string& name)
{
	return graphHandle_[name];
}
