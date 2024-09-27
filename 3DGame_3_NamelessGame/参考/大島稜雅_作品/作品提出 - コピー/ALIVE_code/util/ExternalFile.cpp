#include "ExternalFile.h"
#include "Util.h"
#include "InputState.h"
#include "../object/ObjectData.h"

#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cassert>

using json = nlohmann::json;

//コンストラクタ
ExternalFile::ExternalFile()
{
}

//デストラクタ
ExternalFile::~ExternalFile()
{
	RewritePlayerInfo();
}

//ファイルをまとめて読み込む
void ExternalFile::LoadFile()
{
	for (const auto& file : fileData_)
	{
		LoadFileHandle(file);
	}
}

//配置データを読み込む
void ExternalFile::LoadArrangementData()
{
	LoadPlayerInfo();
	LoadSaveDataInfo("saveData");
	//マップに格納する
	LoadObjectData(fileData_[static_cast<int>(File::UIpos)], loadUIInfo_);
	LoadObjectData(fileData_[static_cast<int>(File::StartPos)], loadStartPos_);
	LoadObjectData(fileData_[static_cast<int>(File::Tutorial)], loadTutorialInfo_);
	LoadObjectData(fileData_[static_cast<int>(File::MainSpecialObj)], loadSpecialInfo_);
	LoadObjectData(fileData_[static_cast<int>(File::CameraPosition)], loadCameraPosInfo_);
	LoadObjectData(fileData_[static_cast<int>(File::TitleSpecialObj)], loadSpecialInfo_);
	//マップのリストに格納する
	LoadObjectDataList(fileData_[static_cast<int>(File::Enemy)], loadEnemyInfo_);
	LoadObjectDataList(fileData_[static_cast<int>(File::End)], loadEndStageObjInfo_);
	LoadObjectDataList(fileData_[static_cast<int>(File::Gimmick)], loadGimmickInfo_);
	LoadObjectDataList(fileData_[static_cast<int>(File::Main)], loadMainStageObjInfo_);
	LoadObjectDataList(fileData_[static_cast<int>(File::Room)], loadOpeningStageObjInfo_);
	LoadObjectDataList(fileData_[static_cast<int>(File::CameraGimmick)], loadCameraGimmickInfo_);
}

//ファイルを読み込む
void ExternalFile::LoadFileHandle(const std::string& name)
{
	//ファイルパスの生成
	std::string filepath = "data/objData/";
	filepath = filepath + name + ".pos";

	//ファイルのロード
	loadFile_[name] = FileRead_open(filepath.c_str());
}

//セーブデータの書き出し
void ExternalFile::SaveDataRewriteInfo(const int num)
{
	json saveData = 
	{
		{"name","saveData"},
		{"pastTotalDeath",pastTotalDeathNum_},
	};

	std::string filename = saveData["name"];
	std::string extension = ".json";
	filename += extension;
	filename = "data/jsonFile/" + filename;

	std::ofstream writeing_file;
	writeing_file.open(filename, std::ios::out);
	writeing_file << saveData.dump() << std::endl;
	writeing_file.close();
}

//特定のギミックの配置情報を取得する
ObjectInfo ExternalFile::GetSpecifiedGimmickInfo(const std::string& name)
{

	ObjectInfo info = {};

	//番号と記号を抜いた名前を取得する
	std::string gimmickName = StrUtil::GetStringWithPartsAfterTheSymbolDeleted(name, ".");

	//引数の名前とloadGimmickInfo_の配列の中で名前が一致するものを探す
	//一致した配列の情報を取得する
	for (const auto& specifiedObj : loadGimmickInfo_[gimmickName])
	{
		if (specifiedObj.name != name)
		{
			continue;
		}
		info = specifiedObj;
	}

	//抜き出した情報を返す
	return info;
}

//カメラが特殊な動きを行う印(オブジェクト)が
// どこにあるかの配置データを取得する
ObjectInfo ExternalFile::GetCameraGimmickInfo(const VECTOR& playerPos, const std::string& name)
{

	float minDistance = 10000.0f;
	ObjectInfo info = {};

	for (const auto& data : loadCameraGimmickInfo_[name])
	{
		float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(data.pos, playerPos);
		if (minDistance > distanceSize)
		{
			minDistance = distanceSize;
			info = data;
		}
	}
	return info;
}

