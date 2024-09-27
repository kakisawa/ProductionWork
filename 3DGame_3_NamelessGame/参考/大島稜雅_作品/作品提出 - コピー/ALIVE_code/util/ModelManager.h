#pragma once
#include <list>
#include <string>
#include <DxLib.h>
#include <unordered_map>

class ModelManager
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ModelManager();

	/// <summary>
	/// インスタンス作成
	/// </summary>
	/// <returns>インスタンス</returns>
	static ModelManager& GetInstance() {
		static ModelManager instance;
		return instance;
	}

	/// <summary>
	///	モデルファイルパスを読み込む
	/// </summary>
	void LoadModelFilePath();

	/// <summary>
	/// モデルをロードする
	/// </summary>
	void LoadModel();

	/// <summary>
	/// 3Dモデルハンドルを取得する
	/// </summary>
	/// <returns>モデルハンドル</returns>
	int GetModelHandle(const std::string& name);
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ModelManager() = default;

	ModelManager(const ModelManager&) = delete;
	void operator = (const ModelManager&) = delete;
private:
	std::unordered_map<std::string, std::list<std::string>>	modelFilePathInfo_;			//モデルファイルパスをまとめる
	std::unordered_map<std::string, int> modelHandle_;									//モデルハンドルテーブル
};

