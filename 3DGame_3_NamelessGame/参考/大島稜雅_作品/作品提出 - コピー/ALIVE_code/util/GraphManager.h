#pragma once
#include <map>
#include <list>
#include <string>

class GraphManager
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~GraphManager();

	/// <summary>
	/// インスタンス作成
	/// </summary>
	/// <returns>インスタンス</returns>
	static GraphManager& GetInstance() {
		static GraphManager instance;
		return instance;
	}

	/// <summary>
	/// 画像のファイルパスを読み込む
	/// </summary>
	void LoadGraphFilePath();

	/// <summary>
	/// 画像を読み込む
	/// </summary>
	void Load();
	
	/// <summary>
	/// 画像を取得する
	/// </summary>
	/// <param name="name">画像の名前</param>
	/// <returns>画像ハンドル</returns>
	int GetGraph(const std::string& name);

private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GraphManager() = default;

	GraphManager(const GraphManager&) = delete;
	void operator = (const GraphManager&) = delete;
private:
	std::map<std::string, int> graphHandle_ = {};							//グラフテーブル
	std::map<std::string, std::list<std::string>> graphFilePathInfo_;		//画像ファイルパスをまとめる
};