//タイトルの指定した名前のオブジェクト配置データを返す
ObjectInfo ExternalFile::GetTitleSpecifiedInfo(const std::string& name)
{
	ObjectInfo info = {};
		
	for (const auto& obj : loadOpeningStageObjInfo_)
	{
		if (obj.first == name) 
		{
			info = loadOpeningStageObjInfo_[name].front();
		}
	}

	return info;
}

//ゲームメインの指定した名前のオブジェクト配置データを返す
ObjectInfo ExternalFile::GetMainSpecifiedInfo(const std::string& name)
{
	ObjectInfo info = {};

	for (const auto& obj : loadMainStageObjInfo_)
	{
		if (obj.first == name)
		{
			info = loadMainStageObjInfo_[name].front();
		}
	}

	return info;
}

//エンディングシーンの指定した名前のオブジェクト配置データを返す
ObjectInfo ExternalFile::GetEndSpecifiedInfo(const std::string& name)
{
	ObjectInfo info = {};

	for (const auto& obj : loadEndStageObjInfo_)
	{
		if (obj.first == name)
		{
			info = loadEndStageObjInfo_[name].front();
		}
	}

	return info;
}

//エネミーの配置データを取得する
std::list<ObjectInfo> ExternalFile::GetEnemyInfo(const VECTOR& playerPos)
{
	std::list<ObjectInfo> info = {};
	float distanceSize = 0.0f;
	float minSize = 3000.0f;

	for (const auto& list : loadEnemyInfo_)
	{
		for (const auto& enemy : list.second)
		{
			distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(enemy.pos, playerPos);
			if (distanceSize < minSize) 
			{
				info.push_back(enemy);
			}
		}
	}

	return info;
}

//プレイヤーの開始位置のデータを取得する
VECTOR ExternalFile::GetStartPos(const std::string& name)
{
	if (static_cast<int>(name.size()) == 0)
	{
		return loadStartPos_["Chapter0"].pos;
	}

	return loadStartPos_[name].pos;
}

//チュートリアルを表示するポイントの配置データを取得する
ObjectInfo ExternalFile::GetTutorialObjInfo(const VECTOR& pos)
{

	ObjectInfo info = {};
	float distanceSize = 0.0f;
	float min = 10000.0f;

	for (const auto& tutorial : loadTutorialInfo_)
	{
		distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(tutorial.second.pos, pos);
		if (min > distanceSize) 
		{
			min = distanceSize;
			info = tutorial.second;
		}
	}

	return info;
}


//カメラの座標データを取得する
VECTOR ExternalFile::GetCameraTargetPos(const std::string& name)
{
	VECTOR pos = {};

	for (const auto& data : loadCameraPosInfo_)
	{
		auto& keyName = data.first;
		if (keyName == name) 
		{
			pos = data.second.pos;
		}
	}

	return pos;
}

//指定UIの配置座標を取得する
VECTOR ExternalFile::GetUIPos(const std::string& name)
{
	VECTOR pos = {};

	for (const auto& data : loadUIInfo_)
	{
		auto& keyName = data.first;
		if (keyName == name) 
		{
			pos = data.second.pos;
		}
	}

	return pos;
}

//開始場所の名前を設定する
void ExternalFile::SetStartName(const std::string& name)
{
	startPosName_ = name;
}

//死んだ回数をセットする
void ExternalFile::SetDeathCount(const int num)
{
	pastTotalDeathNum_.pop_front();
	pastTotalDeathNum_.push_back(num);
}

//プレイヤーのステータスを書き出す
void ExternalFile::RewritePlayerInfo()
{
	//プレイヤーのステータスをまとめる
	json player = 
	{
	   {"name","player"},
	   {"jumpPower",10.0f},
	   {"runningJumpPower",8.0f},
	   {"rotSpeed",15.0f},
	   {"walkSpeed",5.0f},
	   {"runningSpeed",10.0f},
	};

	//書き出す場所の指定
	std::string filename = player["name"];
	std::string extension = ".json";
	filename += extension;
	filename = "data/jsonFile/" + filename;

	//出力
	std::ofstream writeing_file;
	writeing_file.open(filename, std::ios::out);
	writeing_file << player.dump() << std::endl;
	writeing_file.close();
}

//プレイヤーのステータス情報を読み込む
void ExternalFile::LoadPlayerInfo()
{
	//ファイルパス
	std::string path = "data/jsonFile/player.json";

	//ファイルを開く
	std::ifstream ifs(path.c_str());
	assert(ifs);

	//よくわからない
	json json_;
	ifs >> json_;

	//プレイヤーのステータスの取得
	player_.jumpPower			= json_["jumpPower"];
	player_.runningJumpPower	= json_["runningJumpPower"];
	player_.rotSpeed			= json_["rotSpeed"];
	player_.walkSpeed			= json_["walkSpeed"];
	player_.runningSpeed		= json_["runningSpeed"];

	//閉じる
	ifs.close();
}

