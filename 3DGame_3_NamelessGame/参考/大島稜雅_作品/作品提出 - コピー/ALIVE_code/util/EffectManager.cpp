#include "EffectManager.h"
#include "Effect2D.h"

#include "../util/ExternalFile.h"

#include <fstream>
#include <cassert>
#include <algorithm>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

//コンストラクタ
EffectManager::EffectManager()
{
}

//デストラクタ
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

//分割画像のファイルパスを読み込む
void EffectManager::LoadDivGraphFilePath()
{
	//読み込むファイルのパスを生成
	std::string path = "data/jsonFile/divGraphPath.json";

	//ファイルを開く
	std::ifstream ifs(path.c_str());
	assert(ifs);

	//よくわかっていない
	json json_;
	ifs >> json_;

	//ファイル名の取得
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

	//閉じる
	ifs.close();
}

//画像の読み込み
void EffectManager::Load()
{
	//分割画像の読み込み
	for (const auto& place : divGraphFilePathInfo_)
	{
		for (const auto& info : place.second)
		{
			//パスの作成
			std::string path = "data/" + place.first + "/" + info.name + ".png";

			//総分割数の作成
			int totalDivNum = info.divXNum * info.divYNum;

			//画像の分割読み込み
			LoadDivGraph(path.c_str(), totalDivNum, info.divXNum, info.divYNum, info.divXSize, info.divYSize, arrayHandle_);

			for (int i = 0; i < totalDivNum; i++)
			{
				handle_[info.name].push_back(arrayHandle_[i]);
				arrayHandle_[i] = 0;
			}
		}
	}
}

//更新
void EffectManager::Update()
{
	effect2D_.remove_if([](std::shared_ptr<Effect2D> effectPoint) {return !effectPoint->IsEnable(); });

	//更新
	for (const auto& effect : effect2D_)
	{
		effect->Update();
	}
}

//描画
void EffectManager::Draw()
{
	//描画
	for (const auto& effect : effect2D_)
	{
		effect->DrawBillboard();
	}
}

//エフェクトの追加
void EffectManager::AddEffect(const std::string& name, const float size, const VECTOR& pos)
{
	effect2D_.push_back(std::make_shared<Effect2D>(handle_[name], size, pos));
}