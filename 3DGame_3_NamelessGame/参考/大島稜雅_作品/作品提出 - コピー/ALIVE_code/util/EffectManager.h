#pragma once
#include <map>
#include <list>
#include <vector>
#include <string>
#include <memory>
#include <DxLib.h>

class Effect2D;

class EffectManager
{
private:
	//分割画像の構造体
	struct DivGraphData
	{
		std::string name = {};	//名前

		int divXNum = 0;		//画像のXの分割数
		int divYNum = 0;		//画像のYno分割数

		int divXSize = 0;		//画像のXの分割サイズ
		int divYSize = 0;		//画像のYの分割サイズ
	};
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EffectManager();

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	/// <returns>インスタンス</returns>
	static EffectManager& GetInstance() {
		static EffectManager instance;
		return instance;
	}

	/// <summary>
	/// 分割画像のファイルパスを読み込む
	/// </summary>
	void LoadDivGraphFilePath();

	/// <summary>
	/// 画像の読み込み
	/// </summary>
	void Load();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// エフェクトの追加
	/// </summary>
	/// <param name="name">エフェクト名</param>
	/// <param name="size">描画サイズ</param>
	/// <param name="pos">座標</param>
	void AddEffect(const std::string& name,const float size,const VECTOR& pos);

private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EffectManager();

	EffectManager(const EffectManager&) = delete;
	void operator = (const EffectManager&) = delete;
private:
	int arrayHandle_[30] = {};													//画像の配列

	std::map<std::string, std::vector<int>> handle_ = {};						//画像テーブル
	std::map<std::string, std::list<DivGraphData>>	divGraphFilePathInfo_;		//分割画像ファイルパスをまとめる
	std::list<std::shared_ptr<Effect2D>> effect2D_ = {};						//effect
};

