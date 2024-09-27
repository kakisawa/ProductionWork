#pragma once
#include <map>
#include <vector>
#include <string>
#include <DxLib.h>

class Tutorial
{
private:
	//UI画像の列挙
	enum class UIGraph
	{
		XboxBotton,
		KeyBord,
		Max,
	};

	//配置ポジション
	struct ArrangementPos
	{
		float x = 0.0f;			//配置するX座標
		float y = 0.0f;			//配置するY座標
	};

	//チュートリアルのデータ
	struct TutorialData
	{
		int keyType = 0;		//キーの種類をint型にcastした値
		bool isPushKey = false;	//そのキーが押されたか
		std::string str = {};	//説明として表示したい文字列
	};
	
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Tutorial();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Tutorial();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pos">プレイヤーのポジション</param>
	void Update(const VECTOR& playerPos);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// キータイプを表示しないか
	/// </summary>
	/// <param name="data">チュートリアルのデータ</param>
	/// <returns>true:表示しない false:表示する</returns>
	bool DoNotDiplayKeyType(const TutorialData& data);

private:
	
	int elapsedTime_ = 0;												//経過時間
	int fontPigumo42_ = -1;												//フォントハンドル
	int currentDisplayNum_ = 0;											//現在の表示する番号

	std::string oldTutorialName_ = {};									//1フレーム前のチュートリアルに使われていた名前を保存する
	std::string currentTutorialName_ = {};								//これから行われるチュートリアルの名前

	std::map<UIGraph, ArrangementPos> UIPos_;							//UIの画面ポジション pairの中身｜first：X｜second：Y
	std::map<std::string, std::vector<TutorialData>> tutorialData_;		//チュートリアルに必要な情報を保管する
};

