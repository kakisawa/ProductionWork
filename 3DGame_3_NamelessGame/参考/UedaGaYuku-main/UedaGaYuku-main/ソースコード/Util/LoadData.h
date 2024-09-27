#pragma once
#include <vector>

class CharacterBase;

/// <summary>
/// csvファイル読み込み
/// </summary>
class LoadData
{
public:
	LoadData() {};
	LoadData(CharacterBase& data, int charType);
	~LoadData();
	void Draw();

	// キャラクターのデータを読み込む
	void LoadCharaData(CharacterBase& data, int charType);
	// アニメーション速度のデータを読み込む
	void LoadAnimSpeedData(CharacterBase& data, int charType);
	// 当たり判定のデータを読み込む
	void LoadColData(CharacterBase& data, int charType);
	// 敵のデータを読み込む
	void LoadEnemyData(CharacterBase& data, int charType);

private:
	std::vector<float> m_charData;		// キャラクターのステータス情報
	std::vector<float> m_animSpeedData;	// アニメーション速度情報
	std::vector<float> m_colData;		// 当たり判定情報
	std::vector<float> m_enemyData;		// 敵情報
};