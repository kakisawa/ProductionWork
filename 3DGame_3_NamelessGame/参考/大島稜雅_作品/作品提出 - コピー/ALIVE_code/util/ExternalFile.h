#pragma once
#include "../object/PlayerData.h"
#include "../object/ObjectData.h"
#include <list>
#include <deque>
#include <string>
#include <DxLib.h>
#include <unordered_map>

class ExternalFile
{
private:
	//ファイルの種類
	enum class File
	{
		End,
		Main,
		Room,
		UIpos,
		Enemy,
		Gimmick,
		StartPos,
		Tutorial,
		CameraGimmick,
		CameraPosition,
		MainSpecialObj,
		TitleSpecialObj,

		Max,
	};

	//ファイルの名前
	std::string fileData_[static_cast<int>(File::Max)]
	{
		"End",
		"Main",
		"Room",
		"UIpos",
		"Enemy",
		"Gimmick",
		"StartPos",
		"Tutorial",
		"CameraGimmick",
		"CameraPosition",
		"MainSpecialObj",
		"TitleSpecialObj",
	};
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ExternalFile();

	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <returns></returns>
	static ExternalFile& GetInstance() {
		static ExternalFile instance;
		return instance;
	}

	/// <summary>
	/// ファイルをまとめて読み込む
	/// </summary>
	void LoadFile();

	/// <summary>
	/// 配置データを読み込む
	/// </summary>
	void LoadArrangementData();

	/// <summary>
	/// ファイルを読み込む
	/// </summary>
	void LoadFileHandle(const std::string& name);

	/// <summary>
	/// セーブデータの書き出し
	/// </summary>
	/// <param name="num">死亡回数</param>
	void SaveDataRewriteInfo(const int num);


	////////////////Getter////////////////

	/// <summary>
	/// 特定のギミックの配置情報を取得する
	/// </summary>
	/// <param name="name">取得したいオブジェクトの名前</param>
	/// <returns></returns>
	ObjectInfo GetSpecifiedGimmickInfo(const std::string& name);

	/// <summary>
	/// カメラが特殊な動きを行う印(オブジェクト)が
	/// どこにあるかの配置データを取得する
	/// </summary>
	/// <returns></returns>
	ObjectInfo GetCameraGimmickInfo(const VECTOR& playerPos, const std::string& name);

	/// <summary>
	/// タイトルの指定した名前のオブジェクト配置データを返す
	/// </summary>
	/// <param name="name">取得したいオブジェクトの名前</param>
	/// <returns>オブジェクトのデータ</returns>
	ObjectInfo GetTitleSpecifiedInfo(const std::string& name);

	/// <summary>
	/// ゲームメイン指定した名前のオブジェクト配置データを返す
	/// </summary>
	/// <param name="name">取得したいオブジェクトの名前</param>
	/// <returns>オブジェクトのデータ</returns>
	ObjectInfo GetMainSpecifiedInfo(const std::string& name);

	/// <summary>
	/// エンディングシーンの指定した名前のオブジェクト配置データを返す
	/// </summary>
	/// <param name="name">取得したいオブジェクトの名前</param>
	/// <returns>オブジェクトのデータ</returns>
	ObjectInfo GetEndSpecifiedInfo(const std::string& name);

	/// <summary>
	/// エネミーの配置データを取得する
	/// </summary>
	/// <param name="playerPos">プレイヤーのポジション</param>
	/// <returns>配置データ</returns>
	std::list<ObjectInfo> GetEnemyInfo(const VECTOR& playerPos);

	/// <summary>
	/// プレイヤーの開始位置のデータを取得する
	/// </summary>
	/// <param name="name">開始位置の名前</param>
	/// <returns>配置データ</returns>
	VECTOR GetStartPos(const std::string& name);

	/// <summary>
	/// チュートリアルを表示するポイントの配置データを取得する
	/// </summary>
	/// <param name="pos">プレイヤーのポジション</param>
	/// <returns>配置データ</returns>
	ObjectInfo GetTutorialObjInfo(const VECTOR& pos);

	/// <summary>
	/// カメラの座標データを取得する
	/// </summary>
	/// <param name="name">名前</param>
	/// <returns>座標データ</returns>
	VECTOR GetCameraTargetPos(const std::string& name);

	/// <summary>
	/// 指定UIの配置座標を取得する
	/// </summary>
	/// <param name="name">取得したいUIの名前</param>
	/// <returns>座標</returns>
	VECTOR GetUIPos(const std::string& name);

	/// <summary>
	/// プレイヤーに関する情報を取得する
	/// </summary>
	/// <returns>playerData</returns>
	PlayerInfo GetPlayerInfo() const { return player_; }

	/// <summary>
	/// メインステージの特殊なオブジェクトの配置データを取得する
	/// </summary>
	/// <param name="name">名前</param>
	/// <returns>配置データ</returns>
	ObjectInfo GetMainSpecialObjectInfo(const std::string& name) { return loadSpecialInfo_[name]; }