//セーブデータを読み込む
void ExternalFile::LoadSaveDataInfo(const std::string& filename)
{
	std::string path = "data/jsonFile/";
	path += filename;
	path += ".json";

	std::ifstream ifs(path.c_str());
	assert(ifs);

	json json;
	ifs >> json;

	for (int i = 0; i < 5; i++)
	{
		pastTotalDeathNum_.push_back(json["pastTotalDeath"][i]);
	}

	ifs.close();
}

//オブジェクトのポジションを読み込む
void ExternalFile::LoadObjectDataList(const std::string& name,std::unordered_map<std::string, std::list<ObjectInfo>>& dataTable)
{
	//読み込んだデータのハンドルを取得
	auto dataHandle = loadFile_[name.c_str()];

	//データ数の取得
	int dataNum = 0;
	int result = FileRead_read(&dataNum, sizeof(dataNum), dataHandle);
	assert(result != -1);

	for (int i = 0; i < dataNum; i++)
	{

		ObjectInfo info = {};

		//名前の文字列数を読み取る
		uint8_t nameSize = 0;
		result = FileRead_read(&nameSize, sizeof(nameSize), dataHandle);
		assert(result != -1);

		//名前そのものを読み取る
		info.name.resize(nameSize);
		result = FileRead_read(info.name.data(), sizeof(char) * nameSize, dataHandle);
		assert(result != -1);

		//座標データをinfo.pos分読み取る
		result = FileRead_read(&info.pos, sizeof(info.pos), dataHandle);
		assert(result != -1);

		//回転率をinfo.rot分読み取る
		result = FileRead_read(&info.rot, sizeof(info.rot), dataHandle);
		assert(result != -1);

		//拡縮率をinfo.scale分読み取る
		result = FileRead_read(&info.scale, sizeof(info.scale), dataHandle);
		assert(result != -1);

		std::string keyName = StrUtil::GetStringWithPartsAfterTheSymbolDeleted(info.name, ".");

		//追加
		dataTable[keyName].push_back(info);
	}

	//ファイルを閉じる
	FileRead_close(dataHandle);

	//読み取ったデータの回転率を度数法から弧度法へと変換する
	for (auto& obj : dataTable)
	{
		for (auto& objSecond : obj.second)
		{
			objSecond.rot = MathUtil::VECTORDegreeToRadian(objSecond.rot);
		}
	}
}

//オブジェクトの配置情報を読み込む
void ExternalFile::LoadObjectData(const std::string& name,std::unordered_map<std::string, ObjectInfo>& dataTable)
{
	//読み込んだデータのハンドルを取得
	auto dataHandle = loadFile_[name.c_str()];

	//データ数の取得
	int dataNum = 0;
	int result = FileRead_read(&dataNum, sizeof(dataNum), dataHandle);
	assert(result != -1);

	for (int i = 0; i < dataNum; i++) 
	{
		ObjectInfo info = {};

		//名前の文字列数を読み取る
		uint8_t nameSize = 0;
		result = FileRead_read(&nameSize, sizeof(nameSize), dataHandle);
		assert(result != -1);

		//名前そのものを読み取る
		info.name.resize(nameSize);
		result = FileRead_read(info.name.data(), sizeof(char) * nameSize, dataHandle);
		assert(result != -1);

		//座標データをinfo.pos分読み取る
		result = FileRead_read(&info.pos, sizeof(info.pos), dataHandle);
		assert(result != -1);

		//回転率をinfo.rot分読み取る
		result = FileRead_read(&info.rot, sizeof(info.rot), dataHandle);
		assert(result != -1);

		//拡縮率をinfo.scale分読み取る
		result = FileRead_read(&info.scale, sizeof(info.scale), dataHandle);
		assert(result != -1);

		std::string keyName = StrUtil::GetStringWithPartsAfterTheSymbolDeleted(info.name, ".");

		//追加
		dataTable[keyName] = info;
	}

	//ファイルを閉じる
	FileRead_close(dataHandle);

	//読み取ったデータの回転率を度数法から弧度法へと変換する
	for (auto& obj : dataTable) 
	{
		obj.second.rot = MathUtil::VECTORDegreeToRadian(obj.second.rot);
	}
}