	/// <summary>
	/// 開始場所の名前を取得する
	/// </summary>
	/// <returns>開始場所の名前</returns>
	const std::string& GetStartName()const { return startPosName_; }

	/// <summary>
	/// savedataを取得する
	/// </summary>
	/// <returns>過去の死亡回数</returns>
	const std::deque<int>& GetTotalDeathNum() const { return pastTotalDeathNum_; }

	/// <summary>
	/// オブジェクトの配置情報を取得
	/// </summary>
	/// <returns>配置データ</returns>
	const std::unordered_map<std::string, std::list<ObjectInfo>>& GetGimmickInfo() const { return loadGimmickInfo_; }

	/// <summary>
	/// メインステージオブジェクトの配置、回転率、拡縮率を取得する
	/// </summary>
	/// <returns>オブジェクトの配置データなどをまとめた変数</returns>
	const std::unordered_map<std::string, std::list<ObjectInfo>>& GetLoadMainStageObjectInfo()const { return loadMainStageObjInfo_; }

	/// <summary>
	/// オープニングステージオブジェクトの配置、回転率、拡縮率を取得する
	/// </summary>
	/// <returns>オブジェクトの配置データなどをまとめた変数</returns>
	const std::unordered_map<std::string, std::list<ObjectInfo>>& GetLoadOpeningStageObjectInfo() const { return loadOpeningStageObjInfo_; }

	/// <summary>
	/// エンディングシーンオブジェクトの配置、回転率、拡縮率を取得する
	/// </summary>
	/// <returns>オブジェクトの配置データなどをまとめた変数</returns>
	const std::unordered_map<std::string, std::list<ObjectInfo>>& GetLoadEndingStageObjectInfo()const { return loadEndStageObjInfo_; }


	////////////////Setter////////////////

	/// <summary>
	/// 開始場所の名前を設定する
	/// </summary>
	/// <param name="name">開始場所の名前</param>
	void SetStartName(const std::string& name);

	/// <summary>
	/// 死んだ回数をセットする
	/// </summary>
	/// <param name="num"></param>
	void SetDeathCount(const int num);
private:

	/// <summary>
	/// プレイヤーのステータス情報を書き出す 
	/// </summary>
	void RewritePlayerInfo();

	/// <summary>
	/// プレイヤーのステータスに関する情報を読み込む
	/// </summary>
	void LoadPlayerInfo();

	/// <summary>
	/// セーブデータを読み込む
	/// </summary>
	/// <param name="filename">ファイルの名前</param>
	void LoadSaveDataInfo(const std::string& filename);

	/// <summary>
	/// オブジェクトの配置情報を読み込みlistに格納する
	/// </summary>
	/// <param name="filename">ファイルの名前</param>
	/// <param name="dataTable">データテーブル</param>
	void LoadObjectDataList(const std::string& name,std::unordered_map<std::string, std::list<ObjectInfo>>& dataTable);

	/// <summary>
	/// オブジェクトの配置情報を読み込む
	/// </summary>
	/// /// <param name="filename">ファイルの名前</param>
	/// <param name="dataTable">データテーブル</param>
	void LoadObjectData(const std::string& name,std::unordered_map<std::string, ObjectInfo>& dataTable);
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ExternalFile();

	ExternalFile(const ExternalFile&) = delete;
	void operator = (const ExternalFile&) = delete;
private:
	PlayerInfo player_ = {};																//プレイヤーのステータス情報

	std::string startPosName_ = {};															//開始位置の名前

	std::deque<int> pastTotalDeathNum_ = {};												//過去5回の死亡数

	std::unordered_map<std::string, int> loadFile_;											//ロードしたファイル

	std::unordered_map<std::string, std::list<ObjectInfo>>	loadMainStageObjInfo_;		//メインステージオブジェクトの配置データ
	std::unordered_map<std::string, std::list<ObjectInfo>>	loadOpeningStageObjInfo_;	//オープニングステージオブジェクトの配置データ
	std::unordered_map<std::string, std::list<ObjectInfo>>	loadEndStageObjInfo_;		//オープニングステージオブジェクトの配置データ
	std::unordered_map<std::string, std::list<ObjectInfo>>	loadGimmickInfo_;			//ギミックの配置データ	
	std::unordered_map<std::string, std::list<ObjectInfo>>	loadCameraGimmickInfo_;		//カメラギミックの配置データ		
	std::unordered_map<std::string, std::list<ObjectInfo>>	loadEnemyInfo_;				//敵の配置データ
	
	std::unordered_map<std::string, ObjectInfo>				loadUIInfo_;				//UIの配置データ	
	std::unordered_map<std::string, ObjectInfo>				loadStartPos_;				//開始位置のデータ	
	std::unordered_map<std::string, ObjectInfo>				loadSpecialInfo_;			//特殊なオブジェクトの配置データ	
	std::unordered_map<std::string, ObjectInfo>				loadTutorialInfo_;			//チュートリアルポイントの配置データ	
	std::unordered_map<std::string, ObjectInfo>				loadCameraPosInfo_;			//カメラの配置データ	
};

